#include <game_project_1/server/server_net_handler.hpp>
#include <game_project_1/net/network.hpp>
#include <game_project_1/net/packet_builder.hpp>
#include <game_project_1/server/server_scene_controller.hpp>

Server* ServerNetHandler::server=nullptr;
    
void ServerNetHandler::Init(Server* s){
    server = s;
}
void ServerNetHandler::Free(){
    server=nullptr;
}


void ServerNetHandler::Update(int frames){
    if(!ServerNetwork::IsRunning())return;
    Payload payload(0,0,nullptr);
    for(int i=0;i<server->max_players;i++){
        if(!ServerNetwork::TargetConnected(i))continue;

        payload=ServerNetwork::RecvFromTarget(i);
        while(payload.type != 0){
            switch(payload.type){
                case PacketID::CHAT:{ServerNetwork::SendToAllTargets(payload);break;}
                case PacketID::SNPS:{
                    PacketData::SNPS start_new_player_save(payload);
                    SavePlayer* new_player = server->server_save.NewPlayer(
                        start_new_player_save.GetCharacterName(),
                        start_new_player_save.GetCharacterAppearance());

                    Scene* new_player_scene = server->GetActiveScene(new_player->player_scene);
                    if(new_player_scene == nullptr){
                        new_player_scene = server->LoadScene(new_player->player_scene);
                    }
                    int new_player_entity_id = ServerSceneController::TransitionUnitEntity(nullptr,new_player_scene,new_player->player_scene_entrance,new_player->player_unit.ToUnit());

                    //;
                    break;}
                default:break;
            }
            if(payload.free_after_use){free(payload.data);}
            payload=ServerNetwork::RecvFromTarget(i);
        }
    }
}



//NOTE: These are not called on the main thread. 
Packet ServerNetHandler::OnPlayerConnect(Packet* JOIN,int target_id){
    PacketData::JOIN join_request(JOIN);
    Packet response;

    PacketData::ACPT accept_data(&response);
        accept_data.SetPlayerID(target_id);
        accept_data.SetPlayerSaveID(server->server_save.GetPlayerSaveID(join_request.GetPlayerName()));
        accept_data.SetAckID(JOIN->id);
        accept_data.SetPlayerMax(server->max_players);
        accept_data.SetPlayerCount(server->current_players);
        accept_data.WritePacket();

    wchar* name = join_request.GetPlayerName();
    Packet player_info;
        PacketData::NPLR new_player_data(&player_info);
        new_player_data.SetPlayerID(target_id);
        new_player_data.SetPlayerName(name);
        new_player_data.WritePacket();
    ServerNetwork::SendToOtherTargets(&player_info,target_id);

    for(int i=0;i<server->max_players;i++){
        if(i==target_id)continue;
        if(!server->players[i].connected)continue;
        PacketData::PINF current_player_info(&player_info);
            current_player_info.SetPlayerID(i);
            current_player_info.SetPlayerEntityID(server->players[i].player_entity_id);  
            current_player_info.SetPlayerName(server->players[i].player_name);  
            current_player_info.SetCharacterName(server->players[i].character_name);  

        ServerNetwork::SendToTarget(target_id,&player_info);
    }
    server->players[target_id].player_name = wstr::new_copy(name);
    server->players[target_id].connected = true;
    server->current_players++;//will this mess anything up?

    return response;
}
void ServerNetHandler::OnPlayerDisconnect(wchar* reason,int target_id){
    Packet PLDC;
        PacketData::PLDC disconnected_player_data(&PLDC);
        disconnected_player_data.SetPlayerID(target_id);
        disconnected_player_data.SetPlayerName(server->players[target_id].player_name);
        disconnected_player_data.SetReason(reason);
        disconnected_player_data.WritePacket();
    ServerNetwork::SendToAllTargets(&PLDC);
    server->players[target_id].connected=false;
}

void ServerNetHandler::OnPlayerFailConnect(wchar* player_name,wchar* reason){
    Packet PLDC;
        PacketData::PLDC disconnected_player_data(&PLDC);
        disconnected_player_data.SetPlayerID(-1);
        disconnected_player_data.SetPlayerName(player_name);
        disconnected_player_data.SetReason(reason);
    ServerNetwork::SendToAllTargets(&PLDC);
}