#include <game_project_1/server/server_net_handler.hpp>
#include <game_project_1/net/network.hpp>
#include <game_project_1/net/packet_builder.hpp>
#include <game_project_1/base/base_player.hpp>

Server* ServerNetHandler::server=nullptr;
    
const ComponentChunk::Mask server_private_component_mask = ~(1 << ComponentChunk::INVENTORY);

void ServerNetHandler::Init(Server* s){
    server = s;
}
void ServerNetHandler::Free(){
    server=nullptr;
}


void ServerNetHandler::Update(int frames){
    if(!ServerNetwork::IsRunning())return;
    Payload payload(0,0,0,nullptr);
    for(int i=0;i<server->max_players;i++){
        if(!ServerNetwork::TargetConnected(i))continue;
        payload=ServerNetwork::RecvFromTarget(i);
        while(payload.type != 0){
            switch(payload.type){
                case PacketID::CHAT:{ServerNetwork::SendToAllTargets(payload);break;}
                case PacketID::SNPS:{
                    OnPlayerConnected(i);OnStartNewPlayerSave(i,payload);break;}
                case PacketID::CONT:{
                    OnPlayerConnected(i);break;}
                case PacketID::CDLT:{
                    ServerEntity* player_entity = server->GetActiveScene(server->players[i].entity_scene)->GetEntity(server->players[i].entity_id);
                    Deserializer playerDelta(payload.data);
                    int client_given_entity_id= playerDelta.GetInt();
                    if(player_entity->id != client_given_entity_id){
                        logger::warn("Client is sending a delta for an entity they're not. Ours:%d Theirs:%d\n",player_entity->id,client_given_entity_id);
                    }
                    player_entity->Deserialize(playerDelta,payload.timestamp);
                    break;}
                default:break;
            }
            if(payload.free_after_use){free(payload.data);}
            payload=ServerNetwork::RecvFromTarget(i);
        }
    }
}

void ServerNetHandler::OnPlayerConnected(int player_id){    
    Packet player_info;
        PacketData::NPLR new_player_data(&player_info); //Send NPLR packets for client to other players
        new_player_data.SetPlayerID(player_id);
        new_player_data.SetPlayerName(server->players[player_id].name);
        new_player_data.WritePacket();
    ServerNetwork::SendToOtherTargets(&player_info,player_id);

    for(int i=0;i<server->max_players;i++){//send PINF packets to the new client for previously connected players
        if(server->players[i].state <= Player::DISCONNECTED)continue;
        if(i==player_id)continue;
        PacketData::PINF current_player_info(&player_info);
            current_player_info.SetPlayerID(i);
            current_player_info.SetPlayerName(server->players[i].name); 
            current_player_info.SetCharacterName(server->players[i].character_name); 
            current_player_info.SetPlayerEntityID(server->players[i].entity_id);   
            current_player_info.SetPlayerEntityArea(server->players[i].entity_scene);

        ServerNetwork::SendToTarget(player_id,&player_info);
    }

    if(server->players[player_id].save_id != 0){
        SavePlayer* save = server->save.GetPlayerByID(server->players[player_id].save_id);
        server->players[player_id].save_entity_id = save->entity_global_id;
        server->TransitionPlayer(0,save->last_scene,save->last_entrance,player_id);
    }
    //else we wait for a StartNewPlayerSave packet
}

void ServerNetHandler::OnStartNewPlayerSave(int player_id,Payload snps_packet){
    SavePlayer* save = server->save.NewPlayer(server->players[player_id].name);
    server->players[player_id].save_id=save->player_id;
    PacketData::SNPS start_new_player_save(snps_packet);

    ServerEntity* player_base = new ServerEntity(0);//generate temporary player entity outside of any scene
    player_base->player_id=player_id;
    player_base->entity_class_id=EntityClass::Humanoid;
    player_base->name = wstr::new_copy(start_new_player_save.GetCharacterName());
    player_base->char_data = new Character();
    player_base->char_data->race_id =  start_new_player_save.GetRaceID();
    player_base->char_data->class_id =  start_new_player_save.GetClassID();
    player_base->char_data->appearance.Copy(start_new_player_save.GetCharacterAppearance());
    HumanoidServerBuilder(player_base,nullptr);

    save->entity_global_id = server->save.PersistEntity(player_base);   
    server->players[player_id].save_entity_id = save->entity_global_id;
    
    delete player_base;

    server->TransitionPlayer(0,save->last_scene,save->last_entrance,player_id);
}

void ServerNetHandler::OnPlayerSceneTransition(int player_id,int area_id){
    int total_scne_length=sizeof(int)*3;//area_id,your_entity_id,entity_count

    ServerScene* scene = server->GetActiveScene(area_id);//should be loaded by now.
    for(ServerEntity* e:scene->entities){
        total_scne_length+= sizeof(int) + e->SerializedLength(e->AllExistingComponents());//entity_id, entity
    }
    Payload scene_payload(PacketID::SCNE,total_scne_length,0,(byte*)calloc(total_scne_length,1));
    Serializer scene_serializer(scene_payload.data,total_scne_length);
        scene_serializer.PutInt(area_id);
        scene_serializer.PutInt(server->players[player_id].entity_id);
        scene_serializer.PutInt(scene->entities.Count());
        
        logger::debug("Sending player %d scene transition to area %d\n",player_id,area_id);

    for(ServerEntity* e:scene->entities){
        scene_serializer.PutInt(e->id);
        e->Serialize(e->AllExistingComponents() & server_private_component_mask,scene_serializer);
    }

    ServerNetwork::SendToTarget(player_id,scene_payload);
    free(scene_payload.data);
    server->players[player_id].state = Player::PLAYING;

    Packet player_info; PacketData::PINF current_player_info(&player_info);//Tell other clients that player is moving
        current_player_info.SetPlayerID(player_id);
        current_player_info.SetPlayerName(server->players[player_id].name); 
        current_player_info.SetCharacterName(server->players[player_id].character_name); 
        current_player_info.SetPlayerEntityID(server->players[player_id].entity_id);   
        current_player_info.SetPlayerEntityArea(area_id);
    ServerNetwork::SendToOtherTargets(&player_info,player_id);
}

