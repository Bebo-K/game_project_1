#ifndef CLIENT_H
#define CLIENT_H

#include "gui/ui.h"
#include "game/scene.h"

class Client{

    public:

    Scene       scene;
    Entity*     my_player;
    Renderer    scene_renderer;
    UI          ui;

    Client();
    void Start();
    void LoadScene(int scene_id);
    void AddEntity(int eid);
    void RemoveEntity(int uuid);
    void SpawnPlayer(Entrance entrance);

    void Paint();
    void Update(int ms);

};

#endif