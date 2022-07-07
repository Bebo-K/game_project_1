#ifndef NET_TYPES_H
#define NET_TYPES_H

#ifdef _WIN32
    #include <game_project_1/net/os_net_win.hpp>
#endif

#include <game_project_1/net/payload.hpp>
#include <game_project_1/types/str.hpp>
#include <game_project_1/threads.hpp>

struct NetAddress{
    char flags;
    union{
    char ipv4[4];
    char ipv6[16];
    };
    unsigned int ipv6_flowinfo;
    unsigned int ipv6_scope;
    unsigned int ipv6_scopeid;
    unsigned short port;

    inline void Clear(){memset(this,0,sizeof(NetAddress));}
    inline void SetValid(bool value){flags = (value)? (flags | 0x01) : (flags ^ 0x01);}
    inline bool IsValid(){return (flags & 0x01) > 0;}
    inline void SetIpv6(bool value){flags = (value)? (flags | 0x02) : (flags ^ 0x02);}
    inline bool IsIpv6(){return (flags & 0x02) > 0;}
    inline void SetLocalhost(bool value){flags = (value)? (flags | 0x04) : (flags ^ 0x04);}
    inline bool IsLocalhost(){return (flags & 0x04) > 0;}

    inline bool Matches(struct NetAddress* a2){
        if(flags != a2->flags)return false;
        if(port != a2->port)return false;
        if(IsIpv6()){
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

struct Connection{
    const static int CONNECTED=2;           //connection established
    const static int CONNECTING=1;          //awaiting acknowledgement from first packet
    const static int NOT_CONNECTED=0;       //ready for connection
    const static int DISCONNECTED=-1;        //disconnected, needs cleanup
    const static int INVALID=-2;            //not a valid socket
    const static int TIMED_OUT=-3;          //connection timed out
    const static int NETWORK_UNAVAILABLE=-4;//network cannot be reached
    const static int ABORTED=-5;            //connection closed due to error
    const static int CANT_RESOLVE=-6;       //can't resolve host
    const static int UNKNOWN_ERROR=-7;      //unknown failure

    //Connection details
    wchar* hostname;
    unsigned short port;
    NetAddress address;

    //Connection implementation handles
    Socket socket_handle;
    SynchronousBuffer* buffer_link;

    int    state;
    wchar* message;

    Connection();
    ~Connection();

    void FromHostPort(wchar* host, unsigned short connect_port);
    void FromIPAddress(NetAddress incoming_address);
    void LocalLink(SynchronousBuffer* dest);

    void SetState(int state,wchar* msg);

    bool Connect(Datagram* data);
    void Disconnect();

    bool Write(Datagram* data);
    bool Read(Datagram* data);

    bool IsConnected();
    void Clear();
};




#endif