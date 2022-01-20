#include "packet_builder.h"
#include "../os.h"


using namespace PacketData;

const int MAX_PLAYER_NAME_LEN=64;
const int MAX_REASON_LENGTH = 180;


PacketAccessor::PacketAccessor(Packet* p){
    type=0;
    data_length=0;
    data_backing = p->data;
    raw_packet_backing = p;
}
PacketAccessor::PacketAccessor(Payload p){
    type=p.type;
    data_length=p.length;
    data_backing = p.data;
    raw_packet_backing = nullptr;
}
void PacketAccessor::AddDataLength(int len){
    data_length += len;
}
void PacketAccessor::WritePacket(){
    if(raw_packet_backing == nullptr){logger::exception("Unsupported operation: PacketAccessor is read-only for payloads");}
    raw_packet_backing->type = type;
    raw_packet_backing->CreateID();
    raw_packet_backing->SetDataLength(data_length);
    raw_packet_backing->RunCRC();
}


JOIN::JOIN(Packet* p):PacketAccessor(p){type = PacketID::JOIN;}
wchar* JOIN::GetPlayerName(){return ((wchar*)data_backing);}
void   JOIN::SetPlayerName(wchar* name){
    int name_len = wstr::len(name);
    if(name_len > MAX_PLAYER_NAME_LEN){ name_len = MAX_PLAYER_NAME_LEN;}
    memcpy(data_backing,name,name_len*sizeof(wchar));
    ((wchar*)data_backing)[name_len]=0;
    AddDataLength(sizeof(wchar)*(name_len+1));
}

OKAY::OKAY(Packet* p):PacketAccessor(p){type = PacketID::OKAY;data_length=sizeof(int);}
int OKAY::GetAckID(){return ((int*)data_backing)[0];}
void OKAY::SetAckID(int ack){((int*)data_backing)[0] = ack;}

ACPT::ACPT(Packet* p):PacketAccessor(p){type = PacketID::ACPT;data_length=sizeof(int)*5;}
ACPT::ACPT(Payload p):PacketAccessor(p){}
int ACPT::GetAckID(){return ((int*)data_backing)[0];}
void ACPT::SetAckID(int ack){((int*)data_backing)[0] = ack;}
int ACPT::GetPlayerID(){return ((int*)data_backing)[1];}
void ACPT::SetPlayerID(int player_id){((int*)data_backing)[1] = player_id;}
int ACPT::GetPlayerSaveID(){return ((int*)data_backing)[2];}
void ACPT::SetPlayerSaveID(int player_save_id){((int*)data_backing)[2] = player_save_id;}
int ACPT::GetPlayerCount(){return ((int*)data_backing)[3];}
void ACPT::SetPlayerCount(int player_count){((int*)data_backing)[3] = player_count;}
int ACPT::GetPlayerMax(){return ((int*)data_backing)[4];}
void ACPT::SetPlayerMax(int player_max){((int*)data_backing)[4] = player_max;}

NOPE::NOPE(Packet* p):PacketAccessor(p){type = PacketID::NOPE;}
wchar* NOPE::GetReason(){return ((wchar*)data_backing);}
void NOPE::SetReason(wchar* reason){
    int reason_len = wstr::len(reason);
    if(reason_len > MAX_REASON_LENGTH){ reason_len = MAX_REASON_LENGTH;}
    memcpy(data_backing,reason,reason_len*sizeof(wchar));
    ((wchar*)data_backing)[reason_len]=0;
    AddDataLength(sizeof(wchar)*(reason_len+1));
}

PING::PING(Packet* p):PacketAccessor(p){type = PacketID::PING;data_length = sizeof(long)*3;}
long PING::GetTimestamp1(){return ((long*)data_backing)[0];}
long PING::GetTimestamp2(){return ((long*)data_backing)[1];}
long PING::GetTimestamp3(){return ((long*)data_backing)[2];}
void PING::SetTimestamps(long ts_1,long ts_2,long ts_3){
    ((long*)data_backing)[0]=ts_1;
    ((long*)data_backing)[1]=ts_2;
    ((long*)data_backing)[2]=ts_3;
}

