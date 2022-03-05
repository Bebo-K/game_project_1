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

Payload Packet::getPayload(){return Payload(this->type,this->length-HEADER_SIZE,this->data);}


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
    return (OS::time_ms()-last_sent) > config::network_resend_interval;
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