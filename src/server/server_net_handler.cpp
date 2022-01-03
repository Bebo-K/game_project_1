#include "server_net_handler.h"
#include "../net/network.h"
#include "../net/packet_builder.h"


Server* ServerNetHandler::server=nullptr;
    
void ServerNetHandler::Init(Server* s){
    server = s;
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
                default:break;
            }
            if(payload.free_after_use){free(payload.data);}
            payload=ServerNetwork::RecvFromTarget(i);
        }
    }
}
void ServerNetHandler::Free(){
    server=nullptr;
}