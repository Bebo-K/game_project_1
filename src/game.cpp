#include "game.h"
#include "os.h"
#include "gfx/texture.h"
#include "gfx/shader.h"
#include "gfx/model.h"
#include "gfx/animation.h"
#include "log.h"
#include <math.h>

bool Game::running = false;
Client* Game::client;
//Server* Game::server;

int Game::framerate = 60;
int Game::drawrate = 60;

long last_frame;
long last_render;

float Game::FrameInterval(){return 1.0f/framerate;}
float Game::DrawInterval(){return 1.0f/drawrate;}

void Game::Start(){
    client = new Client();
    client->Start();
    last_frame = time_ms();
    running=true;
}

void Game::Update(int frames){
    client->Update(frames);
    //server->Update(frames);
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
    int frame_interval_ms = 1000/framerate;
    int draw_interval_ms = 1000/drawrate;

    long poll_time = time_ms();
    long delta_ms = poll_time - last_frame;

    if(delta_ms >= frame_interval_ms){
        int elapsed_frames = (int)(delta_ms/frame_interval_ms);
        Update(elapsed_frames);
        last_frame = poll_time - (delta_ms%frame_interval_ms);
    }

    delta_ms = poll_time - last_render;
    if(delta_ms >= draw_interval_ms){
        Paint();
        PostRender();
        last_render = poll_time - (delta_ms%draw_interval_ms);
    }
}  

void Game::Exit(){
    running=false;
    if(client != nullptr){
        //client->Unload();
        delete client;
    }
}