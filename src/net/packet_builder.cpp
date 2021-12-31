#include "packet_builder.h"
#include "../os.h"

Packet BuildPacket_JOIN(wchar* name){//TODO: stub
    const int MAX_NAME_LENGTH = (Packet::MAX_DATA_LENGTH - (sizeof(int) + sizeof(wchar))) / sizeof(wchar);
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
Packet BuildPacket_PING(){//TODO: stub
    Packet ret; 
    ret.type = PacketID::ACPT;
    ret.ClearData();
    long timestamp = time_ms();
    memcpy(ret.data,&timestamp,sizeof(long));
    ret.CreateID();
    ret.SetDataLength(sizeof(int));
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