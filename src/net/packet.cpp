#include "packet.h"


const int reliable_packet_ids[] = {
        PacketID::ACPT,
        PacketID::SPRQ,
        PacketID::PLYR,
        PacketID::PLDC,
        PacketID::SPWN,
        PacketID::DSPN,
        PacketID::CHAT,
    };


    const int JOIN = CSTR_TO_PACKETID("JOIN");//   Client join request
    const int ACPT = CSTR_TO_PACKETID("ACPT");//R  Server accepts new client connection
    const int OKAY = CSTR_TO_PACKETID("OKAY");//   (Both sides) Acknowledge reliable packet 
    const int SPRQ = CSTR_TO_PACKETID("SPRQ");//R  Client spawn request
    const int PLYR = CSTR_TO_PACKETID("PLYR");//R  Server notification of a new player
    const int PLDC = CSTR_TO_PACKETID("PLDC");//R  Server notification of a player disconnecting
    const int PING = CSTR_TO_PACKETID("PING");//   Latency loopback
    const int NOPE = CSTR_TO_PACKETID("NOPE");//   Disconnect (Either side)
    const int SPWN = CSTR_TO_PACKETID("SPWN");//R  Spawn entity
    const int DLTA = CSTR_TO_PACKETID("DLTA");//   Entity delta
    const int CHAT = CSTR_TO_PACKETID("CHAT");//R  Client text chat

bool Packet::isReliable(){
    for(int id:reliable_packet_ids){
        if(this->type==id)return true;
    }
    return false;
}

bool Packet::isMultipart(){
    return length > MAX_UDP_PACKET_SIZE;
}