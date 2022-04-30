#ifndef CLIENT_H
#define CLIENT_H

#include <game_project_1/gui/gui.hpp>
#include <game_project_1/signal.hpp>
#include <game_project_1/gfx/renderer.hpp>
#include <game_project_1/game/player.hpp>
#include <game_project_1/client/client_scene.hpp>

class Client{
    private:
    static class Client* instance;
    public:

    ClientScene scene;
    GUI         ui;

    int           my_entity_id;
    int           my_player_id;
    int           my_save_id;
    Array<Player> players;
    int           current_players;

    const int   FRAMESKIP_MAX=5;//Don't simulate more than 5 frames per update.

    Client();
    ~Client();

    static Client* GetClient();
    static void Signal(EventSignal val);

    void Start();

    void Paint();
    void Resize(int screen_w,int screen_h);
    void Update(int frames);
    
    void HandleSignals();

    void Quit();
};

#endif