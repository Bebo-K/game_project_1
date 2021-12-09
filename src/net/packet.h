#ifndef PACKET_H
#define PACKET_H

#include "../struct/data_types.h"
const int MAX_UDP_PACKET_SIZE=508;

int PacketIDFromCSTR(const char* id);

#define CSTR_TO_PACKETID(str) (int)str[0]<<24 | (int)str[1]<<16 | (int)str[2]<<8 | (int)str[3]

namespace PacketID{
    const int JOIN = CSTR_TO_PACKETID("JOIN");//   Client join request
    const int ACPT = CSTR_TO_PACKETID("ACPT");//R  Server accepts new client connection
    const int OKAY = CSTR_TO_PACKETID("OKAY");//   (Both sides) Acknowledge reliable packet 
    const int SPRQ = CSTR_TO_PACKETID("SPRQ");//R  Client spawn request
    const int PLYR = CSTR_TO_PACKETID("PLYR");//R  Server notification of a new player
    const int PLDC = CSTR_TO_PACKETID("PLDC");//R  Server notification of a player disconnecting
    const int PING = CSTR_TO_PACKETID("PING");//   Latency loopback
    const int NOPE = CSTR_TO_PACKETID("NOPE");//   Disconnect (Either side)
    const int SPWN = CSTR_TO_PACKETID("SPWN");//R  Server spawned entity
    const int DSPN = CSTR_TO_PACKETID("DSPN");//R  Server despawned entity
    const int DLTA = CSTR_TO_PACKETID("DLTA");//   Server Entity delta
    const int DLTA = CSTR_TO_PACKETID("CDLT");//   Client state delta
    const int CHAT = CSTR_TO_PACKETID("CHAT");//R  Client text chat
};


//Base packet object for UDP datagrams
struct Packet{
    int id;//unique random ID
    int type;//from PacketID list
    int length;//includes these header items
    int crc;
    byte data[MAX_UDP_PACKET_SIZE-16];
    void runCRC();
    bool isReliable();//if true, return an OKAY acknowledgement packet once recieved.
    bool isMultipart();//if true, this obect can be cast to a MultipartPacket
};

//Complete data object recieved from network.
struct Payload{
    int type;//from PacketID list
    int length;//includes these header items
    byte* data;

    Payload(int type,int size,byte* source);
    //Payload(int type,int size,byte* source,byte* backing); TODO: we need something so we don't need to use 'new' every recvd packet
    ~Payload();
};

//Extended packet objects for multipart data.
struct MultipartPacket{
    int id;
    int type;
    int length;//of the entire payload. Packet length
    int crc;
    short segment;
    short segment_count;
    int   segment_length;
    int   segment_offset;
    byte data[MAX_UDP_PACKET_SIZE-24];
    void runCRC();
};

//Inbound assembly object for multipart data
class MultipartPayload{
    int id;
    BitArray packets_recieved;
    byte* assembled_payload;

    void Start(MultipartPacket*p);
    void Add(MultipartPacket* p);
    bool isFullyAssembled();
};



#endif