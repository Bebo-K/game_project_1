#include <game_project_1/server/server.hpp>
#include <game_project_1/game.hpp>
#include <game_project_1/log.hpp>
#include <game_project_1/os.hpp>
#include <game_project_1/test/perf.hpp>

#include <game_project_1/net/network.hpp>
#include <game_project_1/client/client_signal.hpp>

#include <game_project_1/server/server_net_handler.hpp>
#include <game_project_1/server/server_signal.hpp>

#include <game_project_1/system/npc_controller.hpp>
#include <game_project_1/system/movement.hpp>
#include <game_project_1/system/physics.hpp>

#include <game_project_1/content/base_content.hpp>
#include <game_project_1/game/areas.hpp>
#include <game_project_1/game/entity_serializer.hpp>


Server* Server::instance=nullptr;
float Server::tick_interval=1;

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
    tick_interval = config::tick_interval;
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

    BaseContent::LoadServer();
    Sleep(1000);//temp just so I can see the loading screen

    state=Server::READY;
    
    if(Game::client != null){
        if(server_config::local_only){ServerNetwork::StartLocalOnly();}
        else{ServerNetwork::StartListener(max_players,true,server_config::default_port);}
        Client::Signal({ClientSignalType::LOCAL_SERVER_READY,0,0,0});
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

bool Server::SceneIsActive(int area_id){
    for(ServerScene* active_scene:active_scenes){
        if(active_scene->area_id == area_id){return true;}
    }
    return false;
}

ServerScene* Server::GetActiveScene(int area_id){
    for(ServerScene* active_scene:active_scenes){
        if(active_scene->area_id == area_id){
            return active_scene;
        }
    }
    logger::warn("Looking for active area %d but it's not loaded.\n",area_id);
    return null;
}


void Server::Update(int frames){
    UpdatePlayers();
    for(ServerScene* scene:active_scenes){
        UpdateScene(scene,frames);
        ServerNetHandler::SendEntityDeltas(scene);
    }
    ServerNetwork::Update();
    ServerNetHandler::Update(frames);
    ServerSignalHandler::Update(frames);
}

void Server::UpdateScene(ServerScene* s,int frames){
    for(int i=0;i<frames;i++){
        for(ServerEntity* entity:s->entities){
            if(entity->spawn_mode > 0)s->HandleSpawn(entity);
            if(entity->spawn_mode < 0)s->HandleDespawn(entity);
            NPCController::FrameUpdate(entity,s);
            Movement::Update(entity,tick_interval);
            Physics::ServerFrame(entity,s,tick_interval);
        }
    }
}


void Server::UpdatePlayers(){
    for(int i=0;i < max_players;i++){
        if(!ServerNetwork::PlayerConnected(i)){
            if(players[i].Active()){SaveAndRemovePlayer(i);}//Clean up players who've left
            else continue;
        }
    }
}

ServerEntity* Server::TransitionPlayer(int from_area, int to_area, int entrance_id,int player_slot){
    ServerScene* to;
    if(SceneIsActive(to_area)){ to = GetActiveScene(to_area);}
    else{to = LoadScene(to_area);}
    if(to == null){
        if(to_area != Areas::error_room){
            logger::warnW(L"Level transition for player %S failed, moving to default scene.\n",players[player_slot].character_name);
            return TransitionPlayer(to_area,Areas::error_room,0,player_slot);
        }
        else{
            logger::exceptionW(L"Failed to transition player %S to any scene!\n",players[player_slot].character_name);
            return nullptr;
        }
    }

    SavePlayer* save = players[player_slot].save;
    ServerEntity* new_entity = 
        TransitionGlobalEntity(from_area,to_area,entrance_id,save->character_global_id);

    players[player_slot].entity_id=new_entity->id;
    players[player_slot].entity_scene=to_area;
    save->last_entrance=entrance_id;
    save->last_scene=to_area;
    new_entity->player_id = player_slot;
    ServerNetHandler::OnPlayerSceneTransition(player_slot,to_area);

    if(SceneIsActive(from_area)){
        bool stay_loaded=false;
        for(int i=0;i<max_players;i++){
            if(players[i].entity_scene == from_area){stay_loaded=true;}
        }
        if(!stay_loaded){UnloadScene(from_area);}
    }
    return new_entity;
}

ServerEntity* Server::TransitionGlobalEntity(int from_area, int to_area, int entrance_id,int global_id){
    ServerEntity* target = nullptr;
    ServerScene* from;
    ServerScene* to;
    SaveEntity* save_entity =  save.GetGlobalEntity(global_id);

    if(SceneIsActive(from_area)){
        from = GetActiveScene(from_area);
        target = from->GetEntity(save_entity->id);
        if(target != null){
            save_entity->LoadFrom(target);
            from->RemoveEntity(target->id,DespawnType::FADE_OUT);
        }
    }

    if(SceneIsActive(to_area)){to=GetActiveScene(to_area);}
    else{to=LoadScene(to_area);}

    Location spawn_location = to->level.entrances[entrance_id]->GenerateLocation();

    target = to->CreateEntity((SpawnType)to->level.entrances[entrance_id]->style);
    save_entity->CopyTo(target);

    //Overwrite with position
    target->x=spawn_location.position.x; target->y=spawn_location.position.y; target->z=spawn_location.position.z;
    target->rotation = spawn_location.rotation;
    target->delta_mask |= EntitySerializer::GUARENTEED_COMPONENTS;
    target->spawn_mode = to->level.entrances[entrance_id]->style;
    save.AssignEntityToScene(global_id,to_area,true);
    return target;
}

void Server::SaveAndRemovePlayer(int player_slot){
    int player_scene_id = players[player_slot].entity_scene;
    if(player_scene_id != 0){
        ServerScene* player_scene = GetActiveScene(player_scene_id);
        ServerEntity* player_entity = player_scene->GetEntity(players[player_slot].entity_id);

        if(player_entity != null){
            save.PersistEntity(player_entity);
            player_scene->RemoveEntity(player_entity->id,DespawnType::FADE_OUT);
            bool stay_loaded=false;
            for(int i=0;i<max_players;i++){
                if(i==player_slot)continue;
                if(players[i].entity_scene == player_scene_id){stay_loaded=true;}
            }
            if(!stay_loaded){UnloadScene(player_scene_id);}
        }
    }


    players[player_slot].Clear();
    current_players--;
}