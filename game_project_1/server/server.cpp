#include <game_project_1/server/server.hpp>
#include <game_project_1/game.hpp>
#include <game_project_1/log.hpp>
#include <game_project_1/os.hpp>
#include <game_project_1/test/perf.hpp>

#include <game_project_1/net/network.hpp>
#include <game_project_1/client/client_signal.hpp>

#include <game_project_1/server/server_net_handler.hpp>
#include <game_project_1/server/server_signal.hpp>
#include <game_project_1/server/server_scene_controller.hpp>

Server* Server::instance=nullptr;

void ServerMain(){
    Server* server = new Server();
    Game::server = server;
    logger::info("Launching local server.\n");
    server->Start();
    
    
    int tick_interval_ms = 1000/Game::tickrate;
    long last_tick = OS::time_ms();
    long poll_time = OS::time_ms();
    long delta_ms = poll_time - last_tick;

    do {
        OS::SleepThread(tick_interval_ms/4);
        poll_time = OS::time_ms();
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
    for(ServerScene* scene:active_scenes){
        scene->Unload();
    }
    ServerNetwork::ShutdownListener();
    exit=true;
}
ServerScene* Server::LoadScene(int area_id){
    for(ServerScene* unloading_scene:active_scenes){
        if(unloading_scene->area_id == area_id){return;}
    }
    ServerScene* new_scene = new ServerScene();
    //TODO: Load area details...
    active_scenes.Add(new_scene);
    return new_scene;
}
void Server::UnloadScene(int area_id){
    for(ServerScene* unloading_scene:active_scenes){
        if(unloading_scene->area_id == area_id){
            unloading_scene->Unload();
            active_scenes.Remove(unloading_scene);
            break;
        }
    }
}


ServerScene* Server::GetActiveScene(int area_id){
    for(ServerScene* active_scene:active_scenes){
        if(active_scene->area_id == area_id){
            return active_scene;
        }
    }
    return null;
}

void Server::UpdateScene(ServerScene* scene,int frames){



}

void Server::Update(int frames){
    UpdatePlayers();
    for(ServerScene* scene:active_scenes){
        UpdateScene(scene,frames);
    }
    ServerNetwork::Update();
    ServerNetHandler::Update(frames);
    ServerSignalHandler::Update(frames);
}


void Server::UpdatePlayers(){
    for(int i=0;i<max_players;i++){
        //Player disconnects
        if(!players[i].connected && players[i].character_name != nullptr){//Player disconnected in network thread. Cleanup entity+state
            if(players[i].player_entity_scene >= 0 && players[i].player_entity_id >= 0){
                active_scenes[players[i].player_entity_scene]->RemoveEntity(players[i].player_entity_id);
            }
            current_players--;
            players[i].Clear();
        }
        
        //Player scene transitions


    }
}