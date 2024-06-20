#include <game_project_1/game.hpp>
#include <game_project_1/os.hpp>
#include <game_project_1/gfx/texture.hpp>
#include <game_project_1/gfx/shader.hpp>
#include <game_project_1/gfx/model.hpp>
#include <game_project_1/gfx/animation.hpp>
#include <game_project_1/io/log.hpp>
#include <math.h>

bool Game::running = false;
Client* Game::client=nullptr;
Server* Game::server=nullptr;

int frame_interval_ms = 1000/60;//draw frames
int tick_interval_ms = 1000/60;//simulation frames
float frame_interval = 1.0f/60.0f;
float tick_interval = 1.0f/30.0f;

long last_frame;
long last_tick;

void Game::Start(){
    client = new Client();
    client->Start();
    last_frame = OS::time_ms();
    running=true;
    frame_interval=config::frame_interval;
    tick_interval=config::tick_interval;
    frame_interval_ms=1000/config::framerate;
    tick_interval_ms=1000/config::tickrate;
}

void Game::StartLocalServer(){
    if(Game::server != nullptr){
        logger::exception("A local server is already running. Shut down the existing server before starting a new one!\n");
        return;
    }
    OS::StartThread(ServerMain);
}

void Game::Update(Timestep delta){
    client->Update(delta);
    Performance::frames.Increment();
}

void Game::Paint(){
    glClearColor(0.1,0.1,0.1,1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    client->Paint();
    Performance::draws.Increment();
}

void Game::Poll(){
    if(!running)return;

    long poll_time = OS::time_ms();
    long delta_ms = poll_time - last_frame;
    if(delta_ms >= frame_interval_ms){
        Paint();
        PostRender();
        last_frame = poll_time - (delta_ms%frame_interval_ms);
    }

    delta_ms = poll_time - last_tick;
    if(delta_ms >= tick_interval_ms){
        int elapsed_frames = (int)(delta_ms/tick_interval_ms);
        Update({elapsed_frames,elapsed_frames*tick_interval});
        last_tick = poll_time - (delta_ms%tick_interval_ms);
    }
}  

void Game::Exit(){
    running=false;
    if(server != nullptr){
        server->StartShutdown();
    }
    if(client != nullptr){
        delete client;
        client=nullptr;
    }

    logger::info("Exiting.\n");
    exit(0);
}