#ifndef PACKETS_H
#define PACKETS_H

#include <game_project_1/net/payload.hpp>
#include <game_project_1/types/str.hpp>
#include <game_project_1/game/savefile.hpp>
#include <game_project_1/component/logic_components.hpp>

//Packet structures are wrappers for a payload's data buffer
namespace Packet{

    const int SERVERNAME_MAX = 64;
    const int SERVER_DESC_MAX = 128;
    const int PERSONA_MAX = 64;
    const int PLAYERNAME_MAX = 32;
    const int DC_REASON_MAX = 128;
    const int CHAT_MAX = 128;

    struct OKAY{//Reliable packet
        int ack_id;
        OKAY();
        OKAY(Payload p);
        Payload GetPayload();
    };

    struct JOIN{//Player join request
        int player_save_id;//Existing save ID, or 0 if new save.
        wchar persona_buffer[PERSONA_MAX + 1];//Not player name, just to ID who is joining.
        JOIN();
        JOIN(Payload p);
        void SetPersona(wchar* persona);
        Payload GetPayload();
    };

    struct ACPT{//Server join response
        int ack_id;
        int player_save_id;//(Private id) server generated unique ID to persist player save under
        int player_slot_id;//(Public id) server player slot index 
        int player_count;//Number of player slots filled
        int player_max;//Max player slots the server supports
        ACPT();
        ACPT(Payload p);
        Payload GetPayload();
    };

    struct PLYR{//Server join broadcast to other players
        int player_id;
        wchar persona_buffer[PERSONA_MAX + 1];

        PLYR();
        PLYR(Payload p);
        void SetPersona(wchar* persona);
        Payload GetPayload();
    };

    struct NOPE{//Client or server disconnect
        wchar reason_buffer[DC_REASON_MAX + 1];

        NOPE(wchar* reason);
        NOPE(Payload p);
        Payload GetPayload();
    };

    struct PING{//Latency test
        long ts_1,ts_2,ts_3;
        
        PING();
        PING(Payload p);
        Payload GetPayload();
    };

    struct PINF{//Server player info notification
        int player_id;
        int player_entity;
        int player_area_id;
        wchar persona_buffer[PERSONA_MAX + 1];

        PINF();
        PINF(Payload p);
        Payload GetPayload();
        void SetPersona(wchar* persona);
    };

    struct PLDC{//Server player disconnect notification
        int player_id;//Can be -1 if client never joined
        wchar persona_reason_buffer[PERSONA_MAX + 1 + DC_REASON_MAX + 1];

        PLDC();
        PLDC(Payload p);
        void SetPlayerAndReason(wchar* persona, wchar* reason);
        wchar* GetPlayerName();
        wchar* GetReason();
        Payload GetPayload();
    };

    struct CHAT{//Player chat (Sent by client to server, then server to other client(s))
        int player_id;
        int to_id;//positives are player ids, negatives are group ids, 0 is all.
        wchar chat_msg[CHAT_MAX+1];

        CHAT();
        CHAT(Payload p);
        void SetChatMessage(wchar* msg);
        Payload GetPayload();
    };

    struct SNPS{//Start new player save- Client player creation request
        int race_id;
        int class_id;
        int style_1,style_2,style_3;
        color color_1,color_2,color_3;
        wchar player_name[PLAYERNAME_MAX+1];

        SNPS();
        SNPS(Payload p);
        void SetPlayerName(wchar* name);
        Payload GetPayload();
    };
    
    struct INFO{//Server info request
        int reserved;
        INFO();
        INFO(Payload p);
        Payload GetPayload();
    };

    struct SINF{//Server info response
        int players;
        int max_players;
        wchar name_and_description_buffer[SERVERNAME_MAX + 1 + SERVER_DESC_MAX + 1];

        SINF();
        SINF(Payload p);
        void SetNameAndDescription(wchar* name,wchar* description);
        wchar* GetName();
        wchar* GetDescription();
        Payload GetPayload();
    };

    /*
    These can all be multi-packet
    const int SCNE = CSTR_TO_PACKETID("SCNE");//R  Scene Info (Server prompt for client to load scene)
    const int SPWN = CSTR_TO_PACKETID("SPWN");//R  Server spawned entity
    const int DSPN = CSTR_TO_PACKETID("DSPN");//R  Server despawned entity
    const int DLTA = CSTR_TO_PACKETID("DLTA");//   Server Entity delta
    const int CDLT = CSTR_TO_PACKETID("CDLT");//   Client state delta
    */

    //Utility methods
    wchar* SetWStringWithMax(wchar* dest, wchar* source, int max);
    int GetNthStringLength(wchar* str,int strings);

};


#endif