#ifndef GAME_H
#define GAME_H

#include "input.h"
#include "client/client.h"
#include "test/perf.h"

namespace Game{
    extern int framerate;
    extern int drawrate;
    extern bool running;
    extern Client* client;
    //extern Server* server;

    void Start();
    void Paint();
    void Update(int frames);
    void Poll();
    void Exit();

    float FrameInterval();
    float DrawInterval();

    //OS Hooks
    void PostRender();
}



#endif