#include <game_project_1/server/server_net_handler.hpp>
#include <game_project_1/net/network.hpp>
#include <game_project_1/net/payload.hpp>
#include <game_project_1/net/packets.hpp>
#include <game_project_1/content/base_content.hpp>
#include <game_project_1/component/shared/character_info.hpp>
#include <game_project_1/component/shared/inventory.hpp>


Server* ServerNetHandler::server=nullptr;
ServerScene dummy_scene = ServerScene();

//Components that should be sent to clients by default when loading into a scene or spawning new entities
ServerComponentMask ServerNetHandler::initial_sync_components = ServerComponentMask(bitmask::all).Without<Inventory>();
//Components that should be seen by clients when updated server-side
ServerComponentMask ServerNetHandler::delta_components = ServerComponentMask(bitmask::all).Without<Inventory>();


void ServerNetHandler::Init(Server* s){server = s;}
void ServerNetHandler::Free(){server = nullptr;}
void ServerNetHandler::Update(int frames){
    if(!ServerNetwork::IsRunning())return;
    for(int i=0;i<server->max_players;i++){
        if(!ServerNetwork::PlayerConnected(i))continue;
        Payload payload = ServerNetwork::RecvFromPlayer(i);
        while(payload.type != 0){
            HandlePayload(i,payload);
            if(payload.free_after_use){free(payload.data);}
            payload = ServerNetwork::RecvFromPlayer(i);
        }
    }
}

void ServerNetHandler::HandlePayload(int player_slot,Payload payload){
    switch(payload.type){
        case PacketID::JOIN:{OnPlayerConnect(player_slot,payload);break;}
        case PacketID::CHAT:{ServerNetwork::SendToAllPlayers(payload);break;}
        case PacketID::SNPS:{OnStartNewPlayerSave(player_slot,Packet::SNPS(payload));break;}
        case PacketID::CDLT:{OnClientDelta(player_slot,payload);break;}
        default:break;
    }
}

void ServerNetHandler::SendToPlayersInArea(Payload p,int area_id){
    for(int i=0;i<server->max_players;i++){
        if(!ServerNetwork::PlayerConnected(i))continue;
        if(server->players[i].entity_scene != area_id)continue;
        ServerNetwork::SendToPlayer(i,p);
    }
}


void ServerNetHandler::OnPlayerConnect(int player_slot,Payload request){
    if(server->players[player_slot].persona != null){
        return;/*Duplicate*/
    }
    Packet::JOIN join_info(request);
    server->current_players++;

    Player* new_player = &server->players[player_slot];
        new_player->persona = wstr::new_copy(join_info.persona_buffer);
    int player_save_id = join_info.player_save_id;
    SavePlayer* player_save = server->scene_manager.save.GetPlayer(player_save_id);

    if(player_save != null){new_player->save = player_save;}
    else{player_save_id = server->scene_manager.save.NewPlayerSaveID();}

    Packet::ACPT accept_response;//Accept join request and send back server info and player IDs
        accept_response.ack_id = request.id;
        accept_response.player_count = server->current_players;
        accept_response.player_max =server->max_players;
        accept_response.player_slot_id = player_slot;
        accept_response.player_save_id = player_save_id;
    ServerNetwork::SendToPlayer(player_slot,accept_response.GetPayload());

    if(player_save != nullptr){//If the player has an existing save, go ahead and continue it from the last entrance
        server->scene_manager.TransitionPlayer(0,player_save->last_scene,player_save->last_entrance,player_slot);
        OnPlayerSceneTransition(player_slot,player_save->last_scene);
    }

    Packet::PLYR player_info;//send PLYR packets to all current players for the newly joined player
        player_info.player_id=player_slot;
        player_info.SetPersona(join_info.persona_buffer);
    ServerNetwork::SendToOtherPlayers(player_info.GetPayload(),player_slot);

    //send PLYR packets to the new client for all previously connected players
    for(int i=0;i<server->max_players;i++){
        if(i==player_slot)continue;
        if(ServerNetwork::PlayerConnected(i)){
            player_info.player_id=i;
            player_info.SetPersona(server->players[i].persona);
            ServerNetwork::SendToPlayer(player_slot,player_info.GetPayload());
        }            
    }

    logger::infoW(L"Player %S has joined (slot %d)\n",join_info.persona_buffer,player_slot);
}

