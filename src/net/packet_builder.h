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
        int GetPlayerSaveID();
        void SetPlayerSaveID(int save_id);
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
    struct NPLR: PacketAccessor{
        int GetPlayerID();
        void SetPlayerID(int id);
        wchar* GetPlayerName();
        void SetPlayerName(wchar* name);
        NPLR(Packet* p);
        NPLR(Payload p);
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

    struct ColorCode{byte r,g,b,a;};

    struct SNPS: PacketAccessor{
        int GetRaceID();
        void SetRaceID(int id);
        int GetClassID();
        void SetClassID(int id);
        int GetStyle1();
        void SetStyle1(int style1);
        int GetStyle2();
        void SetStyle2(int style1);
        int GetStyle3();
        void SetStyle3(int style1);
        ColorCode GetColor1();
        void SetColor1(ColorCode c);
        wchar* GetCharacterName();
        void SetCharacterName(wchar* name);
        SNPS(Packet* p);
        SNPS(Payload p);
    };

    //For now CONT has no data. It's just a signal to the server.

    /*

    These can all be multi-packet

    const int SCNE = CSTR_TO_PACKETID("SCNE");//R  Scene Info (Server prompt for client to load scene)
    const int SPWN = CSTR_TO_PACKETID("SPWN");//R  Server spawned entity
    const int DSPN = CSTR_TO_PACKETID("DSPN");//R  Server despawned entity
    const int DLTA = CSTR_TO_PACKETID("DLTA");//   Server Entity delta
    const int CDLT = CSTR_TO_PACKETID("CDLT");//   Client state delta
    */

};


#endif