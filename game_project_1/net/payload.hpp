#ifndef PAYLOAD_H
#define PAYLOAD_H

#include <game_project_1/types/arrays.hpp>

#define CSTR_TO_PACKETID(str) (int)str[0]<<24 | (int)str[1]<<16 | (int)str[2]<<8 | (int)str[3]
const int MAX_UDP_PACKET_SIZE=508;

namespace PacketID{
    const int OKAY = CSTR_TO_PACKETID("OKAY");//   (Both sides) Acknowledge reliable packet 
    const int JOIN = CSTR_TO_PACKETID("JOIN");//   Client join request
    const int ACPT = CSTR_TO_PACKETID("ACPT");//R  Server accepts new client connection
    const int PLYR = CSTR_TO_PACKETID("PLYR");//R  Server broadcasted notification of newly connected player
    const int NOPE = CSTR_TO_PACKETID("NOPE");//   Disconnect (Either side)
    const int PING = CSTR_TO_PACKETID("PING");//   Ping loopback-- finds latency and keeps connection fresh
    const int PINF = CSTR_TO_PACKETID("PINF");//R  Player Info-- Sent to new players to catch them up or when their state changes
    const int PLDC = CSTR_TO_PACKETID("PLDC");//R  Player Disconnect-- Server notification of a player disconnecting
    const int CHAT = CSTR_TO_PACKETID("CHAT");//R  Text Chat
    const int SNPS = CSTR_TO_PACKETID("SNPS");//R  Start New Player Save 
    const int INFO = CSTR_TO_PACKETID("INFO");//   Server info request
    const int SINF = CSTR_TO_PACKETID("SINF");//   Server info response

    const int SCNE = CSTR_TO_PACKETID("SCNE");//R  Scene Info (Server prompt for client to load scene)
    const int SPWN = CSTR_TO_PACKETID("SPWN");//R  Server spawned entity
    const int DSPN = CSTR_TO_PACKETID("DSPN");//R  Server despawned entity
    const int DLTA = CSTR_TO_PACKETID("DLTA");//   Server Entity delta
    const int CDLT = CSTR_TO_PACKETID("CDLT");//   Client state delta
};

const int reliable_payload_ids[] = {
    PacketID::JOIN,
    PacketID::ACPT,
    PacketID::PLYR,
    PacketID::PINF,
    PacketID::PLDC,
    PacketID::SNPS,
    PacketID::SCNE,
    PacketID::SPWN,
    PacketID::DSPN,
    PacketID::CHAT,
};

const int DATAGRAM_HEADER_SIZE = 20;

//Message to transfer over network
struct Payload{
    int id;
    int type;
    int length;//includes these header items
    int timestamp;
    byte* data;
    bool free_after_use;

    Payload();
    Payload(int id,int type,int len,int timestamp,byte* dat);
    Payload(int type,int len,byte* dat);//Outbound constructor
    
    void Clear();

    bool IsReliable();//Packet will be resent on an interval until confirmed receipt
};

struct MultipartHeader{
    short segment;//index of this segment
    short segment_count;//# of segments
    int   packet_data_length;//length of this packet
    int   payload_offset;//offset of packet data into full payload
};
const int MULTIPART_DATA_LENGTH = MAX_UDP_PACKET_SIZE-(DATAGRAM_HEADER_SIZE+sizeof(MultipartHeader));

//Raw UDP datagram wrapper
struct Datagram{
    const static int MAX_DATA_LENGTH = MAX_UDP_PACKET_SIZE-DATAGRAM_HEADER_SIZE;
    int id;//unique random ID
    int type;//from PacketID list
    int length;//length of payload in data buffer only
    int timestamp;//time it was sent (ms)
    int crc;
    byte data[MAX_DATA_LENGTH];

    Datagram();
    void Clear();
    bool IsMultipart();//if true, getMultipartHeader returns a valid multipart header, else ToPayload is valid to use.
    MultipartHeader getMultipartHeader();
    Payload ToPayload();

    void FromPayload(Payload payload);
    int FromPayload(Payload payload,int segment);//for use when packet is multipart
    int Size();//buffer size needed to write this
};



class OutboundReliablePayload{
    public:
    int id;
    int type;
    int length;
    long last_sent;//for resend
    int  retry_count;//for send-side expiry
    byte* data;

    OutboundReliablePayload();
    OutboundReliablePayload(int type_id,byte* dat,int len);
    ~OutboundReliablePayload();
    void Clear();
    bool ShouldSend();
    Payload GetPayload();
};

//Inbound assembly buffer for incomplete multipart data
class InboundMultipartPayload{
    public:
    int id;
    int type;
    int len;
    int last_recv;//for non-reliable packet expiry
    BitArray packets_recieved;
    byte* assembled_payload;

    InboundMultipartPayload();
    ~InboundMultipartPayload();
    void Start(Datagram* p);
    void Add(Datagram* p);
    Payload Finish();
    void Clear();
    bool IsFullyAssembled();
    bool IsExpired();
};



#endif