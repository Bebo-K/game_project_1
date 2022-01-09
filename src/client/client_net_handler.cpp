#include "client_net_handler.h"
#include "../net/packet_builder.h"
#include "../net/network.h"

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
                    logger::info("Connected to server.\n");
                    PacketData::ACPT accept_data(payload);
                        client->player_count = accept_data.GetPlayerCount();
                        client->max_players = accept_data.GetPlayerMax();
                        client->my_player_id = accept_data.GetPlayerID();
                        client->players = (PlayerInfo*)calloc(client->max_players,sizeof(PlayerInfo));
                        client->players[client->my_player_id].player_name=wstr::new_copy(L"Chowzang");

                        client->ui.loading_menu->SetStatusMessage(wstr::new_copy(L"Connected to server!"));

                        
                        if(accept_data.GetPlayerSaveID() > 0){

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

                    if(player_id < 0 || player_id > client->max_players){logger::exception("Recieved data for bad player index %d!",player_id);}
                    client->players[player_id].player_name = wstr::new_copy(player_name);
                    break;
                }
                case PacketID::PINF:{
                    PacketData::PINF player_data(payload);
                        int player_id = player_data.GetPlayerID();
                        int entity_id = player_data.GetPlayerEntityID();
                        wchar* player_name = player_data.GetPlayerName();
                        wchar* character_name = player_data.GetCharacterName();

                    if(player_id < 0 || player_id > client->max_players){logger::exception("Recieved data for bad player index %d!",player_id);}
                    client->players[player_id].player_name = wstr::new_copy(player_name);
                    client->players[player_id].character_name = wstr::new_copy(character_name);
                    client->players[player_id].player_entity_id = entity_id;
                    break;
                }
                case PacketID::PLDC:{
                    PacketData::PLDC player_disconnect(payload);
                        int player_id = player_disconnect.GetPlayerID();
                        wchar* player_name = player_disconnect.GetPlayerName();
                        wchar* reason = player_disconnect.GetReason();
                        
                    client->ui.DebugLog(wstr::allocf(L"Player %ls disconnected (%ls)",player_name,reason));

                    if(player_id > 0 && player_id < client->max_players){
                        client->players[player_id].Clear();
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

void ClientNetHandler::Free(){
    client=nullptr;
}