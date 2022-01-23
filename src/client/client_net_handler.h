#ifndef CLIENT_NET_HANDLER_H
#define CLIENT_NET_HANDLER_H

#include "client.h"
#include "../game/savefile.h"


namespace ClientNetHandler{
    extern Client* client;

    void Init(Client* c);
    void Update(int frames);
    void CreatePlayer(wchar* playername, PlayerAppearance appearance,int player_class);
    void Free();

};





#endif