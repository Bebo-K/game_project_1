#include "server_signal.h"
#include "../net/packet_builder.h"
#include "../threads.h"
#include "../net/network.h"
#include "server.h"

Server* ServerSignalHandler::server=nullptr;
SynchronousBuffer server_signals(sizeof(EventSignal),4);

void ServerSignalHandler::Init(Server* s){
    server=s;
}
void ServerSignalHandler::Signal(EventSignal val){
    server_signals.Write((byte*)&val);
}
void ServerSignalHandler::Update(int frames){
    EventSignal signal = {0};
    while(server_signals.Read((byte*)&signal)){
        switch(signal.type){
            case ServerSignalID::PLAYER_CONNECTED:{
                wchar* name = signal.params[0].strval;
                int player_id = signal.params[1].ival;
                Packet player_info;
                    PacketData::PLYR new_player_data(&player_info);
                    new_player_data.SetPlayerID(player_id);
                    new_player_data.SetPlayerName(name);
                    new_player_data.WritePacket();
                ServerNetwork::SendToOtherTargets(&player_info,player_id);

                for(int i=0;i<server->max_players;i++){
                    if(i==player_id)continue;
                    if(!server->players[i].connected)continue;
                    PacketData::PINF current_player_info(&player_info);
                        current_player_info.SetPlayerID(i);
                        current_player_info.SetPlayerEntityID(server->players[i].player_entity_id);  
                        current_player_info.SetPlayerName(server->players[i].player_name);  
                        current_player_info.SetCharacterName(server->players[i].character_name);  

                    ServerNetwork::SendToTarget(player_id,&player_info);
                }

                server->players[player_id].player_name = wstr::new_copy(name);
                server->players[player_id].connected = true;

                server->current_players++;
                free(name);
                break;}
            case ServerSignalID::PLAYER_DISCONNECTED:{
                int player_id = signal.params[0].ival;
                wchar* reason = signal.params[1].strval;
                Packet PLDC;
                    PacketData::PLDC disconnected_player_data(&PLDC);
                    disconnected_player_data.SetPlayerID(player_id);
                    disconnected_player_data.SetPlayerName(server->players[player_id].player_name);
                    disconnected_player_data.SetReason(reason);
                    disconnected_player_data.WritePacket();
                ServerNetwork::SendToAllTargets(&PLDC);
                server->current_players--;
                free(reason);
                int player_scene_id =server->players[player_id].player_entity_scene;
                int player_entity_id = server->players[player_id].player_entity_id;
                if(player_scene_id >= 0 && player_entity_id >= 0){
                    server->active_scenes[player_scene_id]->RemoveEntity(player_entity_id);
                }
                server->players[player_id].Clear();
                break;}
            case ServerSignalID::PLAYER_FAIL_CONNECT:{
                wchar* name = signal.params[0].strval;
                wchar* reason = signal.params[1].strval;
                Packet PLDC;
                    PacketData::PLDC disconnected_player_data(&PLDC);
                    disconnected_player_data.SetPlayerID(-1);
                    disconnected_player_data.SetPlayerName(name);
                    disconnected_player_data.SetReason(reason);
                ServerNetwork::SendToAllTargets(&PLDC);
                free(name);
                free(reason);
                break;}
            default:break;
        }
    }
}
void ServerSignalHandler::Free(){
    server=nullptr;
}