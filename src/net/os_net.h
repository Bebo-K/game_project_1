#ifndef OS_NET_H
#define OS_NET_H

#include "packet.h"
#include "../struct/str.h"


#ifdef _WIN32
#include <winsock2.h>
#include <ws2tcpip.h>
typedef SOCKET Socket;
#elif
typedef int Socket;
#endif

// Networking
namespace OSNetwork{
    bool init();
    
    Socket bind_to_port(int port);
    Socket connect(Packet* connect_packet,wchar* hostname,short port);
    bool disconnect(Socket remote_socket);

    bool send_packet(Packet* packet, Socket dest);
    bool recv_packet(Socket* bound_socket);

    void set_msg(wchar* msg);
    wchar* get_msg();
    

    void destroy();
};
















#endif