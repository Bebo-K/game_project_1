#ifndef NET_TARGET_H
#define NET_TARGET_H

#include "packet.h"
#include "../struct/str.h"
#include "../struct/pool.h"
#include "../threads.h"

#ifdef _WIN32
#include <ws2tcpip.h>
typedef SOCKET Socket;
#elif
typedef int Socket;
#endif

struct ip_address{
    char flags;
    union{
    char ipv4[4];
    char ipv6[16];
    };
    unsigned int ipv6_flowinfo;
    unsigned int ipv6_scope;
    unsigned int ipv6_scopeid;
    unsigned short port;

    inline void Clear(){memset(this,0,sizeof(ip_address));}
    inline void SetValid(bool value){flags = (value)? (flags | 0x01) : (flags ^ 0x01);}
    inline bool IsValid(){return (flags & 0x01) > 0;}
    inline void SetIpv6(bool value){flags = (value)? (flags | 0x02) : (flags ^ 0x02);}
    inline bool IsIpv6(){return (flags & 0x02) > 0;}
    inline void SetLocalhost(bool value){flags = (value)? (flags | 0x04) : (flags ^ 0x04);}
    inline bool IsLocalhost(){return (flags & 0x04) > 0;}

    inline bool Matches(struct ip_address* a2){
        if(flags != a2->flags)return false;
        if(port != a2->port)return false;
        if(IsIpv6){
            for(int i=0;i<16;i++){if(ipv6[i] != a2->ipv6[i])return false;}
            if(ipv6_flowinfo != a2->ipv6_flowinfo){return false;}//not sure about these 3
            if(ipv6_scope != a2->ipv6_scope){return false;}
            if(ipv6_scopeid != a2->ipv6_scopeid){return false;}
        }
        else{
            for(int i=0;i<4;i++){if(ipv4[i] != a2->ipv4[i])return false;}
        }
        return true;
    }
};

class NetTarget{
    private:
    Packet buffer_packet;
    public:
    const static int
        DISCONNECTING=4,    //waiting for disconnect
        CONNECTED_LOCAL=3,  //non-network local connection only.
        CONNECTED=2,        //connection established
        CONNECTING=1,       //awaiting acknowledgement from first packet
        NOT_CONNECTED=0,    //ready for connection
        INVALID=-1,         //not a valid socket
        TIMED_OUT=-2,       //connection timed out
        NETWORK_UNAVAILABLE=-3, //network cannot be reached
        ABORTED=-4,         //connection closed due to error
        UNKNOWN_ERROR=-5;   //unknown failure


    ip_address address;
    wchar* hostname;

    Socket                 socket;
    int                    state_id;
    wchar*                 state_msg;
    int                    last_recv;
    int                    conn_start;

    Pool<ReliablePacketEnvelope> reliable_buffer;
    Pool<MultipartPayload> multipart_buffer;

    SynchronousBuffer inbound_buffer;//buffer of packets recieved
    SynchronousBuffer outbound_buffer;//buffer of packets to send

    void SetAddress(wchar* hostname,unsigned short port);
    void SetAddress(wchar* host_and_port);
    void SetAddress(ip_address address);
    void ForLocal();
    void Clear();

    bool IsConnected();
    void SetStateMsg(wchar* msg);

    void Update();
    void Send(Packet* packet);
    void Send(Payload payload);
    Payload Recieve();
    void Disconnect(wchar* reason);

    void AddToReliableBuffer(Packet* packet);
    void RemoveFromReliableBuffer(int packet_id);

    MultipartPayload* AddToMultipartPayload(MultipartPacket* packet);//gets or creates a multipartpayload in the buffer for the packet's ID and inserts it.
    void RemoveFromMultipartBuffer(int packet_id);//for purging/expiry, data pointer is freed
    Payload FinishMultipartPayload(int packet_id);//Destroys a completed payload. Data pointer ownership is transferred to the returned Payload object.

    

    NetTarget();
    ~NetTarget();
};



#endif