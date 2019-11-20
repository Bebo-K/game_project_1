#include "game.h"

Client* Game::client = nullptr;

void Game::start(){
    Game::client = new Client();

    return;
}

void Game::update(int ms){
    if(Game::client != nullptr){
        Game::client->update(ms);
    }
    return;
}

void Game::paint(){
    if(Game::client != nullptr){
        Game::client->paint();
    }
    return;
}



void Game::poll(){
    time_point<system_clock> current_time = high_resolution_clock::now();
    duration<double, std::milli> time_delta = current_time - last_frame;
    if(time_delta.count() > frame_interval){
        int elapsed_ms = (time_delta.count() > frame_interval)?frame_interval:time_delta.count();
        update(elapsed_ms);
        last_frame = high_resolution_clock::now();
    }
    current_time = high_resolution_clock::now();
    time_delta = current_time - last_render;
    if(time_delta.count() > render_interval){
        paint();
        last_render = high_resolution_clock::now();
    }
}  