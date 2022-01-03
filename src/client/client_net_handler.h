#ifndef CLIENT_NET_HANDLER_H
#define CLIENT_NET_HANDLER_H

#include "client.h"


namespace ClientNetHandler{
    extern Client* client;

    void Init(Client* c);
    void Update(int frames);
    void Free();

};





#endif