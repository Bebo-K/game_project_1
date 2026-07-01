export module Game;

import OS;
import Texture;
import Shader;
import Model;
import Animation;
import Logger;
import <math>;
import Input;
import Client;
import ClientAsync;
import Server;
import Performance;
import Timestep;


namespace Game{
    export bool running = false;
    export Client* client=nullptr;
    export Server* server=nullptr;

    export int frame_interval_ms = 1000/60;//draw frames
    export int tick_interval_ms = 1000/60;//simulation frames
    export float frame_interval = 1.0f/60.0f;
    export float tick_interval = 1.0f/30.0f;

    export long last_frame;
    export long last_tick;

    export void Start(){
        client = new Client();
        client->Start();
        last_frame = OS::time_ms();
        running=true;
        frame_interval=config::frame_interval;
        tick_interval=config::tick_interval;
        frame_interval_ms=1000/config::framerate;
        tick_interval_ms=1000/config::tickrate;
    }

    export void StartLocalServer(){
        if(server != nullptr){
            logger::exception("A local server is already running. Shut down the existing server before starting a new one!\n");
            return;
        }
        OS::StartThread(ServerMain);
    }

    export void Update(Timestep delta){
        client->Update(delta);
        ClientAsync::Poll();
        Performance::frames.Increment();
    }

    export void Paint(){
        glClearColor(0.1,0.1,0.1,1.0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        client->Paint();
        Performance::draws.Increment();
    }

    export void Poll(){
        if(!running)return;

        long poll_time = OS::time_ms();
        long delta_ms = poll_time - last_frame;
        if(delta_ms >= frame_interval_ms){
            Paint();
            PostRender();
            last_frame = poll_time - (delta_ms%frame_interval_ms);
        }

        delta_ms = poll_time - last_tick;
        if(delta_ms >= tick_interval_ms){
            int elapsed_frames = (int)(delta_ms/tick_interval_ms);
            Update({elapsed_frames,elapsed_frames*tick_interval});
            last_tick = poll_time - (delta_ms%tick_interval_ms);
        }
    }  

    export void Exit(){
        running=false;
        if(server != nullptr){
            server->StartShutdown();
        }
        if(client != nullptr){
            delete client;
            client=nullptr;
        }

        logger::info("Exiting.\n");
        OS::Exit(0);
    }
}

