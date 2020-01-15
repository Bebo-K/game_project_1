#include "game.h"
#include "gfx/texture.h"
#include "gfx/shader.h"

double Game::frame_interval=1000.0f;
double Game::render_interval=2000.0f;
time_point<system_clock> last_frame;
time_point<system_clock> last_render;
bool Game::running=false;
Client Game::client;

void Game::Start(){
    running=true;
    ShaderManager::Init();
    TextureManager::Init();
    client.Load();
}

void Game::Update(int ms){
    client.Update(ms);
}

void Game::Paint(){
    client.Paint();
}

void Game::Poll(){
    time_point<system_clock> current_time = high_resolution_clock::now();
    duration<double, std::milli> time_delta = current_time - last_frame;
    if(time_delta.count() > frame_interval){
        int elapsed_ms = (time_delta.count() > frame_interval)?frame_interval:time_delta.count();
        Update(elapsed_ms);
        last_frame = high_resolution_clock::now();
    }
    current_time = high_resolution_clock::now();
    time_delta = current_time - last_render;
    if(time_delta.count() > render_interval){
        //Paint();
        last_render = high_resolution_clock::now();
    }
}  