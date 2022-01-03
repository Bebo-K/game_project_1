#ifndef OS_NET_H
#define OS_NET_H



#include "packet.h"
#include "net_target.h"
#include "../threads.h"
#include "../struct/str.h"

#ifdef _WIN32
    #include "os_net_win.h"
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