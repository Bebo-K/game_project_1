#include <game_project_1/game.hpp>
#include <game_project_1/os.hpp>
#include <game_project_1/gfx/texture.hpp>
#include <game_project_1/gfx/shader.hpp>
#include <game_project_1/gfx/model.hpp>
#include <game_project_1/gfx/animation.hpp>
#include <game_project_1/log.hpp>
#include <math.h>

bool Game::running = false;
Client* Game::client=nullptr;
Server* Game::server=nullptr;

int frame_interval_ms = 1000/60;
int draw_interval_ms = 1000/60;

long last_frame;
long last_render;

void Game::Start(){
    client = new Client();
    client->Start();
    last_frame = OS::time_ms();
    running=true;
    frame_interval_ms=1000/config::framerate;
    draw_interval_ms=1000/config::max_drawrate;
}

void Game::StartLocalServer(){
    if(Game::server != nullptr){
        logger::exception("A local server is already running. Shut down the existing server before starting a new one!\n");
        return;
    }
    OS::StartThread(ServerMain);
}

void Game::Update(int frames){
    client->Update(frames);
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

    delta_ms = poll_time - last_render;
    if(delta_ms >= draw_interval_ms){
        Paint();
        PostRender();
        last_render = poll_time - (delta_ms%draw_interval_ms);
    }

    if(delta_ms >= frame_interval_ms){
        int elapsed_frames = (int)(delta_ms/frame_interval_ms);
        Update(elapsed_frames);
        last_frame = poll_time - (delta_ms%frame_interval_ms);
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