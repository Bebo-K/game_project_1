#ifndef NETWORK_H
#define NETWORK_H

#include <game_project_1/net/net_target.hpp>
#include <game_project_1/net/payload.hpp>
#include <game_project_1/threads.hpp>
#include <game_project_1/types/arrays.hpp>

#include <iostream>

//Some notes
//The largest safe UDP datagram size over the internet is 508 bytes
//When fragmented, UDP datagrams can combine into a max 67KB payload
namespace ClientNetwork{
    extern bool running;
    extern long last_ping;
    extern NetTarget server_target;

    void Init();
    void Free();
    bool IsRunning();

    void StartConnect(wchar* host_string,wchar* persona, int save_id);
    void LocalConnect(wchar* persona, int save_id);
    void Disconnect(wchar* reason);

    void Update();

    void Send(Payload payload);
    Payload Recieve();
    
    NetTarget* GetNetTargetForLocal();
};

namespace ServerNetwork{
    extern bool running;
    extern bool listener_enabled;
    extern bool local_listener;

    extern Socket listener_socket;
    extern unsigned short listener_port;
    extern NetTarget *targets;
    extern int target_count;
    extern int connected_targets;

    void Init();  
    void Free();
    bool IsRunning();
    NetTarget* GetNetTargetForLocal();
    
    void StartLocalOnly();
    void StartListener(int player_slots,bool include_local,unsigned short port);
    void Update();
    void ShutdownListener();

    bool PlayerConnected(int player_slot);
    void SendToPlayer(int player_slot,Payload payload);
    void SendToAllPlayers(Payload payload);
    void SendToOtherPlayers(Payload payload,int excluded_player);

    Payload RecvFromPlayer(int player_slot);

    void HandleNewConnection(Payload request,NetAddress remote_address);
    int JoinPlayer(NetAddress address);
    void DisconnectPlayer(int player_slot,wchar* reason);
};



















#endif