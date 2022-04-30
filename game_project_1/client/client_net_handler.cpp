#include <game_project_1/client/client_net_handler.hpp>
#include <game_project_1/net/packet_builder.hpp>
#include <game_project_1/net/network.hpp>

Client* ClientNetHandler::client=nullptr;


void ClientNetHandler::Init(Client* c){
    client = c;
}


void ClientNetHandler::Update(int frames){
    if(ClientNetwork::IsRunning()){
        Payload payload = ClientNetwork::Recieve();
        while(payload.type != 0){
            switch (payload.type){
                case PacketID::ACPT:{
                    logger::info("Client: Connected to server.\n");
                    PacketData::ACPT accept_data(payload);
                        client->current_players = accept_data.GetPlayerCount();
                        client->players.Resize(accept_data.GetPlayerMax());
                        for(Player* p:client->players){p->Clear();}
                        client->my_player_id = accept_data.GetPlayerID();
                        client->my_save_id = accept_data.GetPlayerSaveID();
                        //client->me->name = wstr::new_copy(L"Chowzang");
                        //client->me->state = Player::LOADING_INTO_ZONE;

                        client->ui.loading_menu->SetStatusMessage(wstr::new_copy(L"Connected to server!"));

                        if(client->my_save_id > 0){
                            Packet continue_save;
                            continue_save.type = PacketID::CONT;
                            continue_save.SetDataLength(0);
                            continue_save.CreateID();
                            ClientNetwork::Send(&continue_save);
                        }
                        else{
                            client->ui.character_create_menu->Open();
                            client->ui.loading_menu->Close();
                        }

                    break;
                }
                case PacketID::CHAT:{
                    client->ui.DebugLog((wchar*)payload.data);
                    break;
                }
                case PacketID::NPLR:{
                    PacketData::NPLR new_player_data(payload);
                        int player_id = new_player_data.GetPlayerID();
                        wchar* player_name = new_player_data.GetPlayerName();

                    if(player_id < 0 || player_id > client->players.length){logger::exception("Recieved data for bad player index %d!",player_id);}
                    client->players[player_id]->name = wstr::new_copy(player_name);
                    client->players[player_id]->state = Player::LOADING_INTO_ZONE;
                    client->current_players++;
                    break;
                }
                case PacketID::PINF:{
                    PacketData::PINF player_data(payload);
                        int player_id = player_data.GetPlayerID();

                    if(player_id < 0 || player_id > client->players.length){logger::exception("Recieved data for bad player index %d!",player_id);}
                    client->players[player_id]->name = wstr::new_copy(player_data.GetPlayerName());
                    client->players[player_id]->character_name = wstr::new_copy(player_data.GetCharacterName());
                    client->players[player_id]->entity_id = player_data.GetPlayerEntityID();
                    client->players[player_id]->state = Player::PLAYING;
                    break;
                }
                case PacketID::PLDC:{
                    PacketData::PLDC player_disconnect(payload);
                        int player_id = player_disconnect.GetPlayerID();
                        wchar* player_name = player_disconnect.GetPlayerName();
                        wchar* reason = player_disconnect.GetReason();
                        
                    client->ui.DebugLog(wstr::allocf(L"Player %ls disconnected (%ls)",player_name,reason));

                    if(player_id > 0 && player_id < client->players.length){
                        client->players[player_id]->Clear();
                    }
                    client->current_players--;
                    break;
                }
                case PacketID::DLTA:{
                    Deserializer delta_deserializer(payload.data);
                    int updated_entities = delta_deserializer.GetInt();
                    for(int i=0;i<updated_entities;i++){
                        int entity_id = delta_deserializer.GetInt();
                        ClientEntity* e = client->scene.GetEntity(entity_id);
                        if(e != null){
                            if(e->id == client->my_entity_id){
                                e->Skip(delta_deserializer);//don't overwrite my player
                            }
                            else{
                                e->Deserialize(delta_deserializer,payload.timestamp);
                            }
                        }
                        else{
                            ClientEntity* temp_entity = client->scene.AddEntity(entity_id);
                            temp_entity->Deserialize(delta_deserializer,payload.timestamp);
                        }
                    }
                    break;
                }
                case PacketID::SPWN:{
                    Deserializer spawn_deserializer(payload.data);
                    int create_count = spawn_deserializer.GetInt();
                    for(int i=0;i<create_count;i++){
                        int entity_id = spawn_deserializer.GetInt();
                        int spawn_type_id = spawn_deserializer.GetInt();
                        bool entity_exists = true;
                        ClientEntity* entity = client->scene.GetEntity(entity_id);
                        if(entity == nullptr){
                            entity_exists=false;
                            entity = client->scene.AddEntity(entity_id);
                            client->scene.SpawnEntity(entity,spawn_type_id);
                        }
                        logger::debug("Spawning entity %s with ID %d\n",entity->name,entity->id);
                        if(entity_id == client->my_entity_id){
                            logger::debug("Entity %d is me, setting player control\n",client->my_entity_id);
                            client->scene.SetPlayerControl(entity_id);
                            if(!client->ui.ingame_menu->active){client->ui.ingame_menu->Open();}
                            if(client->ui.loading_menu->active){client->ui.loading_menu->Close();}
                        }
                        if(!entity_exists){
                            entity->Deserialize(spawn_deserializer,payload.timestamp);
                        }
                        else{
                            entity->Skip(spawn_deserializer);
                        }
                    }
                    break;
                }
                case PacketID::DSPN:{
                    Deserializer despawn_deserializer(payload.data);
                    int delete_count = despawn_deserializer.GetInt();
                    for(int i=0;i<delete_count;i++){
                        int entity_id = despawn_deserializer.GetInt();
                        int despawn_type_id = despawn_deserializer.GetInt();
                        //what to do if this is my entity? Unload scene?
                        client->scene.DespawnEntity(entity_id,despawn_type_id);
                    }
                    break;
                }
                case PacketID::SCNE:{
                    Deserializer scene_deserializer(payload.data);
                    int new_area_id = scene_deserializer.GetInt();
                    logger::debug("Recieved server scene transition to area %d\n",new_area_id);
                    client->scene.Load(new_area_id);
                    client->my_entity_id = scene_deserializer.GetInt();
                    int entity_count = scene_deserializer.GetInt();
                    for(int i=0;i<entity_count;i++){
                        ClientEntity* new_entity = client->scene.AddEntity(scene_deserializer.GetInt());
                        new_entity->Deserialize(scene_deserializer,payload.timestamp);
                        client->scene.SpawnEntity(new_entity,0);
                        logger::debug("Spawning entity %s with ID %d\n",new_entity->name,new_entity->id);
                        if(new_entity->id == client->my_entity_id){
                            logger::debug("Entity %d is me, setting player control\n",client->my_entity_id);
                            client->scene.SetPlayerControl(new_entity->id);
                            if(!client->ui.ingame_menu->active){client->ui.ingame_menu->Open();}
                            if(client->ui.loading_menu->active){client->ui.loading_menu->Close();}
                        }
                    }
                    break;
                }
                default:break;
            }
            if(payload.free_after_use){free(payload.data);}
            payload = ClientNetwork::Recieve();
        }
    }
}


