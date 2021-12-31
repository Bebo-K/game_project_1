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
    const int JOIN = CSTR_TO_PACKETID("JOIN");//R* Client join request
    const int ACPT = CSTR_TO_PACKETID("ACPT");//R Server accepts new client connection
    const int OKAY = CSTR_TO_PACKETID("OKAY");//   (Both sides) Acknowledge reliable packet 
    const int SPRQ = CSTR_TO_PACKETID("SPRQ");//R  Client spawn request
    const int PLYR = CSTR_TO_PACKETID("PLYR");//R  Server notification of a new player
    const int PLDC = CSTR_TO_PACKETID("PLDC");//R  Server notification of a player disconnecting
    const int PING = CSTR_TO_PACKETID("PING");//   Latency loopback
    const int NOPE = CSTR_TO_PACKETID("NOPE");//   Disconnect (Either side)
    const int SPWN = CSTR_TO_PACKETID("SPWN");//R  Spawn entity
    const int DLTA = CSTR_TO_PACKETID("DLTA");//   Entity delta
    const int CHAT = CSTR_TO_PACKETID("CHAT");//R  Client text chat
    //* JOIN uses the reliable packet resend buffer, but is acknowledged with ACPT instead of OKAY (ACPT is still ack'd with an OKAY however)

void Packet::ClearData(){
    memset(data,0,MAX_DATA_LENGTH);
}
void Packet::CreateID(){
    id = rand();
}
void Packet::SetDataLength(int data_len){
    length = data_len+HEADER_SIZE;
}
void Packet::RunCRC(){
    crc=0;
    crc = CRC((byte*)this,this->length);
}
bool Packet::IsReliable(){
    for(int id:reliable_packet_ids){
        if(this->type==id)return true;
    }
    return false;
}
bool Packet::IsMultipart(){
    return length > MAX_UDP_PACKET_SIZE;
}



void MultipartPacket::ClearData(){
    memset(data,0,MAX_DATA_LENGTH);
}
void MultipartPacket::CreateID(){
    id = rand();
}
void MultipartPacket::SetDataLength(int data_len){
    length = data_len+HEADER_SIZE;
}
void MultipartPacket::RunCRC(){
    crc=0;
    crc = CRC((byte*)this,this->length);
}
bool MultipartPacket::IsReliable(){
    for(int id:reliable_packet_ids){
        if(this->type==id)return true;
    }
    return false;
}

Payload::Payload(int id,int len,byte* dat){
    type=id;
    length=len;
    data=dat;
    free_after_use=false;
}

bool ReliablePacketEnvelope::ShouldSend(){
    return (time_ms()-last_sent) > config::network_resend_interval;
}

MultipartPayload::MultipartPayload():packets_recieved(){}
MultipartPayload::~MultipartPayload(){
    if(assembled_payload != nullptr){free(assembled_payload);assembled_payload=nullptr;}
}
void MultipartPayload::Start(MultipartPacket*p){
    type = p->type;
    len = p->payload_length;
    id = p->id;
    packets_recieved.Initialize(p->segment_count);
    assembled_payload = (byte*)calloc(p->length,1);
}
void MultipartPayload::Add(MultipartPacket*p){
    memcpy(&assembled_payload[p->payload_offset],p->data,p->length);
    packets_recieved.Set(p->segment);
    last_recv=time_ms();
}
void MultipartPayload::Clear(){
    packets_recieved.Resize(0);
    if(assembled_payload != nullptr){free(assembled_payload);assembled_payload=nullptr;}
}
bool MultipartPayload::IsFullyAssembled(){
    return (packets_recieved.CountBitsUnset()==0);
}
bool MultipartPayload::IsExpired(){
    return ((int)(time_ms() - last_recv) > config::network_packet_expiry);
}