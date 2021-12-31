#ifndef PACKET_BUILDER_H
#define PACKET_BUILDER_H

#include "packet.h"
#include "../struct/str.h"




Packet BuildPacket_JOIN(wchar* name);
Packet BuildPacket_ACPT(int packet_id,int player_id);
Packet BuildPacket_OKAY(int packet_id);
Packet BuildPacket_NOPE(wchar* reason);
Packet BuildPacket_SPRQ();
Packet BuildPacket_PLYR();
Packet BuildPacket_PLDC();
Packet BuildPacket_PING(long timestamp1,long timestamp2,long timestamp3);
Packet BuildPacket_SPWN();
Packet BuildPacket_DSPN();
Packet BuildPacket_DLTA();
Packet BuildPacket_CDLT();
Packet BuildPacket_CHAT();

wchar* ReadPacket_JOIN_name(Packet* JOIN);
wchar* ReadPacket_NOPE_reason(Packet* NOPE);
int ReadPacket_ACPT_ackID(Packet* OKAY);
int ReadPacket_ACPT_playerID(Packet* OKAY);
int ReadPacket_OKAY_ackID(Packet* OKAY);
int ReadPacket_PING_timestamp(Packet* OKAY,int timestamp_num);

#endif