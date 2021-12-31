#ifndef CLIENT_H
#define CLIENT_H

#include "../gui/gui.h"
#include "../game/scene.h"
#include "../gui/ui_types.h"
#include "../net/network.h"
#include "client_signal.h"

class Client{
    private:
    static class Client* instance;
    public:

    Scene       scene;
    Renderer    scene_renderer;
    GUI         ui;
    ClientNetwork network;

    const int   FRAMESKIP_MAX=5;//Don't simulate more than 5 frames per update.

    Client();
    ~Client();

    static Client* GetClient();
    static void Signal(EventSignal val);

    void Start();
    void StartLoadScene(int scene_id);
    //void OnLoadSceneFinish();
    void SetMenu(int menu_id);

    void AddEntity(int eid);
    void RemoveEntity(int uuid);
    void SpawnPlayer(Entrance entrance);

    void Paint();
    void Update(int frames);
    void UpdatePositions();
    void HandleSignals();

    void HandleUIInput();
    void HandleFrameInput();

    void Quit();
};

#endif