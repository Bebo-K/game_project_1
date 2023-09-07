#include <game_project_1/server/server.hpp>
#include <game_project_1/game.hpp>
#include <game_project_1/io/log.hpp>
#include <game_project_1/os.hpp>
#include <game_project_1/test/perf.hpp>

#include <game_project_1/net/network.hpp>
#include <game_project_1/client/client_signal.hpp>

#include <game_project_1/server/server_net_handler.hpp>
#include <game_project_1/server/server_signal.hpp>

#include <game_project_1/content/base_content.hpp>



Server* Server::instance=nullptr;
float Server::tick_interval=1;

void ServerMain(){
    Server* server = new Server();
    Game::server = server;
    logger::info("Launching local server.\n");
    server->Start();
    
    
    int tick_interval_ms = 1000/config::tickrate;
    long last_tick = OS::time_ms();
    long poll_time = OS::time_ms();
    long delta_ms = poll_time - last_tick;
    const int   FRAMESKIP_MAX=5;//Don't simulate more than 5 frames per update.

    do {
        OS::SleepThread(tick_interval_ms/4);
        poll_time = OS::time_ms();
        delta_ms = poll_time - last_tick;
        if(delta_ms >= tick_interval_ms){
            int elapsed_frames = (int)(delta_ms/tick_interval_ms);
            if(elapsed_frames > FRAMESKIP_MAX){
                logger::info("Server skipping %d frames, it may be struggling to keep up.\n",elapsed_frames-FRAMESKIP_MAX);
                elapsed_frames = FRAMESKIP_MAX;
            }
            server->Update(elapsed_frames);
            last_tick = poll_time - (delta_ms%tick_interval_ms);
        }
    } while(server->state != Server::EXITING);
    Game::server=nullptr;
    delete server;
}

Server::Server():scene_manager(this){
    state = Server::INIT;
    instance = this;
    active_frames=0;
    max_players = server_config::player_count;
    tick_interval = config::tick_interval;
    current_players =0;
    players = new Player[max_players];
    ServerNetwork::Init();
    ServerNetHandler::Init(this);
    ServerSignalHandler::Init(this);
}

Server::~Server(){
    delete[] players;
    ServerNetwork::Free();
    ServerNetHandler::Free();
    ServerSignalHandler::Free();
    logger::info("Server has shut down.\n");
    instance = nullptr;
}

Server* Server::GetServer(){return instance;}


void Server::HandleSignals(){/*TODO- remember what this is*/}
void Server::Signal(EventSignal val){ServerSignalHandler::Signal(val);}


void Server::Start(){
    logger::info("Server is starting..\n");

    scene_manager.save.LoadOrNew(server_config::save_name);

    BaseContent::LoadServer();
    BaseContent::LoadTestArea(scene_manager.LoadScene(1));
    Sleep(1000);//temp just so I can see the loading screen

    state=Server::READY;
    
    if(Game::client != null){
        if(server_config::local_only){ServerNetwork::StartLocalOnly();}
        else{ServerNetwork::StartListener(max_players,true,server_config::default_port);}
        Client::Signal({ClientSignalType::LOCAL_SERVER_READY,0,0,0});
    }
    else{
        ServerNetwork::StartListener(max_players,false,server_config::default_port);
    }//It's safe to connect.
    logger::info("Server is now accepting clients.\n");
}

void Server::StartShutdown(){
    logger::info("Server shutting down...\n");
    scene_manager.save.Save(server_config::save_name);
    scene_manager.UnloadAllScenes();
    scene_manager.active_scenes.Clear();
    state=EXITING;
    ServerNetwork::ShutdownListener();
}

void Server::Update(int frames){
    UpdatePlayers();
    for(ServerScene* scene:scene_manager.active_scenes){
        scene->Update(frames);
        ServerNetHandler::SendEntityDeltas(scene);
    }
    ServerNetwork::Update();
    ServerNetHandler::Update(frames);
    ServerSignalHandler::Update(frames);
}

void Server::UpdatePlayers(){
    for(int i=0;i < max_players;i++){
        if(!ServerNetwork::PlayerConnected(i)){
            if(players[i].Active()){scene_manager.SaveAndRemovePlayer(i);}//Clean up players who've left
            else continue;
        }
    }
}