void ClientNetHandler::SendPlayerDelta(){
    if(ClientNetwork::IsRunning()){
        Packet CDLT;
        Serializer CDLT_Data(CDLT.data,CDLT.MAX_DATA_LENGTH);

        ClientEntity* player = client->scene.GetEntity(client->my_entity_id);
        ComponentChunk::Mask player_delta_mask = 0;
            player_delta_mask |= 1 << ComponentChunk::POSITION;
            player_delta_mask |= 1 << ComponentChunk::MOVE;
            player_delta_mask |= 1 << ComponentChunk::STATE;

        CDLT.type = PacketID::CDLT;
        CDLT.CreateID();
        CDLT.SetDataLength(sizeof(int) + player->SerializedLength(player_delta_mask));//entity_id, entity
        CDLT_Data.PutInt(player->id);
        player->Serialize(player_delta_mask,CDLT_Data);

        ClientNetwork::Send(&CDLT);
    }
}

void ClientNetHandler::CreatePlayer(wchar* playername,int player_race, int player_class,color color1,int style1,int style2,int style3){
    Packet packet_backing;
    PacketData::SNPS start_new_player_save(&packet_backing);

        start_new_player_save.SetCharacterName(playername);
        start_new_player_save.SetClassID(player_class);
        start_new_player_save.SetRaceID(player_race);
        start_new_player_save.SetColor1(color1);
        start_new_player_save.SetStyle1(style1);
        start_new_player_save.SetStyle2(style2);
        start_new_player_save.SetStyle3(style3);

        start_new_player_save.WritePacket();

    ClientNetwork::Send(&packet_backing);
    client->ui.ingame_menu->Open();
    client->ui.character_create_menu->Close();
}

void ClientNetHandler::Free(){
    client=nullptr;
}