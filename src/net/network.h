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
namespace ClientNetwork{
    extern bool running;
    extern long last_ping;
    extern NetTarget server_target;

    void Init();
    void Free();
    bool IsRunning();

    void StartConnect(wchar* host_string,Packet* join_request);
    void LocalConnect(Packet* join_request);
    void Disconnect(wchar* reason);

    void Update();

    void Send(Payload dat);
    void Send(Packet* dat);
    Payload Recieve();

    NetTarget* GetNetTargetForLocal();
};

namespace ServerNetwork{
    extern bool running;
    extern bool listener_enabled;
    extern Socket listener_socket;
    extern unsigned short listener_port;
    extern NetTarget *targets;
    extern NetTarget *local_client_target; //will be targets[0] if allowing local connections, otherwise null
    extern int target_count;
    extern int connected_targets;

    void Init();  
    void Free();
    bool IsRunning();
    NetTarget* GetNetTargetForLocal();
    
    void StartLocalOnly();
    void StartListener(int target_slots,bool allow_local,unsigned short port);
    void Update();
    void ShutdownListener();

    bool TargetConnected(int target_id);
    void SendToTarget(int target_id,Payload payload);
    void SendToTarget(int target_id,Packet* packet);
    void SendToAllTargets(Packet* packet);
    void SendToAllTargets(Payload payload);
    void SendToOtherTargets(Payload payload,int excluded_target);
    void SendToOtherTargets(Packet* packet,int excluded_target);

    Payload RecvFromTarget(int target_id);

    void HandleNewTarget(Packet* request_packet,ip_address remote_address);
    void HandleNewLocalTarget(Packet* request_packet);
    void DisconnectTarget(int target_id,wchar* reason);
};



















#endif