#include <game_project_1/net/packet.hpp>
#include <game_project_1/io/crc.hpp>
#include <game_project_1/log.hpp>
#include <game_project_1/os.hpp>
#include <game_project_1/config.hpp>

const int reliable_packet_ids[] = {
        PacketID::ACPT,
        PacketID::PINF,
        PacketID::NPLR,
        PacketID::PLDC,
        PacketID::SNPS,
        PacketID::CONT,
        PacketID::SCNE,
        PacketID::SPWN,
        PacketID::DSPN,
        PacketID::CHAT,
    };

void Packet::ClearData(){
    memset(data,0,MAX_DATA_LENGTH);
}
void Packet::CreateID(){
    id = rand();
}
void Packet::SetDataLength(int data_len){
    length = data_len+HEADER_SIZE;
}
bool Packet::IsReliable(){
    for(int id:reliable_packet_ids){
        if(this->type==id)return true;
    }
    return false;
}
bool Packet::IsMultipart(){
    return length >= MAX_UDP_PACKET_SIZE;
}

Payload Packet::getPayload(){
    return Payload(this->type,this->length-HEADER_SIZE,this->timestamp,this->data);
}
                         

void MultipartPacket::ClearData(){
    memset(data,0,MAX_DATA_LENGTH);
}
void MultipartPacket::CreateID(){
    while(id == 0){id = rand();}
}
void MultipartPacket::SetPayloadLength(int payload_len){
    length = payload_len;
}
void MultipartPacket::SetPacketDataLength(int data_len){
    packet_data_length = data_len;
}
bool MultipartPacket::IsReliable(){
    for(int id:reliable_packet_ids){
        if(this->type==id)return true;
    }
    return false;
}


Payload::Payload(){
    id=0;
    type=0;
    length=0;
    data=null;
    timestamp=0;
    free_after_use=false;
}
Payload::Payload(int id,int type,int len,int time,byte* dat){
    id=id;
    type=type;
    length=len;
    data=dat;
    timestamp=time;
    free_after_use=false;
}
Payload::Payload(int type,int len,int time,byte* dat){
    id=rand();
    type=type;
    length=len;
    data=dat;
    timestamp=time;
    free_after_use=false;
}
Payload::Payload(Packet& p){
    id=p.id;
    type=p.type;
    length=p.length-Packet::HEADER_SIZE;
    data=p.data;
    timestamp=p.timestamp;
    free_after_use=false;
}
bool Payload::IsReliable(){
    for(int reliable_ids:reliable_packet_ids){
        if(this->type==reliable_ids)return true;
    }
    return false;
}

ReliablePayloadEnvelope::ReliablePayloadEnvelope(){
    type=0;
    length=0;
    data =nullptr;

    reliable_id=0;
    last_sent=0;
    retry_count=0;
}
ReliablePayloadEnvelope::~ReliablePayloadEnvelope(){
    type=0;
    length=0;
    if(data != nullptr){free(data);data=nullptr;}
    reliable_id=0;
    last_sent=0;
    retry_count=0;
}

bool ReliablePayloadEnvelope::ShouldSend(){
    return (OS::time_ms()-last_sent) > config::network_resend_interval;
}

MultipartPayload::MultipartPayload():packets_recieved(){}
MultipartPayload::~MultipartPayload(){
    if(assembled_payload != nullptr){free(assembled_payload);assembled_payload=nullptr;}
}
void MultipartPayload::Start(MultipartPacket*p){
    type = p->type;
    len = p->length;
    id = p->id;
    packets_recieved.Initialize(p->segment_count);
    assembled_payload = (byte*)calloc(p->length,1);
}
void MultipartPayload::Add(MultipartPacket*p){
    memcpy(&assembled_payload[p->payload_offset],p->data,p->packet_data_length);
    packets_recieved.Set(p->segment);
    last_recv=OS::time_ms();
}
void MultipartPayload::Clear(){
    packets_recieved.Resize(0);
    if(assembled_payload != nullptr){free(assembled_payload);assembled_payload=nullptr;}
}
bool MultipartPayload::IsFullyAssembled(){
    return (packets_recieved.CountBitsUnset()==0);
}
bool MultipartPayload::IsExpired(){
    return ((int)(OS::time_ms() - last_recv) > config::network_packet_expiry);
}