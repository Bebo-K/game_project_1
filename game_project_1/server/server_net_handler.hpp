#ifndef SERVER_NET_HANDLER_H
#define SERVER_NET_HANDLER_H

#include <game_project_1/server/server.hpp>
#include <game_project_1/net/packets.hpp>
#include <game_project_1/core/entity.hpp>
#include <game_project_1/core/player.hpp>
#include <game_project_1/types/data_types.hpp>

namespace ServerNetHandler{
    extern ServerComponentMask initial_sync_components;
    extern ServerComponentMask delta_components;

    extern Server* server;
    
    void Init(Server* s);
    void Update(int frames);
    void Free();

    void HandlePayload(int player_slot,Payload p);
    void SendToPlayersInArea(Payload p,int area_id);

    void OnStartNewPlayerSave(int player_slot,Packet::SNPS new_save_info);
    void OnPlayerSceneTransition(int player_slot,int area_id);
    void OnPlayerInfoUpdate(int player_slot);

    void OnClientDelta(int player_slot,Payload delta);
    void SendEntityDeltas(ServerScene* s);

    //Async methods
    void OnPlayerConnect(int player_slot,Payload request);
    void OnPlayerDisconnect(int player_slot,wchar* reason);

    //Packet handlers
    Payload WriteFullScene(Player* player_for,ServerScene* scene);
    Payload WriteSceneNewEntities(ServerScene* scene);
    Payload WriteSceneDeletedEntities(ServerScene* scene);
    Payload WriteSceneDelta(ServerScene* scene);
    void ParseClientDelta(Player* player_from,ServerScene* scene,Payload p);
}







#endif