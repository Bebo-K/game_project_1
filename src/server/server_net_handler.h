#ifndef SERVER_NET_HANDLER_H
#define SERVER_NET_HANDLER_H

#include "server.h"

namespace ServerNetHandler{
    extern Server* server;
    
    void Init(Server* s);
    void Update(int frames);
    void Free();
    
}







#endif