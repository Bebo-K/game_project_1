#ifndef CLIENT_H
#define CLIENT_H

#include "../gui/gui.h"
#include "../game/scene.h"
#include "../gui/ui_types.h"

namespace ClientNet{
    enum State{
        NO_CONNECTION,
        LOCAL_SERVER_STARTED,
        CONNECTING,
        CONNECTION_ERROR,
        CONNECTED
    };

};

struct ClientState{




};

class Client{
    private:
    static class Client* instance;
    ClientNet::State network_state;
    char*   network_substatus;

    public:

    Scene       scene;
    Renderer    scene_renderer;
    GUI         ui;

    const int   FRAMESKIP_MAX=5;//Don't simulate more than 5 frames per update.

    Client();
    ~Client();

    static Client* GetClient();

    void Start();
    void StartLoadScene(int scene_id);
    //void OnLoadSceneFinish();
    void SetMenu(int menu_id);

    void Connect(char* full_uri_string);
    void LocalConnect();

    void AddEntity(int eid);
    void RemoveEntity(int uuid);
    void SpawnPlayer(Entrance entrance);

    void Paint();
    void Update(int frames);
    void LoadingUpdate();

    void UpdatePositions();
    void HandleUIInput();
    void HandleFrameInput();
    void UpdateNetworkState(ClientNet::State new_state);
    void SetNetworkSubstatus(char* status_msg);
    void HandleNetworkState();
    void SignalLocalServerReady();

    void Quit();
};

#endif