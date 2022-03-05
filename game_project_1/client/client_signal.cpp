#include <game_project_1/client/client_signal.hpp>
#include <game_project_1/net/packet_builder.hpp>
#include <game_project_1/threads.hpp>
#include <game_project_1/net/network.hpp>
#include <game_project_1/server/server.hpp>

Client* ClientSignalHandler::client;
SynchronousBuffer client_signals(sizeof(EventSignal),4);

void ClientSignalHandler::Init(Client* c){
    client=c;
}
void ClientSignalHandler::Signal(EventSignal val){
    client_signals.Write((byte*)&val);
}
void ClientSignalHandler::Update(int frames){
    EventSignal signal = {0};
    while(client_signals.Read((byte*)&signal)){
        switch(signal.type){
            case ClientSignalID::DISCONNECTED_FROM_SERVER:{
                int disconnect_code = signal.params[0].ival;
                wchar* disconnect_reason = signal.params[1].strval;
                if(disconnect_code < 0){
                    client->ui.CloseAll();
                    client->ui.error_menu->Open();
                    client->scene.Unload();
                    if(disconnect_reason != null){
                        
                        logger::infoW(L"Disconnected from server (%S)\n",disconnect_reason);
                        client->ui.error_menu->SetStatusMessage(wstr::allocf(L"Disconnected from server (%S)",disconnect_reason));
                        free(disconnect_reason);
                    }
                    else{
                        logger::infoW(L"Disconnected from server (No reason given)\n");
                        client->ui.error_menu->SetStatusMessage(wstr::new_copy(L"Disconnected from server (No reason given)"));
                    }
                }
                else{
                    client->ui.CloseAll();
                    client->ui.main_menu->Open();
                    client->scene.Unload();
                }
                if(Server::GetServer() != nullptr){Server::GetServer()->StartShutdown();}
                break;}
            case ClientSignalID::LOCAL_SERVER_READY:{
                    logger::info("Local server is ready. Connecting...\n");
                    client->ui.loading_menu->SetStatusMessage(wstr::new_copy(L"Connecting to local server..."));
                    Packet join_request;
                        PacketData::JOIN join_data(&join_request);
                        join_data.SetPlayerName(L"Chowzang");
                        join_data.WritePacket();
                    ClientNetwork::LocalConnect(&join_request);
                break;}
            default:break;
        }
    }
}
void ClientSignalHandler::Free(){
    client=nullptr;
}