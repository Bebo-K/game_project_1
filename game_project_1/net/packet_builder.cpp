#include <game_project_1/net/packet_builder.hpp>
#include <game_project_1/os.hpp>

using namespace PacketData;

const int MAX_PLAYER_NAME_LEN=64;
const int MAX_REASON_LENGTH = 180;


PacketAccessor::PacketAccessor(Packet* p){
    type=0;
    data_backing = p->data;
    raw_packet_backing = p;
}
PacketAccessor::PacketAccessor(Payload p){
    type=p.type;
    data_backing = p.data;
    raw_packet_backing = nullptr;
}

int PacketAccessor::Byte(int count){return sizeof(byte)*count;}
int PacketAccessor::Int(int count){return sizeof(int)*count;}
int PacketAccessor::Wstr(int place){return place + sizeof(wchar)*(wstr::len((wchar*)&data_backing[place])+1);}

int PacketAccessor::GetInt(int place){return *((int*)&data_backing[place]);}
wchar* PacketAccessor::GetWstr(int place){return ((wchar*)&data_backing[place]);}

void PacketAccessor::SetInt(int place,int val){*((int*)&data_backing[place]) = val;}
int PacketAccessor::SetWstr(wchar* str,int place,int max){
    int str_len = wstr::len(str);
    if(str_len > max){str_len = MAX_PLAYER_NAME_LEN;}
    memcpy(&data_backing[place],str,str_len*sizeof(wchar));
    ((wchar*)&data_backing[place])[str_len]=0;
    return place + sizeof(wchar)*(str_len+1);
}

void PacketAccessor::WritePacket(){
    if(raw_packet_backing == nullptr){logger::exception("Unsupported operation: PacketAccessor is read-only for payloads");}
    raw_packet_backing->type = type;
    raw_packet_backing->CreateID();
    raw_packet_backing->SetDataLength(Size());
}


JOIN::JOIN(Packet* p):PacketAccessor(p){type = PacketID::JOIN;}
wchar* JOIN::GetPlayerName(){return GetWstr(0);}
void   JOIN::SetPlayerName(wchar* name){SetWstr(name,0,MAX_PLAYER_NAME_LEN);}
int JOIN::Size(){return Wstr(0);}


OKAY::OKAY(Packet* p):PacketAccessor(p){type = PacketID::OKAY;}
int OKAY::GetAckID(){return GetInt(0);}
void OKAY::SetAckID(int ack){SetInt(0,ack);}
int OKAY::Size(){return Int(0);}

ACPT::ACPT(Packet* p):PacketAccessor(p){type = PacketID::ACPT;}
ACPT::ACPT(Payload p):PacketAccessor(p){}
int ACPT::GetAckID(){return GetInt(0);}
void ACPT::SetAckID(int ack){SetInt(0,ack);}
int ACPT::GetPlayerID(){return GetInt(Int(1));}
void ACPT::SetPlayerID(int player_id){SetInt(Int(1),player_id);}
int ACPT::GetPlayerSaveID(){return GetInt(Int(2));}
void ACPT::SetPlayerSaveID(int player_save_id){SetInt(Int(2),player_save_id);}
int ACPT::GetPlayerCount(){return GetInt(Int(3));}
void ACPT::SetPlayerCount(int player_count){SetInt(Int(3),player_count);}
int ACPT::GetPlayerMax(){return GetInt(Int(4));}
void ACPT::SetPlayerMax(int player_max){SetInt(Int(4),player_max);}
int ACPT::Size(){return Int(5);}

NOPE::NOPE(Packet* p):PacketAccessor(p){type = PacketID::NOPE;}
wchar* NOPE::GetReason(){return GetWstr(0);}
void NOPE::SetReason(wchar* reason){SetWstr(reason,0,MAX_REASON_LENGTH);}
int NOPE::Size(){return Wstr(0);}

PING::PING(Packet* p):PacketAccessor(p){type = PacketID::PING;}
long PING::GetTimestamp1(){return ((long*)data_backing)[0];}
long PING::GetTimestamp2(){return ((long*)data_backing)[1];}
long PING::GetTimestamp3(){return ((long*)data_backing)[2];}
void PING::SetTimestamps(long ts_1,long ts_2,long ts_3){
    ((long*)data_backing)[0]=ts_1;
    ((long*)data_backing)[1]=ts_2;
    ((long*)data_backing)[2]=ts_3;
}
int PING::Size(){return sizeof(long)*3;}

