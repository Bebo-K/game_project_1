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
using namespace Network;
class ClientNetwork{
    public:
    bool running;
    long last_ping;
    NetTarget server_target;

    ClientNetwork();
    ~ClientNetwork();

    void StartConnect(wchar* host_string,Packet* join_request);
    void LocalConnect(Packet* join_request);
    void Disconnect(wchar* reason);

    void Update();

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

    bool TargetConnected(int target_id);
    void WriteToTarget(int target_id,Payload payload);
    void WriteToAllTargets(Payload payload);
    Payload ReadFromTarget(int target_id);

    void HandleNewTarget(Packet* request_packet,ip_address remote_address);
    void DisconnectTarget(int target_id,wchar* reason);
};



















#endif