#include <game_project_1/server/server.hpp>
#include <game_project_1/game.hpp>
#include <game_project_1/log.hpp>
#include <game_project_1/os.hpp>
#include <game_project_1/test/perf.hpp>

#include <game_project_1/net/network.hpp>
#include <game_project_1/client/client_signal.hpp>

#include <game_project_1/server/server_net_handler.hpp>
#include <game_project_1/server/server_signal.hpp>

#include <game_project_1/base/base_player.hpp>

Server* Server::instance=nullptr;

void ServerMain(){
    Server* server = new Server();
    Game::server = server;
    logger::info("Launching local server.\n");
    server->Start();
    
    
    int tick_interval_ms = 1000/config::tickrate;
    long last_tick = OS::time_ms();
    long poll_time = OS::time_ms();
    long delta_ms = poll_time - last_tick;
    const int   FRAMESKIP_MAX=5;//Don't simulate more than 5 frames per update.

    do {
        OS::SleepThread(tick_interval_ms/4);
        poll_time = OS::time_ms();
        delta_ms = poll_time - last_tick;
        if(delta_ms >= tick_interval_ms){
            int elapsed_frames = (int)(delta_ms/tick_interval_ms);
            if(elapsed_frames > FRAMESKIP_MAX){
                logger::info("Server skipping %d frames, it may be struggling to keep up.\n",elapsed_frames-FRAMESKIP_MAX);
                elapsed_frames = FRAMESKIP_MAX;
            }
            server->Update(elapsed_frames);
            last_tick = poll_time - (delta_ms%tick_interval_ms);
        }
    } while(server->state != Server::EXITING);
    Game::server=nullptr;
    delete server;
}

Server::Server():active_scenes(),save(){
    state = Server::INIT;
    instance = this;
    active_frames=0;
    max_players = server_config::player_count;
    current_players =0;
    players = new Player[max_players];
    ServerNetwork::Init();
    ServerNetHandler::Init(this);
    ServerSignalHandler::Init(this);
}

Server::~Server(){
    delete[] players;
    ServerNetwork::Free();
    ServerNetHandler::Free();
    ServerSignalHandler::Free();
    logger::info("Server has shut down.\n");
    instance = nullptr;
}
void Server::Signal(EventSignal val){ServerSignalHandler::Signal(val);}
Server* Server::GetServer(){return instance;}

void Server::Start(){
    logger::info("Server is starting..\n");

    save.LoadOrNew(server_config::save_name);


    ServerInit_BasePak1();
    Sleep(1000);//temp just so I can see the loading screen

    state=Server::READY;
    
    if(Game::client != null){
        if(server_config::local_only){ServerNetwork::StartLocalOnly();}
        else{ServerNetwork::StartListener(max_players,true,server_config::default_port);}
        Client::Signal({ClientSignalID::LOCAL_SERVER_READY,0,0,0});
    }
    else{
        ServerNetwork::StartListener(max_players,false,server_config::default_port);
    }//It's safe to connect.
    logger::info("Server is now accepting clients.\n");
}

void Server::StartShutdown(){
    logger::info("Server shutting down...\n");
    save.Save(server_config::save_name);
    for(ServerScene* scene:active_scenes){
        scene->Unload();
    }
    state=EXITING;
    ServerNetwork::ShutdownListener();
}

ServerScene* Server::LoadScene(int area_id){
    if(area_id ==0){
        return nullptr;
    }
    for(ServerScene* active_scene:active_scenes){
        if(active_scene->area_id == area_id){return active_scene;/*already loaded!*/}
    }
    ServerScene* new_scene = new (active_scenes.Allocate()) ServerScene();
    new_scene->Load(area_id);
    return new_scene;
}

void Server::UnloadScene(int area_id){
    for(ServerScene* unloading_scene:active_scenes){
        if(unloading_scene->area_id == area_id){
            unloading_scene->Unload();
            active_scenes.Delete(unloading_scene);
            break;
        }
    }
}

