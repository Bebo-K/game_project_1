#ifndef OS_NET_H
#define OS_NET_H

#include <game_project_1/net/packet.hpp>
#include <game_project_1/net/net_target.hpp>
#include <game_project_1/types/str.hpp>
#include <game_project_1/threads.hpp>

#ifdef _WIN32
    #include <game_project_1/net/os_net_win.hpp>
#endif



// Networking
namespace OSNetwork{
    bool Init();
    void Destroy();
    
    ip_address DNS_lookup(wchar* hostname, unsigned short port);
    bool connect(Packet* connect_packet,NetTarget* target);
    void disconnect(NetTarget* target);

    bool send_packet(Packet* packet, NetTarget* target);
    bool recv_packet(Packet* packet, NetTarget* target);

    Socket bind_to_port(unsigned short port);
    void unbind(Socket* listen_socket);
    int listen(Packet* packet, Socket socket,ip_address* source_addr);
};
















#endif