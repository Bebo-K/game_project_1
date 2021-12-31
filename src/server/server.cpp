#include "server.h"
#include "../game.h"
#include "../log.h"
#include "../os.h"
#include "../test/perf.h"

#include "server_signal.h"
#include "server_network_sync.h"

SynchronousBuffer server_signals(sizeof(EventSignal),4);

void ServerMain(){
    Server* server = new Server();
    Game::server = server;
    server->Start();
    
    int tick_interval_ms = 1000/Game::tickrate;
    long last_tick = time_ms();
    long poll_time = time_ms();
    long delta_ms = poll_time - last_tick;

    do {
        Sleep(tick_interval_ms/4);  
        poll_time = time_ms();
        delta_ms = poll_time - last_tick;
        if(delta_ms >= tick_interval_ms){
            int elapsed_frames = (int)(delta_ms/tick_interval_ms);
            server->Update(elapsed_frames);
            last_tick = poll_time - (delta_ms%tick_interval_ms);
        }
    } while(!server->exit);
}

Server::Server():active_scenes(),server_save(),network(){
    exit = false;
    ready = false;
    current_players=0;
    max_players = 1;
    active_frames=0;
    ServerNetworkSynchronizer::Init(&network);
}
void Server::Start(){
    if(SaveFile::Exists(server_config::save_name)){
        server_save.Load(server_config::save_name);
    }else{
        logger::info("No server savefile found- starting a new save.");
        server_save.New();
    }
    
    Sleep(1000);//temp just so I can see the loading screen message
    ready=true;
    //It's safe to connect.
    if(Game::client != null){
        if(max_players==1){network.StartLocalOnly();}
        else{network.StartListener(max_players-1,server_config::default_port);}
        Client::Signal({ClientSignalID::LOCAL_SERVER_READY,0,0,0});
    }
    else{
        network.StartListener(max_players,server_config::default_port);
    }
}
void Server::LoadScene(int scene_id){}
void Server::AddEntity(int eid){}
void Server::RemoveEntity(int uuid){}

void Server::UpdateScene(Scene* scene,int frames){




}

void Server::Update(int frames){

    for(Scene* scene:active_scenes){
        UpdateScene(scene,frames);

    }
    
    ServerNetworkSynchronizer::Update(frames);
}


void Server::UpdateNetwork(int frames){
    Payload payload(0,0,nullptr);
    current_players=0;
    for(int i=0;i<max_players;i++){
        if(!network.TargetConnected(i))continue;

        payload=network.ReadFromTarget(i);
        while(payload.type != 0){
            switch(payload.type){
                case PacketID::CHAT:{network.WriteToAllTargets(payload);break;}
                default:break;
            }
            if(payload.free_after_use){free(payload.data);}
            payload=network.ReadFromTarget(i);
        }
    }
}

void Server::Signal(EventSignal val){server_signals.Write((byte*)&val);}
void Server::HandleSignals(){
    EventSignal signal = {0};
    while(server_signals.Read((byte*)&signal)){
        switch(signal.type){
            case ServerSignalID::PLAYER_CONNECTED:{
                int id = signal.params[1].ival;
                wchar* name = signal.params[0].strval;
                current_players++;
                Packet new_player;
                    new_player.type =  PacketID::PLYR;
                    new_player.length =  sizeof(int) + wstr::len(name);
                    //TODO: start here
                    //memcpy(new_player.data ...

                break;}
            case ServerSignalID::PLAYER_DISCONNECTED:{
                current_players--;


                break;}
            default:break;
        }
    }
}