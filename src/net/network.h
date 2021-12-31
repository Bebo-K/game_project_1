#ifndef NETWORK_H
#define NETWORK_H

#include "net_target.h"
#include "packet.h"
#include "../threads.h"
#include "../struct/arrays.h"

#include <iostream>

//Some notes
//The largest safe UDP packet size over the internet is 508 bytes
//When fragmented, UDP packets can combine into a max 67KB payload

class ClientNetwork{
    public:

    const static int
        CONNECTION_ERROR=-1,
        NO_CONNECTION=0,
        CONNECTING=1,
        CONNECTED=2,
        LOCAL_SERVER_STARTED=4;

    bool running;
    bool local_only;
    NetTarget server_target;
    
    int      network_state;
    wchar*   network_substatus;
    long     connection_start;
    long     last_ping;

    ClientNetwork();
    ~ClientNetwork();

    void StartConnect(wchar* host_string,Packet* join_request);
    void LocalConnect(Packet* join_request);
    void Disconnect(wchar* reason);

    void Update();
    void SetNetworkState(int state,wchar* status);

    void Send(Payload dat);
    void Send(Packet* dat);
    Payload Recieve();
};

class ServerNetwork{
    public:
    bool running;

    bool listener_enabled;
    Socket listener_socket;
    unsigned short listener_port;

    Array<NetTarget> targets;
    NetTarget local_client_target;

    ServerNetwork();  
    ~ServerNetwork();
    
    void StartLocalOnly();
    void StartListener(int target_slots,unsigned short port);
    void Update();
    void ShutdownListener();

    void WriteToTarget(int target_id,Payload payload);
    Payload ReadFromTarget(int target_id);

    void HandleNewTarget(Packet* request_packet,ip_address remote_address);
    void DisconnectTarget(int target_id,wchar* reason);
};



















#endif