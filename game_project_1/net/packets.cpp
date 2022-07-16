#include <game_project_1/net/packets.hpp>
#include <game_project_1/os.hpp>

using namespace Packet;


OKAY::OKAY(){ack_id=0;}
OKAY::OKAY(Payload p){memcpy(this,p.data,sizeof(OKAY));}
Payload OKAY::GetPayload(){return Payload(PacketID::OKAY,sizeof(OKAY),(byte*)this);}
    
JOIN::JOIN(){
    player_save_id=0;
    memset(persona_buffer,0,sizeof(wchar)*(PERSONA_MAX+1));
}
JOIN::JOIN(Payload p){memcpy(this,p.data,p.length);}
void JOIN::SetPersona(wchar* persona){SetWStringWithMax(persona_buffer,persona,PERSONA_MAX);}
Payload JOIN::GetPayload(){return Payload(PacketID::JOIN,sizeof(int)+(wstr::len(persona_buffer)+1)*sizeof(wchar),(byte*)this);}

ACPT::ACPT(){
    ack_id=0;
    player_save_id=0;player_slot_id=0;
    player_count=0; player_max=0;
}
ACPT::ACPT(Payload p){memcpy(this,p.data,sizeof(ACPT));}
Payload ACPT::GetPayload(){return Payload(PacketID::ACPT,sizeof(ACPT),(byte*)this);}

PLYR::PLYR(){
    player_id=-1;
    memset(persona_buffer,0,sizeof(wchar)*(PERSONA_MAX+1));}
PLYR::PLYR(Payload p){memcpy(this,p.data,p.length);}
void PLYR::SetPersona(wchar* persona){SetWStringWithMax(persona_buffer,persona,PERSONA_MAX);}
Payload PLYR::GetPayload(){return Payload(PacketID::PLYR,sizeof(int)+(wstr::len(persona_buffer)+1)*sizeof(wchar),(byte*)this);}

NOPE::NOPE(wchar* reason){SetWStringWithMax(reason_buffer,reason,DC_REASON_MAX);}
NOPE::NOPE(Payload p){memcpy(this,p.data,p.length);}
Payload NOPE::GetPayload(){
    return Payload(PacketID::NOPE,(wstr::len(reason_buffer)+1)*sizeof(wchar),(byte*)this);
}

PING::PING(){ts_1=ts_2=ts_3=0;}
PING::PING(Payload p){memcpy(this,p.data,sizeof(PING));}
Payload PING::GetPayload(){return Payload(PacketID::PING,sizeof(PING),(byte*)this);}

PINF::PINF(){
    player_id=0; player_entity=0; player_area_id=0;
    memset(character_name_buffer,0,sizeof(wchar)*(PLAYERNAME_MAX+1));
}
PINF::PINF(Payload p){memcpy(this,p.data,p.length);}
Payload PINF::GetPayload(){
    return Payload(PacketID::PINF,
    sizeof(int)*3 + sizeof(wchar)*(wstr::len(character_name_buffer)+1),
    (byte*)this);
}
void PINF::SetCharacterName(wchar* charactername){
    SetWStringWithMax(character_name_buffer,charactername,PLAYERNAME_MAX);
}

PLDC::PLDC(){
    player_id=0;
    memset(persona_reason_buffer,0,sizeof(wchar)*(PERSONA_MAX + DC_REASON_MAX + 2));
}
PLDC::PLDC(Payload p){memcpy(this,p.data,p.length);}
void PLDC::SetPlayerAndReason(wchar* player_name, wchar* reason){
    wchar* reason_start = SetWStringWithMax(persona_reason_buffer,player_name,PERSONA_MAX);
    SetWStringWithMax(reason_start,reason,DC_REASON_MAX);
}
wchar* PLDC::GetPlayerName(){return persona_reason_buffer;}
wchar* PLDC::GetReason(){return &persona_reason_buffer[wstr::len(persona_reason_buffer)+1];}
Payload PLDC::GetPayload(){
    return Payload(PacketID::PLDC, sizeof(int) + GetNthStringLength(persona_reason_buffer,2), (byte*)this);
}

CHAT::CHAT(){
    player_id=to_id=0;
    memset(chat_msg,0,sizeof(wchar)*(CHAT_MAX+1));
}
CHAT::CHAT(Payload p){memcpy(this,p.data,p.length);}
void CHAT::SetChatMessage(wchar* msg){SetWStringWithMax(chat_msg,msg,CHAT_MAX);}
Payload CHAT::GetPayload(){
    return Payload(PacketID::CHAT, sizeof(int)*2+ sizeof(wchar)*(wstr::len(chat_msg)+1), (byte*)this); 
}

SNPS::SNPS(){
    save_id=race_id=class_id=style_1=style_2=style_3=0;
    color_1.from_int(0);color_2.from_int(0);color_3.from_int(0);
    memset(player_name,0,sizeof(wchar)*(PLAYERNAME_MAX+1));
}
SNPS::SNPS(Payload p){memcpy(this,p.data,p.length);}
void SNPS::SetPlayerName(wchar* name){SetWStringWithMax(player_name,name,PLAYERNAME_MAX);}
Payload SNPS::GetPayload(){
    return Payload(PacketID::SNPS, sizeof(int)*6+ sizeof(color)*3+ sizeof(wchar)*(wstr::len(player_name)+1), (byte*)this); 
}

INFO::INFO(){reserved=0;}
INFO::INFO(Payload p){memcpy(this,p.data,sizeof(INFO));}
Payload INFO::GetPayload(){return Payload(PacketID::INFO,sizeof(INFO),(byte*)this);}

SINF::SINF(){
    players=max_players=0;
    memset(name_and_description_buffer,0,sizeof(wchar)*(SERVERNAME_MAX + SERVER_DESC_MAX + 2));
}
SINF::SINF(Payload p){memcpy(this,p.data,p.length);}
void SINF::SetNameAndDescription(wchar* name,wchar* description){
    wchar* desc_start = SetWStringWithMax(name_and_description_buffer,name,SERVERNAME_MAX);
    SetWStringWithMax(desc_start,description,SERVER_DESC_MAX);
}
wchar* SINF::GetName(){return name_and_description_buffer;}
wchar* SINF::GetDescription(){return &name_and_description_buffer[wstr::len(name_and_description_buffer)+1];}
Payload SINF::GetPayload(){
    return Payload(PacketID::PLDC, sizeof(int)*2 + GetNthStringLength(name_and_description_buffer,2), (byte*)this);
}


wchar* Packet::SetWStringWithMax(wchar* dest, wchar* source, int max){
    int strlen = wstr::len(source);
    if(strlen > DC_REASON_MAX)strlen = DC_REASON_MAX;
    memcpy(dest,source,strlen*sizeof(wchar));
    dest[strlen]=0;
    return &dest[strlen+1];
}

int Packet::GetNthStringLength(wchar* str,int strings){//Length in bytes of wchar buffer containing (strings) null-terminated strings (including terminating 0s)
    int place=0;
    for(int i=0;i<strings;i++){place+=wstr::len(&str[place])+1;}
    return place*sizeof(wchar);
}
