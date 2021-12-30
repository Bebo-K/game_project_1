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

    enum SocketState{
        CONNECTED_LOCAL=2,//non-network local connection only.
        CONNECTED=1,//connection established
        NOT_CONNECTED=0,//ready for connection
        INVALID=-1, //not a valid socket
        TIMED_OUT=-2,
        NETWORK_UNAVAILABLE=-3,
        ABORTED=-4,
        UNKNOWN_ERROR=-5
    };

    bool init();
    
    Socket bind_to_port(int port);
    Socket connect(Packet* connect_packet,wchar* hostname,short port);
    bool disconnect(Socket remote_socket);

    bool send_packet(Packet* packet, Socket dest);
    bool recv_packet(Socket* bound_socket);

    void set_state_for_socket(Socket target_socket,SocketState state_code, wchar* msg);
    int get_state_for_socket(Socket target_socket,wchar* message);
    

    void destroy();
};
















#endif