NPLR::NPLR(Packet* p):PacketAccessor(p){type = PacketID::NPLR;data_length = sizeof(int);}
NPLR::NPLR(Payload p):PacketAccessor(p){}
int NPLR::GetPlayerID(){return ((int*)data_backing)[0];}
void NPLR::SetPlayerID(int player_id){((int*)data_backing)[0] = player_id;}
wchar* NPLR::GetPlayerName(){return (wchar*)(&data_backing[sizeof(int)]);}
void   NPLR::SetPlayerName(wchar* name){
    int name_len = wstr::len(name);
    if(name_len > MAX_PLAYER_NAME_LEN){name_len = MAX_PLAYER_NAME_LEN;}
    memcpy(&data_backing[sizeof(int)],name,name_len*sizeof(wchar));
    ((wchar*)(&data_backing[sizeof(int)]))[name_len]=0;
    AddDataLength(sizeof(wchar)*(name_len+1));
}

PINF::PINF(Packet* p):PacketAccessor(p){type = PacketID::PINF;data_length = sizeof(int)*2;}
PINF::PINF(Payload p):PacketAccessor(p){}
int PINF::GetPlayerID(){return ((int*)data_backing)[0];}
void PINF::SetPlayerID(int player_id){((int*)data_backing)[0] = player_id;}
int PINF::GetPlayerEntityID(){return ((int*)data_backing)[1];}
void PINF::SetPlayerEntityID(int entity_id){((int*)data_backing)[1] = entity_id;}
wchar* PINF::GetPlayerName(){return (wchar*)(&data_backing[sizeof(int)*2]);}
void   PINF::SetPlayerName(wchar* name){
    int name_len = wstr::len(name);
    int offset = sizeof(int)*2;
    if(name_len > MAX_PLAYER_NAME_LEN){ name_len = MAX_PLAYER_NAME_LEN;}
    memcpy(&data_backing[offset],name,name_len*sizeof(wchar));
    ((wchar*)(&data_backing[offset]))[name_len]=0;
    AddDataLength(sizeof(wchar)*(name_len+1));
}
wchar* PINF::GetCharacterName(){
    int name_len = wstr::len(GetPlayerName());
    int offset = sizeof(int)*2 + sizeof(wchar)*(name_len+1);
    return (wchar*)(&data_backing[offset]);}
void   PINF::SetCharacterName(wchar* character){
    int name_len = wstr::len(GetPlayerName());
    int offset = sizeof(int)*2 + sizeof(wchar)*(name_len+1);
    int character_len = wstr::len(character);
    if(character_len > MAX_PLAYER_NAME_LEN){ character_len = MAX_PLAYER_NAME_LEN;}
    memcpy(&data_backing[offset],character,character_len*sizeof(wchar));
    ((wchar*)(&data_backing[offset]))[character_len]=0;
    AddDataLength(sizeof(wchar)*(character_len+1));
}


PLDC::PLDC(Packet* p):PacketAccessor(p){type = PacketID::PLDC;data_length = sizeof(int);}
PLDC::PLDC(Payload p):PacketAccessor(p){}
int PLDC::GetPlayerID(){return ((int*)data_backing)[0];}
void PLDC::SetPlayerID(int player_id){((int*)data_backing)[0] = player_id;}
wchar* PLDC::GetPlayerName(){return (wchar*)(&data_backing[sizeof(int)]);}
void   PLDC::SetPlayerName(wchar* name){
    int name_len = wstr::len(name);
    if(name_len > MAX_PLAYER_NAME_LEN){ name_len = MAX_PLAYER_NAME_LEN;}
    memcpy(&data_backing[sizeof(int)],name,name_len*sizeof(wchar));
    ((wchar*)(&data_backing[sizeof(int)]))[name_len]=0;
    AddDataLength(sizeof(wchar)*(name_len+1));
}
wchar* PLDC::GetReason(){
    int name_len = wstr::len(GetPlayerName());
    int offset = sizeof(int)+sizeof(wchar)*(name_len+1);
    return (wchar*)(&data_backing[offset]);}
void   PLDC::SetReason(wchar* reason){
    int name_len = wstr::len(GetPlayerName());
    int offset = sizeof(int)+sizeof(wchar)*(name_len+1);
    int reason_len = wstr::len(reason);
    if(reason_len > MAX_REASON_LENGTH){ reason_len = MAX_REASON_LENGTH;}
    memcpy(&data_backing[offset],reason,reason_len*sizeof(wchar));
    ((wchar*)(&data_backing[offset]))[reason_len]=0;
    AddDataLength(sizeof(wchar)*(reason_len+1));
}


