#ifndef CLIENT_NET_HANDLER_H
#define CLIENT_NET_HANDLER_H

#include <game_project_1/client/client.hpp>
#include <game_project_1/game/savefile.hpp>
#include <game_project_1/net/payload.hpp>


namespace ClientNetHandler{
    extern Client* client;

    void Init(Client* c);
    void Free();
    void Update(int frames);
    void HandlePayload(Payload p);
    void CreatePlayer(wchar* playername,int player_race, int player_class,color color1,int style1,int style2,int style3);
    void SendPlayerDelta();

};





#endif