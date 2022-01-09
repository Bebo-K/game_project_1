#ifndef PACKET_H
#define PACKET_H

#include "../struct/arrays.h"

#define CSTR_TO_PACKETID(str) (int)str[0]<<24 | (int)str[1]<<16 | (int)str[2]<<8 | (int)str[3]
const int MAX_UDP_PACKET_SIZE=508;
struct Payload;

namespace PacketID{
    const int JOIN = CSTR_TO_PACKETID("JOIN");//   Client join request
    const int ACPT = CSTR_TO_PACKETID("ACPT");//R  Server accepts new client connection
    const int OKAY = CSTR_TO_PACKETID("OKAY");//   (Both sides) Acknowledge reliable packet 
    const int NOPE = CSTR_TO_PACKETID("NOPE");//   Disconnect (Either side)
    const int PINF = CSTR_TO_PACKETID("PINF");//R  Player Info-- Sent to new players to catch them up
    const int NPLR = CSTR_TO_PACKETID("NPLR");//R  New Player-- Server notification of a new player
    const int PLDC = CSTR_TO_PACKETID("PLDC");//R  Player Disconnect-- Server notification of a player disconnecting
    const int PING = CSTR_TO_PACKETID("PING");//   Ping loopback-- finds latency and keeps connection fresh
    const int SNPS = CSTR_TO_PACKETID("SNPS");//R  Start New Player Save 
    const int CONT = CSTR_TO_PACKETID("CONT");//R  Continue Player Save
    const int SCNE = CSTR_TO_PACKETID("SCNE");//R  Scene Info (Server prompt for client to load scene)
    const int SPWN = CSTR_TO_PACKETID("SPWN");//R  Server spawned entity
    const int DSPN = CSTR_TO_PACKETID("DSPN");//R  Server despawned entity
    const int DLTA = CSTR_TO_PACKETID("DLTA");//   Server Entity delta
    const int CDLT = CSTR_TO_PACKETID("CDLT");//   Client state delta
    const int CHAT = CSTR_TO_PACKETID("CHAT");//R  Text Chat
};


//Base packet object for UDP datagrams
struct Packet{
    const static int  HEADER_SIZE = 16;
    const static int MAX_DATA_LENGTH = MAX_UDP_PACKET_SIZE-HEADER_SIZE;
    int id;//unique random ID
    int type;//from PacketID list
    int length;//includes these header items
    int crc;
    byte data[MAX_DATA_LENGTH];
    void ClearData();
    void CreateID();
    void SetDataLength(int data_len);
    void RunCRC();
    bool IsReliable();//if true, return an OKAY acknowledgement packet once recieved.
    bool IsMultipart();//if true, this obect can be cast to a MultipartPacket
    Payload getPayload();
};

//Extended packet objects for multipart data.
struct MultipartPacket{
    const static int  HEADER_SIZE = 28;
    const static int MAX_DATA_LENGTH = MAX_UDP_PACKET_SIZE-HEADER_SIZE;
    int id;
    int type;
    int length;
    int crc;
    short segment;
    short segment_count;
    int   payload_length;
    int   payload_offset;
    byte data[MAX_DATA_LENGTH];
    void ClearData();
    void CreateID();
    void SetDataLength(int data_len);
    void RunCRC();
    bool IsReliable();//if true, return an OKAY acknowledgement packet once recieved.
};

class ReliablePacketEnvelope{
    public:
    Packet dataPacket;
    long last_sent;
    int  retry_count;
    bool ShouldSend();
};

struct Payload{
    int type;
    int length;//includes these header items
    byte* data;
    bool free_after_use;
    Payload(int id,int len,byte* dat);
};


//Inbound assembly object for multipart data
class MultipartPayload{
    public:
    int type;
    int len;
    int id;
    int last_recv;//for non-reliable packet expiry
    BitArray packets_recieved;
    byte* assembled_payload;


    MultipartPayload();
    ~MultipartPayload();
    void Start(MultipartPacket*p);
    void Add(MultipartPacket* p);
    void Clear();
    bool IsFullyAssembled();
    bool IsExpired();
};



#endif