#ifndef CLIENT_H
#define CLIENT_H

#include "../gui/ui.h"
#include "../game/scene.h"

class Client{

    public:

    Scene       scene;
    Entity*     my_player;
    Renderer    scene_renderer;
    UI          ui;

    const int   FRAMESKIP_MAX=5;//Don't simulate more than 5 frames per update.

    Client();
    ~Client();
    void Start();
    void LoadScene(int scene_id);
    void SetMenu(int menu_id);
    void AddEntity(int eid);
    void RemoveEntity(int uuid);
    void SpawnPlayer(Entrance entrance);

    void Paint();
    void Update(int frames);
    void UpdatePositions();
    void HandleUIInput();
    void HandleFrameInput();

};

#endif