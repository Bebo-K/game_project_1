#ifndef SERVER_NET_HANDLER_H
#define SERVER_NET_HANDLER_H

#include "server.h"
#include "../net/packet_builder.h"

namespace ServerNetHandler{
    extern Server* server;
    
    void Init(Server* s);
    void Update(int frames);
    void Free();

    Packet OnPlayerConnect(Packet* JOIN,int player_id);
    void OnPlayerDisconnect(wchar* reason,int player_id);
    void OnPlayerFailConnect(wchar* player_name,wchar* reason);
}







#endif