NPLR::NPLR(Packet* p):PacketAccessor(p){type = PacketID::NPLR;}
NPLR::NPLR(Payload p):PacketAccessor(p){}
int NPLR::GetPlayerID(){return GetInt(0);}
void NPLR::SetPlayerID(int player_id){SetInt(0,player_id);}
wchar* NPLR::GetPlayerName(){return GetWstr(Int(1));}
void   NPLR::SetPlayerName(wchar* name){SetWstr(name,Int(1),MAX_PLAYER_NAME_LEN);}
int NPLR::Size(){return Wstr(Int(1));}

PINF::PINF(Packet* p):PacketAccessor(p){type = PacketID::PINF;}
PINF::PINF(Payload p):PacketAccessor(p){}
int PINF::GetPlayerID(){return GetInt(0);}
void PINF::SetPlayerID(int player_id){SetInt(0,player_id);}
wchar* PINF::GetPlayerName(){return GetWstr(Int(1));}
void   PINF::SetPlayerName(wchar* name){SetWstr(name,Int(1),MAX_PLAYER_NAME_LEN);}
wchar* PINF::GetCharacterName(){return GetWstr(Wstr(Int(1)));}
void   PINF::SetCharacterName(wchar* character){SetWstr(character,Wstr(Int(1)),MAX_PLAYER_NAME_LEN);}

int PINF::GetPlayerEntityID(){return GetInt(Wstr(Wstr(Int(1))));}
void PINF::SetPlayerEntityID(int entity_id){SetInt(Wstr(Wstr(Int(1))),entity_id);}

int PINF::GetPlayerEntityArea(){return GetInt(Wstr(Wstr(Int(1)))+Int(1));}
void PINF::SetPlayerEntityArea(int entity_id){SetInt(Wstr(Wstr(Int(1)))+Int(1),entity_id);}
int PINF::Size(){return Wstr(Wstr(Int(1)))+Int(2);}

PLDC::PLDC(Packet* p):PacketAccessor(p){type = PacketID::PLDC;}
PLDC::PLDC(Payload p):PacketAccessor(p){}
int PLDC::GetPlayerID(){return GetInt(0);}
void PLDC::SetPlayerID(int player_id){SetInt(0,player_id);}
wchar* PLDC::GetPlayerName(){return GetWstr(Int(1));}
void   PLDC::SetPlayerName(wchar* name){SetWstr(name,Int(1),MAX_PLAYER_NAME_LEN);}
wchar* PLDC::GetReason(){return GetWstr(Wstr(Int(1)));}
void   PLDC::SetReason(wchar* reason){SetWstr(reason,Wstr(Int(1)),MAX_REASON_LENGTH);}
int PLDC::Size(){return Wstr(Wstr(Int(1)));}

CHAT::CHAT(Packet* p):PacketAccessor(p){type = PacketID::CHAT;}
CHAT::CHAT(Payload p):PacketAccessor(p){}
int CHAT::GetPlayerID(){return GetInt(0);}
void CHAT::SetPlayerID(int player_id){SetInt(0,player_id);}
wchar* CHAT::GetChatText(){return GetWstr(Int(1));}
void   CHAT::SetChatText(wchar* text){
    const int MAX_CHAT_LENGTH = (Packet::MAX_DATA_LENGTH - sizeof(wchar)) / sizeof(wchar);
    SetWstr(text,Int(1),MAX_CHAT_LENGTH);
}
int CHAT::Size(){return Wstr(Int(1));}

SNPS::SNPS(Packet* p):PacketAccessor(p){type = PacketID::SNPS;}
SNPS::SNPS(Payload p):PacketAccessor(p){}
wchar* SNPS::GetCharacterName(){return GetWstr(0);}
void SNPS::SetCharacterName(wchar* name){SetWstr(name,0,MAX_PLAYER_NAME_LEN);}
int SNPS::GetRaceID(){return GetInt(Wstr(0));}
void SNPS::SetRaceID(int id){SetInt(Wstr(0),id);}
int SNPS::GetClassID(){return GetInt(Wstr(0)+Int(1));}
void SNPS::SetClassID(int id){SetInt(Wstr(0)+Int(1),id);}
CharacterAppearance* SNPS::GetCharacterAppearance(){return (CharacterAppearance*)&data_backing[Wstr(0)+Int(2)];}
void SNPS::SetStyle1(int style1){SetInt(Wstr(0)+Int(2),style1);}
void SNPS::SetStyle2(int style2){SetInt(Wstr(0)+Int(3),style2);}
void SNPS::SetStyle3(int style3){SetInt(Wstr(0)+Int(4),style3);}
void SNPS::SetColor1(color color1){SetInt(Wstr(0)+Int(5),color1.to_int());}
int SNPS::Size(){return Wstr(0)+Int(6);}

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