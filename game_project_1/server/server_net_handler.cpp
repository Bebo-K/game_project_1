#include <game_project_1/server/server_net_handler.hpp>
#include <game_project_1/net/network.hpp>
#include <game_project_1/net/payload.hpp>
#include <game_project_1/net/packet.hpp>
#include <game_project_1/base/base_player.hpp>


Server* ServerNetHandler::server=nullptr;

void ServerNetHandler::Init(Server* s){server = s;}
void ServerNetHandler::Update(int frames){
    if(!ServerNetwork::IsRunning())return;
    for(int i=0;i<server->max_players;i++){
        if(!ServerNetwork::PlayerConnected(i))continue;
        Payload payload = ServerNetwork::RecvFromPlayer(i);
        while(payload.type != 0){
            switch(payload.type){
                case PacketID::CHAT:{ServerNetwork::SendToAllTargets(payload);break;}
                case PacketID::SNPS:{OnStartNewPlayerSave(i,Packet::SNPS(payload));break;}
                case PacketID::CDLT:{OnClientDelta(i,payload);break;}
                default:break;
            }
            if(payload.free_after_use){free(payload.data);}
            payload = ServerNetwork::RecvFromPlayer(i);
        }
    }
}
void ServerNetHandler::Free(){server = nullptr;}

void ServerNetHandler::OnStartNewPlayerSave(int player_slot,Packet::SNPS new_save_info){
    SavePlayer* save = server->save.NewPlayer();
    server->players[player_slot].save = save;

    ServerEntity* player_base_entity = new ServerEntity();
        player_base_entity->entity_class_id = EntityClass::Humanoid;
        player_base_entity->name = new_save_info.player_name;
        player_base_entity->char_data = new Character();
            player_base_entity->char_data->class_id = new_save_info.class_id;
            player_base_entity->char_data->race_id = new_save_info.race_id;
            player_base_entity->char_data->appearance.color1 = new_save_info.color_1;
            player_base_entity->char_data->appearance.style1 = new_save_info.style_1;
            player_base_entity->char_data->appearance.style2 = new_save_info.style_2;
            player_base_entity->char_data->appearance.style3 = new_save_info.style_3; 
    HumanoidInitialPlayerBuilder(player_base_entity);
    save->character_global_id = server->save.PersistEntity(player_base_entity);
    server->TransitionPlayer(0,save->last_scene,save->last_entrance,player_slot);
    delete player_base_entity;
}

void ServerNetHandler::OnPlayerInfoUpdate(int player_slot){
    Packet::PINF player_info();
        player_info.player_id = player_slot;
        player_info.player_entity = server->players[player_slot].entity_id;
        player_info.player_area_id = server->players[player_slot].entity_scene;
        player_info.SetPersona(server->players[player_slot].persona);
        
    ServerNetwork::SendToOtherTargets(player_info.GetPayload(),player_slot);
}

void ServerNetHandler::OnPlayerSceneTransition(int player_slot,int area_id){
    int total_scne_length=sizeof(int)*3;//area_id,your_entity_id,entity_count

    ServerScene* scene = server->GetActiveScene(area_id);//should be loaded by now.
    for(ServerEntity* e:scene->entities){
        total_scne_length+= sizeof(int) + e->SerializedLength(e->AllExistingComponents());//entity_id, entity
    }
    Payload scene_payload(PacketID::SCNE,total_scne_length,0,(byte*)calloc(total_scne_length,1));
    Serializer scene_serializer(scene_payload.data,total_scne_length);
        scene_serializer.PutInt(area_id);
        scene_serializer.PutInt(server->players[player_slot].entity_id);
        scene_serializer.PutInt(scene->entities.Count());
    for(ServerEntity* e:scene->entities){
        scene_serializer.PutInt(e->id);
        e->Serialize(e->AllExistingComponents() & server_private_component_mask,scene_serializer);
    }    
    logger::debug("Sending player %d scene transition to area %d\n",player_slot,area_id);
    ServerNetwork::SendToTarget(player_slot,scene_payload);

    free(scene_payload.data);
    OnPlayerInfoUpdate(player_slot);
}

void ServerNetHandler::OnClientDelta(int player_slot,Payload delta){
    int entity_id = server->players[player_slot].entity_id;
    int area_id = server->players[player_slot].entity_scene;
    ServerEntity* player_entity = server->GetActiveScene(area_id)->GetEntity(entity_id);
    Deserializer playerDelta(payload.data);
    int client_given_entity_id= playerDelta.GetInt();
    if(entity_id != client_given_entity_id){
        logger::warn("Client is sending a delta for an entity they're not. Ours:%d Theirs:%d\n",player_entity->id,client_given_entity_id);
    }
    player_entity->Deserialize(playerDelta,payload.timestamp);
}

