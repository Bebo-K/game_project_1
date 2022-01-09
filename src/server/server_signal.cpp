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
            default:break;
        }
    }
}
void ServerSignalHandler::Free(){
    server=nullptr;
}