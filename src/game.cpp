#include "game.h"
#include "gfx/texture.h"
#include "gfx/shader.h"
#include "gfx/model.h"
#include "gfx/animation.h"
#include "log.h"

double Game::frame_interval = 100.0f;
double Game::render_interval = 100.0f;
int Game::window_width = 720;
int Game::window_height = 1280;
time_point<system_clock> last_frame;
time_point<system_clock> last_render;
bool Game::running = false;
Client* Game::client;

void Game::Start(){
    client = new Client();
    running=true;
    ShaderManager::Init();
    TextureManager::Init();
    ModelManager::Init();
    AnimationManager::Init();
    client->Load();
}

void Game::Update(int ms){
    AnimationManager::Update(ms/1000.0f);
    client->Update(ms);
}

void Game::Paint(){
    glClearColor(0.1,0.1,0.1,1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    client->Paint();
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