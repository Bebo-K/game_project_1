#ifndef SERVER_H
#define SERVER_H

#include <game_project_1/types/list.hpp>
#include <game_project_1/server/server_scene.hpp>
#include <game_project_1/game/savefile.hpp>
#include <game_project_1/game/player_info.hpp>
#include <game_project_1/signal.hpp>

class Server{
    private:
    static Server* instance;

    public:

    bool    exit;
    bool    ready;
    int     current_players;
    int     max_players;
    PlayerInfo* players;
    long long active_frames;
    List<ServerScene> active_scenes;
    SaveFile server_save;

    Server();
    ~Server();
    void Start();
    void StartShutdown();
    
    ServerScene* LoadScene(int area_id);
    void UnloadScene(int area_id);
    ServerScene* GetActiveScene(int area_id);//returns the scene if it's active, otherwise reuturns null.

    void Update(int frames);
    void UpdateScene(ServerScene* scene,int ms);

    void UpdateNetwork(int frames);
    void UpdatePlayers();

    void HandleSignals();
    static void Signal(EventSignal val);
    static Server* GetServer();
};

void ServerMain();






#endif