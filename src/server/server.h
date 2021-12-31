#ifndef SERVER_H
#define SERVER_H

#include "../struct/list.h"
#include "../game/scene.h"
#include  "../game/savefile.h"
#include  "../net/network.h"


class Server{

    public:

    bool    exit;
    bool    ready;
    int     current_players;
    int     max_players;
    List<Scene> active_scenes;
    SaveFile server_save;
    ServerNetwork network;

    Server();
    void Start();
    void LoadScene(int scene_id);
    void AddEntity(int eid);
    void RemoveEntity(int uuid);
    
    void Update(int ms);
    void UpdateScene(Scene* scene,int ms);


};

void ServerMain();






#endif