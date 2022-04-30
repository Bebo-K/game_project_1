#ifndef PACKET_BUILDER_H
#define PACKET_BUILDER_H

#include <game_project_1/net/packet.hpp>
#include <game_project_1/types/str.hpp>
#include <game_project_1/game/savefile.hpp>
#include <game_project_1/component/logic_components.hpp>

namespace PacketData{

    struct PacketAccessor{
        int     type;
        //int     data_length;
        byte*   data_backing;
        Packet* raw_packet_backing;
        PacketAccessor(Packet* p);
        PacketAccessor(Payload p);

        virtual int Size() = 0;

        int Byte(int count);//returns sizeof(byte)*count;
        int Int(int count);//returns sizeof(int)*count;
        int Wstr(int place);//returns place after wstr starting at place (0 terminated)

        int GetInt(int place);
        wchar* GetWstr(int place);

        void SetInt(int place,int val);
        int SetWstr(wchar* str,int place,int max);//returns new place
        
        void WritePacket();
    };

    struct JOIN: PacketAccessor{
        wchar* GetPlayerName();
        void   SetPlayerName(wchar* name);
        //int  GetPlayerSaveID();
        //void SetPlayerSaveID(int save_id);
        JOIN(Packet* p);
        int Size();
    };
    struct OKAY: PacketAccessor{
        int GetAckID();
        void SetAckID(int ack);
        OKAY(Packet* p);
        int Size();
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
        int Size();
    };
    struct NOPE: PacketAccessor{
        wchar* GetReason();
        void SetReason(wchar* reason);
        NOPE(Packet* p);
        int Size();
    };
    struct PING: PacketAccessor{
        long GetTimestamp1();
        long GetTimestamp2();
        long GetTimestamp3();
        void SetTimestamps(long ts_1,long ts_2,long ts_3);
        PING(Packet* p);
        int Size();
    };
    struct NPLR: PacketAccessor{
        int GetPlayerID();
        void SetPlayerID(int id);
        wchar* GetPlayerName();
        void SetPlayerName(wchar* name);
        NPLR(Packet* p);
        NPLR(Payload p);
        int Size();
    };
    struct PINF: PacketAccessor{
        int GetPlayerID();
        void SetPlayerID(int id);
        wchar* GetPlayerName();
        void SetPlayerName(wchar* name);
        wchar* GetCharacterName();
        void SetCharacterName(wchar* charname);
        int GetPlayerEntityID();
        void SetPlayerEntityID(int eid);
        int GetPlayerEntityArea();
        void SetPlayerEntityArea(int area_id);
        PINF(Packet* p);
        PINF(Payload p);
        int Size();
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
        int Size();
    };
    struct CHAT: PacketAccessor{
        int GetPlayerID();
        void SetPlayerID(int id);
        wchar* GetChatText();
        void SetChatText(wchar* name);
        CHAT(Packet* p);
        CHAT(Payload p);
        int Size();
    };

    struct SNPS: PacketAccessor{
        wchar* GetCharacterName();
        void SetCharacterName(wchar* name);
        int GetRaceID();
        void SetRaceID(int id);
        int GetClassID();
        void SetClassID(int id);

        void SetStyle1(int style1);
        void SetStyle2(int style1);
        void SetStyle3(int style1);
        void SetColor1(color c);
        CharacterAppearance* GetCharacterAppearance();

        SNPS(Packet* p);
        SNPS(Payload p);
        int Size();
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