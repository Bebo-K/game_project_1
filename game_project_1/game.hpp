#ifndef GAME_H
#define GAME_H

#include <game_project_1/input.hpp>
#include <game_project_1/client/client.hpp>
#include <game_project_1/server/server.hpp>
#include <game_project_1/test/perf.hpp>
#include <game_project_1/types/timestep.hpp>

namespace Game{
    extern bool running;
    extern Client* client;
    extern Server* server;

    void Start();
    void StartLocalServer();
    void Paint();
    void Update(Timestep delta);
    void Poll();
    void Exit();

    //OS Hooks
    void PostRender();
}



#endif