void ServerNetHandler::SendEntityDeltas(ServerScene* scene){
    int total_create_length=sizeof(int);//num_entities
    int total_delete_length=sizeof(int);//num_entities
    int total_delta_length=sizeof(int);//num_entities
    int created_entites=0;
    int updated_entities=0;
    int deleted_entities=0;

    

    for(ServerEntity* e:scene->entities){//count how big delta packets are first
        if(e->just_created){//Create
            total_create_length += sizeof(int)*3;//id, create_style, delta_mask
            total_create_length += e->SerializedLength(e->delta_mask & server_private_component_mask);
            created_entites++;
        }
        else if(e->just_deleted){//Delete
            total_delete_length += sizeof(int)*2;//id, delete style
            deleted_entities++;
        }
        else if((e->delta_mask & server_private_component_mask) != 0){//Update
            total_delta_length += sizeof(int)*2;//id, delta_mask
            total_delta_length += e->SerializedLength(e->delta_mask & server_private_component_mask);
            updated_entities++;
        }
    }
    Payload create_payload(PacketID::SPWN,total_create_length,0,
            (created_entites > 0)?(byte*)calloc(total_create_length,1):nullptr);
    Payload delete_payload(PacketID::DSPN,total_delete_length,0,
            (deleted_entities > 0)?(byte*)calloc(total_delete_length,1):nullptr);
    Payload delta_payload(PacketID::DLTA,total_delta_length,0,
            (updated_entities > 0)?(byte*)calloc(total_delta_length,1):nullptr);
            
    Serializer create_serializer(create_payload.data,total_create_length);
        if(created_entites > 0){create_serializer.PutInt(created_entites);}
    Serializer delete_serializer(delete_payload.data,total_delete_length);
        if(deleted_entities > 0){delete_serializer.PutInt(deleted_entities);}
    Serializer delta_serializer(delta_payload.data,total_delta_length);
        if(updated_entities > 0){delta_serializer.PutInt(updated_entities);}

    for(ServerEntity* e:scene->entities){//Create
        if(e->just_created){
            create_serializer.PutInt(e->id);
            create_serializer.PutInt(0);
            e->Serialize(e->delta_mask & server_private_component_mask,create_serializer);
            e->delta_mask=0;
        }
        else if(e->just_deleted){//Delete
            delete_serializer.PutInt(e->id);
            delete_serializer.PutInt(0);//TODO: create/delete style IDs
        }
        else if(e->delta_mask != 0){//Update
            delta_serializer.PutInt(e->id);
            e->Serialize(e->delta_mask & server_private_component_mask,delta_serializer);
            e->delta_mask=0;
        }
    }

    for(int i=0;i<server->max_players;i++){//Send to all players in the zone
        if(server->players->entity_scene == scene->area_id){
            if(created_entites >0){ServerNetwork::SendToTarget(i,create_payload);}
            if(updated_entities >0){ServerNetwork::SendToTarget(i,delta_payload);}
            if(deleted_entities >0){ServerNetwork::SendToTarget(i,delete_payload);}
        }
    }

    //free delta payloads;
    if(create_payload.data){free(create_payload.data);}
    if(delta_payload.data){free(delta_payload.data);}
    if(delete_payload.data){free(delete_payload.data);}
}



//ASYNC: These are not called on the main thread. 
Packet ServerNetHandler::OnPlayerConnect(Packet* JOIN,int target_id){
    PacketData::JOIN join_request(JOIN);
    Packet response;

    wchar* name = join_request.GetPlayerName();
    SavePlayer* player_save = server->save.GetPlayer(name);  
    int save_id = (player_save != null)? player_save->player_id:0;
    
    server->players[target_id].name = wstr::new_copy(name);
    server->players[target_id].state = Player::CONNECTING;
    server->players[target_id].save_id = save_id;
    server->current_players++;

    PacketData::ACPT accept_data(&response);//Send ACPT packet back to client
        accept_data.SetPlayerID(target_id);
        accept_data.SetPlayerSaveID(save_id);
        accept_data.SetAckID(JOIN->id);
        accept_data.SetPlayerMax(server->max_players);
        accept_data.SetPlayerCount(server->current_players);
        accept_data.WritePacket();

    return response;
}

//ASYNC: These are not called on the main thread. 
void ServerNetHandler::OnPlayerDisconnect(wchar* reason,int target_id){
    Packet PLDC;
        PacketData::PLDC disconnected_player_data(&PLDC);
        disconnected_player_data.SetPlayerID(target_id);
        disconnected_player_data.SetPlayerName(server->players[target_id].name);
        disconnected_player_data.SetReason(reason);
        disconnected_player_data.WritePacket();
    ServerNetwork::SendToAllTargets(&PLDC);
    server->players[target_id].state=Player::DISCONNECTED;
}

//ASYNC: These are not called on the main thread. 
void ServerNetHandler::OnPlayerFailConnect(wchar* player_name,wchar* reason){
    Packet PLDC;
        PacketData::PLDC disconnected_player_data(&PLDC);
        disconnected_player_data.SetPlayerID(-1);
        disconnected_player_data.SetPlayerName(player_name);
        disconnected_player_data.SetReason(reason);
    ServerNetwork::SendToAllTargets(&PLDC);
}