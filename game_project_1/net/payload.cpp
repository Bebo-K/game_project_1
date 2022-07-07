#include <game_project_1/net/payload.hpp>
#include <game_project_1/io/crc.hpp>
#include <game_project_1/log.hpp>
#include <game_project_1/os.hpp>
#include <game_project_1/config.hpp>

Payload::Payload(){Clear();}
Payload::Payload(int id,int type,int len,int time,byte* dat){
    id=id; type=type; length=len; data=dat;
    timestamp=time;
    free_after_use=false;
}
Payload::Payload(int type,int len,byte* dat){
    id=rand(); type=type; length=len; data=dat;
    timestamp = OS::time_ms();
    free_after_use=false;
}
void Payload::Clear(){
    id=0;type=0;length=0;data=null;timestamp=0;free_after_use=false;
}
bool Payload::IsReliable(){
    for(int reliable_ids:reliable_payload_ids){
        if(type==reliable_ids)return true;
    }
    return false;
}

Datagram::Datagram(){Clear();}   
void Datagram::Clear(){
    id=0; type=0; length=0;
    timestamp=0; crc=0;
    memset(data,0,MAX_DATA_LENGTH);
}
void Datagram::CreateID(){
    id = rand();
}
bool Datagram::IsMultipart(){
    return length >= MAX_DATA_LENGTH;
}
MultipartHeader Datagram::getMultipartHeader(){
    return *((MultipartHeader*)data);
}
Payload Datagram::ToPayload(){
    if(IsMultipart()){
        logger::exception("Cannot convert single multipart packet to payload");
    }
    return Payload(id,type,length,timestamp,data);
}
void Datagram::FromPayload(Payload payload){
    if(payload.length > MAX_DATA_LENGTH){
        logger::exception("Cannot build payload of length %d with one packet.",payload.length);
    }
    id = payload.id; if(payload.id == 0){CreateID();}
    type = payload.type;
    length = payload.length;
    memcpy(data,payload.data,payload.length);
    timestamp = OS::time_ms();
    crc = 0; crc = CRC((byte*)this,length+DATAGRAM_HEADER_SIZE);
}

int Datagram::FromPayload(Payload payload,int segment){
    if(payload.length <= MAX_DATA_LENGTH){
        logger::exception("Multipart payload of length %d should be built with only one packet.",payload.length);
    }
    if(payload.id <= 0){
        logger::exception("Multipart payload must have a unique ID");
    }
    MultipartHeader header; const int max_length = MULTIPART_DATA_LENGTH;
    header.segment_count = payload.length/max_length;
        if(payload.length%max_length != 0){header.segment_count++;}
    if(segment < 0 || segment > header.segment_count){
        logger::exception("Invalid segment index %d for payload of segment count %d",segment,header.segment_count);
    }
    header.payload_offset=segment*max_length;
    int remaining = payload.length-header.payload_offset;
    header.packet_data_length=(remaining < max_length)? remaining:max_length;
    header.segment=segment;

    id = payload.id;
    length = payload.length;
    memcpy(data,&payload.data[header.payload_offset],header.packet_data_length);
    timestamp = OS::time_ms();
    crc = 0; crc = CRC((byte*)this,DATAGRAM_HEADER_SIZE+sizeof(MultipartHeader)+header.packet_data_length);

    return header.segment_count;
}

int Datagram::Size(){
    if(IsMultipart()){return DATAGRAM_HEADER_SIZE+sizeof(MultipartHeader)+ getMultipartHeader().packet_data_length;}
    else return length + DATAGRAM_HEADER_SIZE;
}

OutboundReliablePayload::OutboundReliablePayload(){
    data=nullptr;
    Clear();
}
OutboundReliablePayload::OutboundReliablePayload(int type_id,byte* dat,int len){
    id=rand(); type=type_id; data=dat; length=len;
    last_sent=0; retry_count=0;
}
OutboundReliablePayload::~OutboundReliablePayload(){
    Clear();
}
void OutboundReliablePayload::Clear(){
    id=0; type=0; length=0;
    last_sent=0; retry_count=0;
    if(data != nullptr){free(data);data=nullptr;}
}
bool OutboundReliablePayload::ShouldSend(){
    return (OS::time_ms()-last_sent) > config::network_resend_interval;
}
Payload OutboundReliablePayload::GetPayload(){
    return Payload(id,type,length,OS::time_ms(),data);
}

InboundMultipartPayload::InboundMultipartPayload():packets_recieved(){}
InboundMultipartPayload::~InboundMultipartPayload(){
    if(assembled_payload != nullptr){free(assembled_payload);assembled_payload=nullptr;}
}
void InboundMultipartPayload::Start(Datagram* p){
    if(!p->IsMultipart()){
        logger::exception("Packet id %d is not multipart!",p->id);
    }
    MultipartHeader header = p->getMultipartHeader();
    type = p->type;
    len = p->length;
    id = p->id;
    packets_recieved.Initialize(header.segment_count);
    assembled_payload = (byte*)calloc(p->length,1);
}
void InboundMultipartPayload::Add(Datagram* p){
    if(!p->IsMultipart()){
        logger::exception("Packet id %d is not multipart!",p->id);
    }
    MultipartHeader header = p->getMultipartHeader();
    memcpy(&assembled_payload[header.payload_offset],p->data,header.packet_data_length);
    packets_recieved.Set(header.segment);
    last_recv=OS::time_ms();
}

Payload InboundMultipartPayload::Finish(){
    if(!IsFullyAssembled()){
        logger::exception("Cannot finish unassembled packet %d",id);
    }
    Payload multipart_payload(id,type,len,last_recv,assembled_payload);
    multipart_payload.free_after_use=true;
    assembled_payload=nullptr;
    Clear();
    return multipart_payload;
}
void InboundMultipartPayload::Clear(){
    packets_recieved.Resize(0);
    if(assembled_payload != nullptr){free(assembled_payload);assembled_payload=nullptr;}
}
bool InboundMultipartPayload::IsFullyAssembled(){
    return (packets_recieved.CountBitsUnset()==0);
}
bool InboundMultipartPayload::IsExpired(){
    return ((int)(OS::time_ms() - last_recv) > config::network_packet_expiry);
}