#ifndef CLIENT_H
#define CLIENT_H

#include "../gui/gui.h"
#include "../game/scene.h"

class Client{

    public:
    Scene       scene;
    Renderer    scene_renderer;
    GUI         ui;

    const int   FRAMESKIP_MAX=5;//Don't simulate more than 5 frames per update.

    Client();
    ~Client();
    void Start();
    void StartLoadScene(int scene_id);
    //void OnLoadSceneFinish();
    void SetMenu(int menu_id);
    void AddEntity(int eid);
    void RemoveEntity(int uuid);
    void SpawnPlayer(Entrance entrance);

    void Paint();
    void Update(int frames);
    void LoadingUpdate();

    void UpdatePositions();
    void HandleUIInput();
    void HandleFrameInput();

};

#endif