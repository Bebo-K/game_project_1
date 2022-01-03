#ifndef PACKET_BUILDER_H
#define PACKET_BUILDER_H

#include "packet.h"
#include "../struct/str.h"

namespace PacketData{

    struct PacketAccessor{
        int     type;
        int     data_length;
        byte*   data_backing;
        Packet* raw_packet_backing;
        PacketAccessor(Packet* p);
        PacketAccessor(Payload p);
        void AddDataLength(int len);
        void WritePacket();
    };

    struct JOIN: PacketAccessor{
        wchar* GetPlayerName();
        void   SetPlayerName(wchar* name);
        JOIN(Packet* p);
    };
    struct OKAY: PacketAccessor{
        int GetAckID();
        void SetAckID(int ack);
        OKAY(Packet* p);
    };
    struct ACPT: PacketAccessor{
        int GetAckID();
        void SetAckID(int ack);
        int GetPlayerID();
        void SetPlayerID(int player_id);
        int GetPlayerCount();
        void SetPlayerCount(int player_count);
        int GetPlayerMax();
        void SetPlayerMax(int player_max);
        ACPT(Packet* p);
        ACPT(Payload p);
    };
    struct NOPE: PacketAccessor{
        wchar* GetReason();
        void SetReason(wchar* reason);
        NOPE(Packet* p);
    };
    struct PING: PacketAccessor{
        long GetTimestamp1();
        long GetTimestamp2();
        long GetTimestamp3();
        void SetTimestamps(long ts_1,long ts_2,long ts_3);
        PING(Packet* p);
    };
    struct PLYR: PacketAccessor{
        int GetPlayerID();
        void SetPlayerID(int id);
        wchar* GetPlayerName();
        void SetPlayerName(wchar* name);
        PLYR(Packet* p);
        PLYR(Payload p);
    };
    struct PINF: PacketAccessor{
        int GetPlayerID();
        void SetPlayerID(int id);
        int GetPlayerEntityID();
        void SetPlayerEntityID(int eid);
        wchar* GetPlayerName();
        void SetPlayerName(wchar* name);
        wchar* GetCharacterName();
        void SetCharacterName(wchar* renameason);
        PINF(Packet* p);
        PINF(Payload p);
    };
    struct PLDC: PacketAccessor{
        int GetPlayerID();
        void SetPlayerID(int id);
        wchar* GetPlayerName();
        void SetPlayerName(wchar* name);
        wchar* GetReason();
        void SetReason(wchar* reason);
        PLDC(Packet* p);
        PLDC(Payload p);
    };
    struct CHAT: PacketAccessor{
        int GetPlayerID();
        void SetPlayerID(int id);
        wchar* GetChatText();
        void SetChatText(wchar* name);
        CHAT(Packet* p);
        CHAT(Payload p);
    };
};


#endif