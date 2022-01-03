#include "server.h"
#include "../game.h"
#include "../log.h"
#include "../os.h"
#include "../test/perf.h"

#include "../net/network.h"
#include "server_net_handler.h"
#include "server_signal.h"
#include "../client/client_signal.h"

Server* Server::instance=nullptr;

void ServerMain(){
    Server* server = new Server();
    Game::server = server;
    logger::info("Launching local server.\n");
    server->Start();
    
    
    int tick_interval_ms = 1000/Game::tickrate;
    long last_tick = time_ms();
    long poll_time = time_ms();
    long delta_ms = poll_time - last_tick;

    do {
        Sleep(tick_interval_ms/4);  
        poll_time = time_ms();
        delta_ms = poll_time - last_tick;
        if(delta_ms >= tick_interval_ms){
            int elapsed_frames = (int)(delta_ms/tick_interval_ms);
            server->Update(elapsed_frames);
            last_tick = poll_time - (delta_ms%tick_interval_ms);
        }
    } while(!server->exit);
    Game::server=nullptr;
    delete server;
}

Server::Server():active_scenes(),server_save(){
    exit = false;
    ready = false;
    instance = this;
    current_players=0;
    max_players = server_config::player_count;
    players= new PlayerInfo[max_players];
    active_frames=0;
    ServerNetwork::Init();
    ServerNetHandler::Init(this);
    ServerSignalHandler::Init(this);
}
Server::~Server(){
    instance = nullptr;
    ServerNetwork::Free();
    ServerNetHandler::Free();
    ServerSignalHandler::Free();
    delete[] players;
    logger::info("Server has shut down.\n");
}
void Server::Signal(EventSignal val){ServerSignalHandler::Signal(val);}
Server* Server::GetServer(){return instance;}

void Server::Start(){
    logger::info("Server is starting..\n");
    server_save.LoadOrNew(server_config::save_name);

    Sleep(1000);//temp just so I can see the loading screen message

    ready=true;
    
    if(Game::client != null){
        if(max_players==1){ServerNetwork::StartLocalOnly();}
        else{ServerNetwork::StartListener(max_players,true,server_config::default_port);}
        Client::Signal({ClientSignalID::LOCAL_SERVER_READY,0,0,0});
    }
    else{
        ServerNetwork::StartListener(max_players,false,server_config::default_port);
    }//It's safe to connect.
    logger::info("Server is now accepting clients.\n");
}

void Server::StartShutdown(){
    logger::info("Server shutting down...\n");
    server_save.Save(server_config::save_name);
    for(Scene* scene:active_scenes){
        scene->Unload();
    }
    ServerNetwork::ShutdownListener();
    exit=true;
}
void Server::LoadScene(int area_id){
    for(Scene* unloading_scene:active_scenes){
        if(unloading_scene->area_id == area_id){return;}
    }
    Scene* new_scene = new Scene();
    //TODO: Load area details...
    active_scenes.Add(new_scene);
}
void Server::UnloadScene(int area_id){
    for(Scene* unloading_scene:active_scenes){
        if(unloading_scene->area_id == area_id){
            unloading_scene->Unload();
            active_scenes.Remove(unloading_scene);
            break;
        }
    }
}
void Server::UpdateScene(Scene* scene,int frames){



}

void Server::Update(int frames){
    for(Scene* scene:active_scenes){
        UpdateScene(scene,frames);
    }
    ServerNetwork::Update();
    ServerNetHandler::Update(frames);
    ServerSignalHandler::Update(frames);
}


