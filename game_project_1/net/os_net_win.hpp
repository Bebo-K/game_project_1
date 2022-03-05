#ifndef OS_NET_WIN_H
#define OS_NET_WIN_H

#ifndef _WINSOCKAPI_
    #include <winsock2.h>
    #include <ws2tcpip.h>
#endif
typedef SOCKET Socket;

#endif