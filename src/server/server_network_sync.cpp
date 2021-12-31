#include "server_network_sync.h"

#include "../net/packet_builder.h"
#include "../config.h"


using namespace ServerNetworkSynchronizer;

ServerNetwork* net=nullptr;


void Init(ServerNetwork* handled_net){
    net=handled_net;
}


void Update(int frames){
    Payload payload(0,0,nullptr);
    for(int i=0;i<server_config::player_count;i++){
        if(!net->TargetConnected(i))continue;

        payload=net->ReadFromTarget(i);
        while(payload.type != 0){
            switch(payload.type){
                case PacketID::CHAT:{break;}
                default:break;
            }
            if(payload.free_after_use){free(payload.data);}
            payload=net->ReadFromTarget(i);
        }
    }
}