#ifndef CLIENT_NET_HANDLER_H
#define CLIENT_NET_HANDLER_H

#include <game_project_1/client/client.hpp>
#include <game_project_1/game/savefile.hpp>


namespace ClientNetHandler{
    extern Client* client;

    void Init(Client* c);
    void Update(int frames);
    void CreatePlayer(wchar* playername, PlayerAppearance appearance,int player_class);
    void Free();

};





#endif