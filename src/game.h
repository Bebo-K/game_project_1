#ifndef GAME_H
#define GAME_H

#include "input.h"
#include "client/client.h"
#include "server/server.h"
#include "test/perf.h"

namespace Game{
    extern int framerate; //client updates/second
    extern int drawrate; //cient draws/second
    extern int tickrate;//server updates/second
    extern bool running;
    extern Client* client;
    extern Server* server;

    void Start();
    void StartLocalServer();
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