void ServerNetHandler::SendEntityDeltas(ServerScene* s){
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

ComponentChunk::Mask ServerNetHandler::GetComponentPermissionMask(ComponentChunk::Mask mask,int player_slot,Server* e){}

void ServerNetHandler::OnPlayerConnect(int player_slot,Packet::JOIN join_info){
    server->current_players++;
    Player* new_player = &server->players[player_slot];
        new_player->persona = wstr::new_copy(join_info.persona_buffer);
    SavePlayer* player_save = server->save.GetPlayer(join_info.player_save_id);
    new_player->save_id = (player_save)?join_info.player_save_id:0;
    if(new_player->save_id != 0){
        server->TransitionPlayer(0,player_save->last_scene,player_save->last_entrance,player_slot);
    }

    Packet::PINF existing_player_info();//send PINF packets to the new client for previously connected players
    for(int i=0;i<server->max_players;i++){
        if(i==player_slot)continue;
        if(ServerNetwork::PlayerConnected(i)){
            existing_player_info.player_id = i;
            existing_player_info.player_entity = server->players[i].entity_id;
            existing_player_info.player_area_id = server->players[i].entity_scene;
            existing_player_info.SetPersona(server->players[i].persona);
            ServerNetwork::SendToTarget(player_slot,existing_player_info.GetPayload());
        }            
    }

    Packet::PLYR new_player_notification();//send PLYR packets to all current players for the newly joined player
        new_player_notification.player_id = player_slot;
        new_player_notification.SetPersona(join_info.persona_buffer);
    ServerNetwork::SendToOtherPlayers(new_player_notification.GetPayload(),player_slot);
}

void ServerNetHandler::OnPlayerDisconnect(int player_slot,wchar* reason){
    Packet::PLDC dc_notification();
        dc_notification.player_id = player_slot;
        dc_notification.SetPlayerAndReason(server->players[player_slot].persona,reason);
    ServerNetwork::SendToOtherPlayers(dc_notification.GetPayload(),player_slot);
    
    server->players[player_slot].Clear();
}

void ServerNetHandler::OnPlayerFailConnect(wchar* persona, wchar* reason){
    Packet::PLDC dc_notification();
        dc_notification.player_id = -1;
        dc_notification.SetPlayerAndReason(persona,reason);
    ServerNetwork::SendToAllPlayers(dc_notification.GetPayload());
}




































































    
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
                case PacketID::SNPS:{OnStartNewPlayerSave(i,Packet::SNPS(payload));break;}
                case PacketID::CDLT:{OnClientDelta(i,payload);break;}
                default:break;
            }
            if(payload.free_after_use){free(payload.data);}
            payload=ServerNetwork::RecvFromTarget(i);
        }
    }
}


void ServerNetHandler::CatchUpNewPlayer(int player_slot){

}

void ServerNetHandler::OnPlayerInfoUpdate(int player_slot){    

}

void ServerNetHandler::OnPlayerSceneTransition(int player_slot,int area_id){

}


void ServerNetHandler::OnClientDelta(int player_slot,Payload delta){
    ServerEntity* player_entity = server->GetActiveScene(server->players[player_slot].entity_scene)->GetEntity(server->players[player_slot].entity_id);
    Deserializer playerDelta(payload.data);
    int client_given_entity_id= playerDelta.GetInt();
    if(player_entity->id != client_given_entity_id){
        logger::warn("Client is sending a delta for an entity they're not. Ours:%d Theirs:%d\n",player_entity->id,client_given_entity_id);
    }
    player_entity->Deserialize(playerDelta,payload.timestamp);
}

void ServerNetHandler::SendEntityDeltas(ServerScene* scene){

}



//ASYNC: These are not called on the main thread. 
void ServerNetHandler::OnPlayerConnect(int player_save_id,int target_id){
    SavePlayer* player_save = server->save.GetPlayerByID(player_save_id);

    if(player_save == nullptr){
        player_save = server->save.NewPlayer();
    }
    
    server->players[target_id].name = null;
    server->players[target_id].state = Player::CONNECTING;
    server->players[target_id].save_id = player_save->player_id;
    server->current_players++;
}

//ASYNC: These are not called on the main thread. 
void ServerNetHandler::OnPlayerDisconnect(int player_slot,wchar* reason){
    Packet::PLDC pldc();
        pldc.SetPlayerAndReason(player_slot,reason);
    ServerNetwork::SendToAllTargets(pldc.GetPayload());

    server->players[player_slot].Clear();
}

//ASYNC: These are not called on the main thread. 
void ServerNetHandler::OnPlayerFailConnect(wchar* player_name,wchar* reason){    
    Packet::PLDC pldc();
        pldc.SetPlayerAndReason(-1,reason);
    ServerNetwork::SendToAllTargets(pldc.GetPayload());
}