CHAT::CHAT(Packet* p):PacketAccessor(p){type = PacketID::CHAT;}
CHAT::CHAT(Payload p):PacketAccessor(p){}
int CHAT::GetPlayerID(){return ((int*)data_backing)[0];}
void CHAT::SetPlayerID(int player_id){((int*)data_backing)[0] = player_id;}
wchar* CHAT::GetChatText(){return (wchar*)(&data_backing[sizeof(int)]);}
void   CHAT::SetChatText(wchar* text){
    const int MAX_CHAT_LENGTH = (Packet::MAX_DATA_LENGTH - sizeof(wchar)) / sizeof(wchar);
    int text_len = wstr::len(text);
    if(text_len > MAX_CHAT_LENGTH){ text_len = MAX_CHAT_LENGTH;}
    memcpy(&data_backing[sizeof(int)],text,text_len*sizeof(wchar));
    ((wchar*)(&data_backing[sizeof(int)]))[text_len]=0;
    AddDataLength(sizeof(wchar)*(text_len+1));
}



SNPS::SNPS(Packet* p):PacketAccessor(p){type = PacketID::SNPS;}
SNPS::SNPS(Payload p):PacketAccessor(p){}
int SNPS::GetRaceID(){return ((int*)data_backing)[0];}
void SNPS::SetRaceID(int id){((int*)data_backing)[0] = id;}
int SNPS::GetClassID(){return ((int*)data_backing)[1];}
void SNPS::SetClassID(int id){((int*)data_backing)[1] = id;}
int SNPS::GetStyle1(){return ((int*)data_backing)[2];}
void SNPS::SetStyle1(int style1){((int*)data_backing)[2] = style1;}
int SNPS::GetStyle2(){return ((int*)data_backing)[3];}
void SNPS::SetStyle2(int style1){((int*)data_backing)[3] = style1;}
int SNPS::GetStyle3(){return ((int*)data_backing)[4];}
void SNPS::SetStyle3(int style1){((int*)data_backing)[4] = style1;}
ColorCode SNPS::GetColor1(){return ((ColorCode*)(&data_backing[sizeof(int)*5]))[0];}
void SNPS::SetColor1(ColorCode c){((ColorCode*)(&data_backing[sizeof(int)*5]))[0] = c;}
wchar* SNPS::GetCharacterName(){
    int place = (sizeof(int)*5) + sizeof(ColorCode);
    return ((wchar*)(&data_backing[place]));
}
void SNPS::SetCharacterName(wchar* name){
    int text_len = wstr::len(name);
    int place = (sizeof(int)*5) + sizeof(ColorCode);
    if(text_len > MAX_PLAYER_NAME_LEN){ text_len = MAX_PLAYER_NAME_LEN;}
    memcpy(&data_backing[place],name,text_len*sizeof(wchar));
    ((wchar*)(&data_backing[place]))[text_len]=0;
    AddDataLength(sizeof(wchar)*(text_len+1));
}

