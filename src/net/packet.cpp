#include "packet.h"

#include "../io/crc.h"
#include "../log.h"
#include "../os.h"
#include "../config.h"

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

void Packet::runCRC(){
    crc=0;
    crc = CRC((byte*)this,this->length);
}



byte* payload_buffer=nullptr;
long  payload_buffer_len=0;
Payload* payload_buffer_user;


Payload::Payload(int id,int len,byte* dat){
    type=id;
    length=len;
    data=dat;
}


bool ReliablePacketEnvelope::should_send(){
    return (time_ms()-this->last_sent) > config::network_resend_interval;
}

MultipartPayload::MultipartPayload():packets_recieved(){}
MultipartPayload::~MultipartPayload(){
    if(assembled_payload != nullptr){free(assembled_payload);assembled_payload=nullptr;}
}
void MultipartPayload::Start(MultipartPacket*p){
    id = p->id;
    packets_recieved.Initialize(p->segment_count);
    assembled_payload = (byte*)calloc(p->length,1);
}

void MultipartPayload::Add(MultipartPacket*p){
    memcpy(&assembled_payload[p->segment_offset],p->data,p->segment_length);
    packets_recieved.Set(p->segment);
}

void MultipartPayload::Clear(){
    packets_recieved.Resize(0);
    if(assembled_payload != nullptr){free(assembled_payload);assembled_payload=nullptr;}
}

bool MultipartPayload::isFullyAssembled(){
    return (packets_recieved.CountBitsUnset()==0);
}