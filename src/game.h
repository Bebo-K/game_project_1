#ifndef GAME_H
#define GAME_H

#include <chrono>
#include "client.h"
using namespace std::chrono;

class Game{

    time_point<system_clock> last_frame;
    time_point<system_clock> last_render;
    double frame_interval = 1000.0; 
    double render_interval = 2000.0;



    public:

    static Client* client;
    //static Server server;

    
    bool running;

    void start();
    void paint();
    void update(int ms);
    void poll();
    


};


#endif