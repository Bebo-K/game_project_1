#ifndef CLIENT_H
#define CLIENT_H

#include "../gui/gui.h"
#include "../game/scene.h"
#include "../signal.h"
#include "../game/player_info.h"

class Client{
    private:
    static class Client* instance;
    public:

    Scene       scene;
    Renderer    scene_renderer;
    GUI         ui;

    PlayerInfo*  players;
    int          max_players;
    int          player_count;
    int          my_player_id;

    const int   FRAMESKIP_MAX=5;//Don't simulate more than 5 frames per update.

    Client();
    ~Client();

    static Client* GetClient();
    static void Signal(EventSignal val);

    void Start();
    void LoadScene(int scene_id);

    void AddEntity(int eid);
    void RemoveEntity(int uuid);
    void SpawnPlayer(Entrance entrance);

    void Paint();
    void Resize(int screen_w,int screen_h);
    void Update(int frames);
    void UpdatePositions();
    void HandleSignals();

    void HandleUIInput();
    void HandleFrameInput();

    void Quit();
};

#endif