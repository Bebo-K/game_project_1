#ifndef OS_NET_H
#define OS_NET_H

#include <game_project_1/net/net_types.h>

#ifdef _WIN32
    #include <game_project_1/net/os_net_win.hpp>
#endif



// Networking
namespace OSNetwork{

    bool Init();
    void Destroy();
    
    int DNS_lookup(wchar* hostname, unsigned short port,NetAddress& addr);
    
    void connect(Datagram* data,Connection* conn);
    void disconnect(Connection* conn);

    bool send(Datagram* data, Connection* conn);
    bool recv(Datagram* data, Connection* conn);
    bool ping(Datagram* data, NetAddress target);//One-off send that closes connection immediately

    Socket bind_to_port(unsigned short port);
    void unbind(Socket* listen_socket);
    int listen(Datagram* dgram, Socket socket,NetAddress* source_addr);
};
















#endif