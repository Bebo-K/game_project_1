#include <game_project_1/server/server_signal.hpp>
#include <game_project_1/server/server.hpp>
#include <game_project_1/threads.hpp>

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
            default:break;
        }
    }
}
void ServerSignalHandler::Free(){
    server=nullptr;
}