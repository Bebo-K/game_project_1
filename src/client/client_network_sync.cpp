#include "client_network_sync.h"

#include "../net/packet_builder.h"


using namespace ClientNetworkSynchronizer;

ClientNetwork* net=nullptr;
int my_player_id=-1;
int my_entity_id=-1;


void Init(ClientNetwork* handled_net){
    net=handled_net;
}


void Update(int frames){
    Payload payload(0,0,nullptr);
    payload=net->Recieve();
    while(payload.type != 0){
        switch(payload.type){
            case PacketID::ACPT:{
                my_player_id = ((int*)payload.data)[1];//[0] is ack ID
                break;}
            case PacketID::PLYR:{break;}
            case PacketID::PLDC:{break;}
            case PacketID::SPWN:{break;}
            case PacketID::DSPN:{break;}
            case PacketID::DLTA:{break;}
            case PacketID::CHAT:{break;}
            default:break;
        }
        if(payload.free_after_use){free(payload.data);}
        payload=net->Recieve();
    }
}