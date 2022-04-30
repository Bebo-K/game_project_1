#ifndef SERVER_H
#define SERVER_H

#include <game_project_1/types/list.hpp>
#include <game_project_1/server/server_scene.hpp>
#include <game_project_1/game/savefile.hpp>
#include <game_project_1/game/player.hpp>
#include <game_project_1/signal.hpp>

class Server{
    private:
    static Server* instance;

    public:
    int state;  const static int INIT=1,READY=2,PAUSED=3,EXITING=0;
    long long active_frames;
    int current_players;
    int max_players;
    Player* players;
    Pool<ServerScene> active_scenes;
    SaveFile save;

    Server();
    ~Server();
    void Start();
    void StartShutdown();
    
    ServerScene* LoadScene(int area_id);
    void UnloadScene(int area_id);
    ServerScene* GetActiveScene(int area_id);//returns the scene if it's active, otherwise returns null.

    void Update(int frames);

    void UpdateNetwork(int frames);
    void UpdatePlayers();

    void HandleSignals();
    static void Signal(EventSignal val);
    static Server* GetServer();

    ServerEntity* TransitionPlayer(int from_area, int to_area, int entrance_id,int player_id);
    ServerEntity* TransitionGlobalEntity(int from_area, int to_area, int entrance_id,int global_id);
};

void ServerMain();






#endif