#ifndef SERVER_NET_HANDLER_H
#define SERVER_NET_HANDLER_H

#include <game_project_1/server/server.hpp>
#include <game_project_1/net/packet_builder.hpp>

namespace ServerNetHandler{
    extern Server* server;
    
    void Init(Server* s);
    void Update(int frames);
    void Free();

    void OnPlayerConnected(int player_id);
    void OnStartNewPlayerSave(int player_id,Payload snps_packet);
    void OnPlayerSceneTransition(int player_id,int area_id);
    void SendEntityDeltas(ServerScene* s);

    ComponentChunk::Mask GetComponentPermissionMask(ComponentChunk::Mask mask,int player_id,Server* e);

    //Async methods
    Packet OnPlayerConnect(Packet* JOIN,int player_id);
    void OnPlayerDisconnect(wchar* reason,int player_id);
    void OnPlayerFailConnect(wchar* player_name,wchar* reason);
}







#endif