/*

Packet BuildPacket_JOIN(wchar* name){//TODO: stub
    
    Packet ret;
    ret.type = PacketID::JOIN;
    ret.ClearData();

    int name_len = wstr::len(name);
    if(name_len > MAX_NAME_LENGTH){ name_len = MAX_NAME_LENGTH;}
    memcpy(ret.data,&name_len,sizeof(int));
    name_len = (name_len + 1) * sizeof(wchar);
    memcpy(&ret.data[4],name,name_len);
    
    ret.CreateID();
    ret.SetDataLength(sizeof(int)+name_len);
    ret.RunCRC();
    return ret;    
}
Packet BuildPacket_ACPT(int packet_id,int player_id){//TODO: stub
    Packet ret; 
    ret.type = PacketID::ACPT;
    ret.ClearData();
    ret.CreateID();
    ret.SetDataLength(0);
    ret.RunCRC();
    return ret;    
}
Packet BuildPacket_OKAY(int packet_id){
    Packet ret; 
    ret.type = PacketID::ACPT;
    ret.ClearData();
    int ack_packet_id = packet_id;
    memcpy(ret.data,&ack_packet_id,sizeof(int));
    ret.CreateID();
    ret.SetDataLength(sizeof(int));
    ret.RunCRC();
    return ret;    
}
Packet BuildPacket_NOPE(wchar* reason){
    const int MAX_REASON_LENGTH = (Packet::MAX_DATA_LENGTH - (sizeof(int) + sizeof(wchar))) / sizeof(wchar);
    Packet ret;
    ret.type = PacketID::NOPE;
    ret.ClearData();

    int reason_len = wstr::len(reason);
    if(reason_len > MAX_REASON_LENGTH){ reason_len = MAX_REASON_LENGTH;}
    memcpy(ret.data,&reason_len,sizeof(int));
    reason_len = (reason_len + 1) * sizeof(wchar);
    memcpy(&ret.data[4],reason,reason_len);

    ret.CreateID();
    ret.SetDataLength(sizeof(int)+reason_len);
    ret.RunCRC();
    return ret;    
}
Packet BuildPacket_SPRQ(){//TODO: stub
    Packet ret; 
    ret.type = PacketID::ACPT;
    ret.ClearData();
    ret.CreateID();
    ret.SetDataLength(0);
    ret.RunCRC();
    return ret;    
}
Packet BuildPacket_PLYR(){//TODO: stub
    Packet ret; 
    ret.type = PacketID::ACPT;
    ret.ClearData();
    ret.CreateID();
    ret.SetDataLength(0);
    ret.RunCRC();
    return ret;    
}
Packet BuildPacket_PLDC(){//TODO: stub
    Packet ret; 
    ret.type = PacketID::ACPT;
    ret.ClearData();
    ret.CreateID();
    ret.SetDataLength(0);
    ret.RunCRC();
    return ret;    
}
Packet BuildPacket_PING(long timestamp1,long timestamp2,long timestamp3){//TODO: stub
    Packet ret; 
    ret.type = PacketID::ACPT;
    ret.ClearData();
    ((long*)ret.data)[0] = timestamp1;
    ((long*)ret.data)[1] = timestamp1;
    ((long*)ret.data)[2] = timestamp1;
    ret.CreateID();
    ret.SetDataLength(sizeof(long)*3);
    ret.RunCRC();
    return ret;    
}
Packet BuildPacket_SPWN(){//TODO: stub
    Packet ret; 
    ret.type = PacketID::ACPT;
    ret.ClearData();
    ret.CreateID();
    ret.SetDataLength(0);
    ret.RunCRC();
    return ret;    
}
Packet BuildPacket_DSPN(){//TODO: stub
    Packet ret; 
    ret.type = PacketID::ACPT;
    ret.ClearData();
    ret.CreateID();
    ret.SetDataLength(0);
    ret.RunCRC();
    return ret;    
}
Packet BuildPacket_DLTA(){//TODO: stub
    Packet ret; 
    ret.type = PacketID::ACPT;
    ret.ClearData();
    ret.CreateID();
    ret.SetDataLength(0);
    ret.RunCRC();
    return ret;    
}
Packet BuildPacket_CDLT(){//TODO: stub
    Packet ret; 
    ret.type = PacketID::ACPT;
    ret.ClearData();
    ret.CreateID();
    ret.SetDataLength(0);
    ret.RunCRC();
    return ret;    
}
Packet BuildPacket_CHAT(){//TODO: stub
    Packet ret; 
    ret.type = PacketID::ACPT;
    ret.ClearData();
    ret.CreateID();
    ret.SetDataLength(0);
    ret.RunCRC();
    return ret;    
}


wchar* ReadPacket_JOIN_name(Packet* JOIN){
    return (wchar*)&JOIN->data[4];
}
wchar* ReadPacket_NOPE_reason(Packet* NOPE){
    return (wchar*)&NOPE->data[4];
}
int ReadPacket_OKAY_ackID(Packet* OKAY){
    return (*(int*)OKAY->data);
}
int ReadPacket_ACPT_ackID(Packet* OKAY){
    return (*(int*)OKAY->data);
}
int ReadPacket_ACPT_playerID(Packet* OKAY){
    return ((int*)OKAY->data)[1];
}
int ReadPacket_PING_timestamp(Packet* OKAY,int timestamp_num){
    if(timestamp_num > 2 || timestamp_num < 0)return 0;
    return ((long*)OKAY->data)[timestamp_num];
}

*/