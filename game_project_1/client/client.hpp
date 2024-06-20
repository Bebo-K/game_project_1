#ifndef CLIENT_H
#define CLIENT_H

#include <game_project_1/types/timestep.hpp>
#include <game_project_1/gui/gui.hpp>
#include <game_project_1/signal.hpp>
#include <game_project_1/gfx/renderer.hpp>
#include <game_project_1/core/player.hpp>
#include <game_project_1/client/client_scene.hpp>

class Client{
    private:
    static class Client* instance;
    static float    frame_interval;
    public:

    ClientScene     scene;
    GUI             ui;

    int             my_slot_id;
    int             my_save_id;
    Array<Player>   players;
    int             current_players;

    const static int FRAMESKIP_MAX=5;//Don't simulate more than 5 frames per update.

    Client();
    ~Client();

    static Client* GetClient();
    static void Signal(EventSignal val);
    Player* Me();

    void Start();

    void Paint();
    void Resize(int screen_w,int screen_h);
    void Update(Timestep delta);
    void UpdateScene(Timestep delta);
    
    void HandleSignals();

    void OnSpawnPlayer(ClientEntity* player);

    void Quit();
};

#endif