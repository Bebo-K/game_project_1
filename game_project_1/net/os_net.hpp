#ifndef OS_NET_H
#define OS_NET_H

#include <game_project_1/net/net_types.h>

#ifdef _WIN32
    #include <game_project_1/net/os_net_win.hpp>
#endif


// Raw network calls
namespace OSNetwork{
    bool Init();
    void Destroy();
    
    int DNS_Lookup(wchar* hostname, unsigned short port,NetAddress& addr);
    
    void Connect(Datagram* data,Connection* conn);
    void Disconnect(Connection* conn);

    bool Send(Datagram* data, Connection* conn);
    bool Recv(Datagram* data, Connection* conn);
    bool Ping(Datagram* data, NetAddress target);//One-off send that closes socket immediately

    Socket BindToPort(unsigned short port);
    void Unbind(Socket* listen_socket);
    int Listen(Datagram* dgram, Socket socket,NetAddress* source_addr);
};
















#endif