ServerScene* Server::GetActiveScene(int area_id){
    for(ServerScene* active_scene:active_scenes){
        if(active_scene->area_id == area_id){
            return active_scene;
        }
    }
    return null;
}


void Server::Update(int frames){
    UpdatePlayers();
    for(ServerScene* scene:active_scenes){
        scene->Update(frames);
        ServerNetHandler::SendEntityDeltas(scene);
        for(int i = scene->entities.next(-1);i != scene->entities.end().index;i = scene->entities.next(i)){
            ServerEntity* e = scene->entities[i];
            if(e->just_created){e->just_created=false;}
            if(e->just_deleted){scene->entities.Delete(i);}
        }
    }
    ServerNetwork::Update();
    ServerNetHandler::Update(frames);
    ServerSignalHandler::Update(frames);
}


void Server::UpdatePlayers(){
    for(int i=0;i < max_players;i++){
        if(players[i].state == Player::DISCONNECTING){//Clean up disconnecting players
            if(players[i].entity_scene >=0 && players[i].entity_id >= 0){
                ServerScene* scene = GetActiveScene(players[i].entity_scene);
                if(scene){scene->RemoveEntity(players[i].entity_id);}
            }
            players[i].Clear();
            current_players--;
        }
    }
}

ServerEntity* Server::TransitionPlayer(int from_area, int to_area, int entrance_id,int player_id){
    SavePlayer* player_save = save.GetPlayerByID(players[player_id].save_id);
    player_save->last_scene=to_area;
    player_save->last_entrance=entrance_id;
    
    players[player_id].state = Player::LOADING_INTO_ZONE;

    ServerScene* to = GetActiveScene(to_area);
    if(to==null){to = LoadScene(to_area);}
    if(to == null){
        if(to_area != GameConstants::DEFAULT_SCENE){
            logger::warnW(L"Level transition for player %S failed, moving to default scene.\n",players[player_id].name);
            return TransitionPlayer(to_area,GameConstants::DEFAULT_SCENE,0,player_id);
        }
        else{
            logger::exceptionW(L"Failed to transition player %S to any scene!\n",players[player_id].name);
            return nullptr;
        }
    }

    ServerEntity* new_entity = 
        TransitionGlobalEntity(from_area,to_area,entrance_id,players[player_id].save_entity_id);

    players[player_id].entity_id = new_entity->id;
    players[player_id].entity_scene=to_area;
    new_entity->player_id = player_id;

    ServerScene* from = GetActiveScene(from_area);
    if(from != null){
        bool stay_loaded=false;
        for(int i=0;i<max_players;i++){
            if(players[i].entity_scene == from_area){stay_loaded=true;}
        }
        if(!stay_loaded){UnloadScene(from_area);}
    }
    ServerNetHandler::OnPlayerSceneTransition(player_id,to_area);
    return new_entity;
}

ServerEntity* Server::TransitionGlobalEntity(int from_area, int to_area, int entrance_id,int global_id){
    ServerScene* to = GetActiveScene(to_area);
    ServerScene* from = GetActiveScene(from_area);
    ServerEntity* ret = nullptr;
    if(from != null){
        SaveEntity* save_entity =  save.GetGlobalEntity(global_id);
        ServerEntity* local_entity = from->GetEntity(save_entity->id);
        if(local_entity != null){
            save_entity->LoadFrom(local_entity);
            from->RemoveEntity(local_entity->id);
        }
    }
    if(to!=null){
        Location spawn_location = to->level.entrances[entrance_id]->GenerateLocation();
        ServerEntity* e = to->CreateEntity();
        SaveEntity* save_entity= save.GetGlobalEntity(global_id);
        save_entity->CopyTo(e);

        //Overwrite with position
        e->x=spawn_location.position.x; e->y=spawn_location.position.y; e->z=spawn_location.position.z;
        e->rotation = spawn_location.rotation;
        e->delta_mask |= ComponentChunk::BASIC_COMPONENTS;
        to->HandleSpawn(e,to->level.entrances[entrance_id]->style);
        ret = e;
    }
    save.AssignEntityToScene(global_id,to_area,true);
    return ret;
}
