#ifndef SERVER_H
#define SERVER_H

#include "../struct/list.h"
#include "../game/scene.h"
#include  "../game/savefile.h"
#include "../game/player_info.h"
#include "../signal.h"

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
    List<Scene> active_scenes;
    SaveFile server_save;

    Server();
    ~Server();
    void Start();
    void StartShutdown();
    
    void LoadScene(int area_id);
    void UnloadScene(int area_id);
    void Update(int frames);
    void UpdateScene(Scene* scene,int ms);
    void UpdateNetwork(int frames);
    void UpdatePlayers();

    
    
    void HandleSignals();
    static void Signal(EventSignal val);
    static Server* GetServer();
};

void ServerMain();






#endif