#ifndef SERVER_H
#define SERVER_H

#include <game_project_1/types/list.hpp>
#include <game_project_1/types/timestep.hpp>
#include <game_project_1/core/savefile.hpp>
#include <game_project_1/core/player.hpp>
#include <game_project_1/signal.hpp>
#include <game_project_1/server/scene_manager.hpp>

class Server{
    private:
    static Server* instance;

    public:
    static float tick_interval;
    int state;  const static int INIT=1,READY=2,PAUSED=3,EXITING=0;
    long long active_frames;
    int current_players;
    int max_players;
    Player* players;
    SceneManager scene_manager;

    Server();
    ~Server();
    void Start();
    void StartShutdown();

    void Update(Timestep delta);
    void UpdatePlayers();

    void HandleSignals();
    static void Signal(EventSignal val);
    static Server* GetServer();
};

void ServerMain();






#endif