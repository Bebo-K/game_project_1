#include "client.h"
#include "../game.h"
#include "../log.h"
#include <stdio.h>
#include "../game/scene_loader.h"
#include "../game/system/camera_manager.h"
#include "../game/system/entity_collision.h"
#include "../game/system/movement.h"
#include "../game/system/npc_controller.h"
#include "../game/system/physics.h"
#include "../game/system/player_input.h"
#include "../game/system/transition_manager.h"
//#include "../game/system/animation_controller.h"
#include "../game/system/state_manager.h"
#include "../gfx/ui_text.h"
#include "../net/packet_builder.h"
#include "../threads.h"

Client* Client::instance = nullptr;

SynchronousBuffer client_signals(sizeof(EventSignal),4);

Client::Client() : scene(), scene_renderer(), ui(){
    logger::info("Initializing client\n");
    Client::instance = this;
    ShaderManager::Init();
    TextureManager::Init();
    ModelManager::Init();
    AnimationManager::Init();  
    FontManager::Init();
    ModelManager::Init();
}

Client::~Client(){
    FontManager::Free();
    ModelManager::Free();
    ShaderManager::Free();
    TextureManager::Free();
    ModelManager::Free();
    AnimationManager::Free();
}

Client* Client::GetClient(){return Client::instance;}
void Client::Signal(EventSignal val){client_signals.Write((byte*)&val);}

void Client::Start(){
        FontManager::LoadFontFace("Merriweather/Merriweather-Regular",8);
        int debug_font =FontManager::LoadFontFace("SourceSansPro-Regular",12);
        FontManager::SetActiveFont(debug_font);
        ModelManager::Register(PLAYER,"default_human");

    ui.Load();
    ui.main_menu->Open();
}

void Client::StartLoadScene(int scene_id){
    SceneLoader::StartLoadDefault(&scene);

    scene_renderer.camera.ortho=false;
    scene_renderer.camera.z = 10.0f;
    scene_renderer.camera.y = 2.0f;

    scene_renderer.Add(&scene.level);
    scene_renderer.Add(&scene.skybox);
}

void Client::AddEntity(int eid){
    Entity *e = scene.AddEntity(eid);
    if(e->models != null){scene_renderer.Add(e->models);}    
}

void Client::RemoveEntity(int eid){
    Entity *e = scene.GetEntity(eid);
    if(e->models != null){scene_renderer.Remove(e->models);}
    scene.RemoveEntity(eid);
}

void Client::SpawnPlayer(Entrance eid){
    Entity* my_player = scene.AddEntity(0);
    my_player->type = 1;
    my_player->name= cstr::new_copy("Chowzang");
    my_player->state = new State();
        my_player->state->Set(IDLE);
    my_player->anim_state = new AnimationState(GROUND_UNIT);
    my_player->models = new ModelSet();
        my_player->models->Add(PLAYER);
    my_player->phys_data = new PhysicsData();
        my_player->phys_data->world_hitsphere.height=3;
        my_player->phys_data->world_hitsphere.radius=0.75f;
    my_player->colliders = new ColliderSet();
        //my_player->colliders->Add();
    my_player->movement = new MovementData();
        my_player->movement->base_speed=10;
    my_player->player_data = new PlayerData();
    my_player->camera_target = new CameraTarget(&scene_renderer.camera,{0,8,12},{0,1},{-30,-31});
    my_player->unit_data = new UnitData();
    PlayerInput::Track(my_player);

    if(my_player->models != null){scene_renderer.Add(my_player->models);}  
}

void Client::Paint(){
    CameraManager::Update(&scene,0);
    scene_renderer.Draw();
    ui.Paint();
}

void Client::UpdatePositions(){
    for(Entity* e:scene.entities){        
        if(e->models != null){
            e->models->x=e->x;
            e->models->y=e->y;
            e->models->z=e->z;
            e->models->rotation=e->rotation;
        }
    }
}

void Client::Update(int frames){
    HandleUIInput();
    if(scene.loaded){
        float frame_interval = Game::FrameInterval();
        float seconds = frames*frame_interval;
        
        AnimationManager::Update(seconds);

        int frames_to_run = (frames < FRAMESKIP_MAX)?frames:FRAMESKIP_MAX;
        scene.global_timer+= frames_to_run;

        for(int frame=0;frame<frames_to_run;frame++){
            HandleFrameInput();
            
            //NPCController::Update(&scene);
            //EntityCollision::Update(&scene);
            Movement::Update(&scene,frame_interval);
            Physics::FrameUpdate(&scene,frame_interval); 
            //TransitionManager::Update(&scene,ms);
            StateManager::Update(&scene,frame_interval);
            UpdatePositions();
            ui.Update(&scene,frames);
            
            PlayerInput::UpdateCamera(&scene,frame_interval);
        }
        
        //Network::RunSync(scene);
    }
    Input::Update();
    network.Update();
    HandleSignals();
}

void Client::HandleUIInput(){
    for(Input::Event input = Input::NextEvent();input != Input::None;input = Input::NextEvent(input)){
        if(ui.OnInput(input)){Input::ClearEvent(input);}
    }
}

void Client::HandleFrameInput(){
    for(Input::Event input = Input::NextEvent();input != Input::None;input = Input::NextEvent(input)){
        if(PlayerInput::HandleInput(input)){Input::ClearEvent(input);}
    }
}

void Client::HandleSignals(){
    EventSignal signal = {0};
    while(client_signals.Read((byte*)&signal)){
        switch(signal.type){
            case ClientSignalID::CONNECTED_TO_SERVER:{
                    //LOTS:
                    ui.loading_menu->SetStatusMessage(wstr::new_copy(L"Connected to server!"));


                    //ui.CloseAll();
                    //ui.ingame_menu->Open();
                
                break;}
            case ClientSignalID::DISCONNECTED_FROM_SERVER:{
                int disconnect_code = signal.params[0].ival;
                wchar* disconnect_reason = signal.params[1].strval;
                if(disconnect_code < 0){
                    ui.CloseAll();
                    ui.error_menu->Open();
                    scene.Unload();
                    if(disconnect_reason != null){
                        ui.error_menu->SetStatusMessage(wstr::allocf(L"Disconnected from server: %s",disconnect_reason));
                        free(disconnect_reason);
                    }
                    else{
                        ui.error_menu->SetStatusMessage(wstr::new_copy(L"Disconnected from server (No reason given)"));
                    }
                }
                else{
                    ui.CloseAll();
                    ui.main_menu->Open();
                    scene.Unload();
                }
                break;}
            case ClientSignalID::LOCAL_SERVER_READY:{
                    ui.loading_menu->SetStatusMessage(wstr::new_copy(L"Connecting to local server..."));
                    Packet join_request = BuildPacket_JOIN(L"Chowzang");
                    network.LocalConnect(&join_request);
                break;}
            default:break;
        }
    }
}


void Client::Quit(){
    Game::Exit();
}