void ServerNetHandler::OnPlayerDisconnect(int player_slot,wchar* reason){
    server->scene_manager.SaveAndRemovePlayer(player_slot);

    Packet::PLDC dc_notification;
        dc_notification.player_id = player_slot;
        dc_notification.SetPlayerAndReason(server->players[player_slot].persona,reason);
    ServerNetwork::SendToOtherPlayers(dc_notification.GetPayload(),player_slot);
}

void ServerNetHandler::OnStartNewPlayerSave(int player_slot,Packet::SNPS new_save_info){
    SavePlayer* save_player = server->scene_manager.save.NewPlayer(new_save_info.save_id);
    
    Player* my_player = &server->players[player_slot];
    my_player->save = save_player;

    ServerEntity* player_base_entity = new ServerEntity(-1);

    Identity* identity = new Identity();
        identity->name = wstr::new_copy(new_save_info.player_name);
        identity->type = BaseContent::HUMANOID;

    CharacterInfo* char_data = new CharacterInfo();
        char_data->class_id = new_save_info.class_id;
        char_data->race_id = new_save_info.race_id;
        char_data->appearance.color1 = new_save_info.color_1;
        char_data->appearance.style1 = new_save_info.style_1;
        char_data->appearance.style2 = new_save_info.style_2;
        char_data->appearance.style3 = new_save_info.style_3; 

    player_base_entity->Set(identity);
    player_base_entity->Set(char_data);

    save_player->character.SetFromEntity(player_base_entity);
    server->scene_manager.TransitionPlayer(my_player,0,save_player->last_scene,save_player->last_entrance);
    OnPlayerSceneTransition(player_slot,save_player->last_scene);
    delete player_base_entity;
}

void ServerNetHandler::OnPlayerInfoUpdate(int player_slot){
    Packet::PINF player_info;
        player_info.player_id = player_slot;
        player_info.player_entity = server->players[player_slot].entity_id;
        player_info.player_area_id = server->players[player_slot].entity_scene;
        player_info.SetCharacterName(server->players[player_slot].character_name);
        
    ServerNetwork::SendToOtherPlayers(player_info.GetPayload(),player_slot);
}

void ServerNetHandler::OnPlayerSceneTransition(int player_slot,int area_id){
    logger::debug("Sending player %d scene transition to area %d\n",player_slot,area_id);
    Payload new_scene_payload = WriteFullScene(
        &server->players[player_slot],
        server->scene_manager.GetActiveScene(area_id));

    ServerNetwork::SendToPlayer(player_slot,new_scene_payload);
    free(new_scene_payload.data);
    OnPlayerInfoUpdate(player_slot);
}

void ServerNetHandler::OnClientDelta(int player_slot,Payload delta){
    Player* player = &server->players[player_slot];
    ParseClientDelta(player,
        server->scene_manager.GetActiveScene(player->entity_scene),delta);
}

void ServerNetHandler::SendEntityDeltas(ServerScene* s){
    Payload delta_payload = WriteSceneDelta(s);
    Payload spawn_payload = WriteSceneNewEntities(s);
    Payload despawn_payload = WriteSceneDeletedEntities(s);

    if(delta_payload.data != null){
        SendToPlayersInArea(delta_payload,s->area_id);
        free(delta_payload.data);
    }

    if(spawn_payload.data != null){
        SendToPlayersInArea(spawn_payload,s->area_id);
        free(spawn_payload.data);
    }

    if(despawn_payload.data != null){
        SendToPlayersInArea(despawn_payload,s->area_id);
        free(despawn_payload.data);
    }
}

