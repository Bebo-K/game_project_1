#ifndef GAME_H
#define GAME_H

#include <chrono>
#include "client.h"
using namespace std::chrono;

namespace Game{
    extern double frame_interval;
    extern double render_interval;
    extern int window_width;
    extern int window_height;
    extern bool running;
    extern Client* client;
    //extern Server server;

    void Start();
    void Paint();
    void Update(int ms);
    void Poll();
}


#endif