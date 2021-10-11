#include "server.h"
#include "../game.h"
#include "../log.h"
#include "../os.h"
#include "../test/perf.h"


void ServerMain(){
    Server* server = new Server();
    Game::server = server;
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
            server->Update(delta_ms);
            Performance::ticks.Increment();
            last_tick=poll_time;
        }
    } while(!server->exit);
}


Server::Server():active_scenes(),server_save(){
    exit = false;
    ready = false;
    current_players=0;
    max_players = 1;
}
void Server::Start(){
    server_save.Load(server_config::save_name);
    ready=true;
}
void Server::LoadScene(int scene_id){}
void Server::AddEntity(int eid){}
void Server::RemoveEntity(int uuid){}

void Server::UpdateScene(Scene* scene,int ms){




}

void Server::Update(int ms){

    for(Scene* scene:active_scenes){
        UpdateScene(scene,ms);

    }
}