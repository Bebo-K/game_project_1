#include <game_project_1/client/client_net_handler.hpp>
#include <game_project_1/net/packets.hpp>
#include <game_project_1/net/network.hpp>
#include <game_project_1/net/client_serializer.hpp>

Client* ClientNetHandler::client=nullptr;


void ClientNetHandler::Init(Client* c){client = c;ClientSerializer::Init(c);}
void ClientNetHandler::Free(){client=nullptr;ClientSerializer::Free();}

void ClientNetHandler::Update(int frames){
    if(ClientNetwork::IsRunning()){
        Payload payload = ClientNetwork::Recieve();
        while(payload.type != 0){
            HandlePayload(payload);
            if(payload.free_after_use){free(payload.data);}
            payload = ClientNetwork::Recieve();
        }
    }
}

void ClientNetHandler::HandlePayload(Payload payload){
    switch (payload.type){
        case PacketID::ACPT:{
            logger::info("Client: Connected to server.\n");
            Packet::ACPT accept_response(payload);

            client->current_players = accept_response.player_count;
            client->players.Resize(accept_response.player_max);
                for(Player* p:client->players){p->Clear();}
            client->my_slot_id = accept_response.player_slot_id;
            client->Me()->persona = wstr::new_copy(config::player_persona);

            client->ui.loading_menu->SetStatusMessage(wstr::new_copy(L"Connected to server!"));

            if(client->my_save_id != accept_response.player_save_id){
                client->my_save_id = accept_response.player_save_id;
                client->ui.character_create_menu->Open();
                client->ui.loading_menu->Close();
            }
            break;
        }
        case PacketID::CHAT:{
            client->ui.DebugLog((wchar*)payload.data);
            break;
        }
        case PacketID::PLYR:{
            Packet::PLYR new_player_data(payload);
            int player_id = new_player_data.player_id;
            if(player_id < 0 || player_id > client->players.length){logger::exception("Recieved data for bad player index %d!",player_id);}
            client->players[player_id]->persona = wstr::new_copy(new_player_data.persona_buffer);
            client->current_players++;
            break;
        }
        case PacketID::PINF:{
            Packet::PINF player_info(payload);
            int player_id = player_info.player_id;
            if(player_id < 0 || player_id > client->players.length){logger::exception("Recieved data for bad player index %d!",player_id);}
            client->players[player_id]->character_name = wstr::new_copy(player_info.character_name_buffer);
            client->players[player_id]->entity_id = player_info.player_entity;
            client->players[player_id]->entity_scene = player_info.player_area_id;
            break;
        }
        case PacketID::PLDC:{
            Packet::PLDC player_disconnect(payload);
            int player_id = player_disconnect.player_id;
            wchar* persona = player_disconnect.GetPlayerName();
            wchar* reason = player_disconnect.GetReason();

            client->ui.DebugLog(wstr::allocf(L"Player %ls disconnected (%ls)",persona,reason));

            if(player_id > 0 && player_id < client->players.length){
                client->players[player_id]->Clear();
            }
            client->current_players--;
            break;
        }
        case PacketID::DLTA:{
            ClientSerializer::DeserializeServerDelta(&client->scene,payload);
            break;
        }
        case PacketID::SPWN:{
            ClientSerializer::DeserializeSpawnEntities(&client->scene,payload);
            break;
        }
        case PacketID::DSPN:{
            ClientSerializer::DeserializeDespawnEntities(&client->scene,payload);
            break;
        }
        case PacketID::SCNE:{
            ClientSerializer::DeserializeNewScene(&client->scene,payload);
            break;
        }
        default:break;
    }
}

void ClientNetHandler::SendPlayerDelta(){
    if(ClientNetwork::IsRunning()){
        ClientEntity* player = client->scene.GetEntity(client->Me()->entity_id);
        Payload delta = ClientSerializer::ClientDelta(player);
        if(delta.data != nullptr){
            ClientNetwork::Send(delta);
        }
    }
}

void ClientNetHandler::CreatePlayer(wchar* playername,int player_race, int player_class,color color1,int style1,int style2,int style3){
    Packet::SNPS start_new_player_save;
        start_new_player_save.SetPlayerName(playername);
        start_new_player_save.save_id=client->my_save_id;
        start_new_player_save.class_id=player_class;
        start_new_player_save.race_id=player_race;
        start_new_player_save.color_1=color1;
        start_new_player_save.style_1=style1;
        start_new_player_save.style_2=style2;
        start_new_player_save.style_3=style3;
    
    client->Me()->character_name = wstr::new_copy(playername);

    ClientNetwork::Send(start_new_player_save.GetPayload());
    client->ui.ingame_menu->Open();
    client->ui.character_create_menu->Close();
}

