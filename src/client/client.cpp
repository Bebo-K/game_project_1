#include "client.h"
#include "../log.h"
#include <stdio.h>
#include "../game/systems/camera_manager.h"
#include "../game/systems/entity_collision.h"
#include "../game/systems/movement.h"
#include "../game/systems/npc_controller.h"
#include "../game/systems/physics.h"
#include "../game/systems/player_input.h"
#include "../game/systems/transition_manager.h"
#include "../game/systems/animation_controller.h"


Client::Client() : scene(), scene_renderer(), ui(){}

void Client::Start(){
    logger::info("Initializing client...\n");

    ShaderManager::AddShader("basic_lighting","dat/gfx/basic_lighting.vrt","dat/gfx/basic_lighting.frg");
    ShaderManager::AddShader("shadeless","dat/gfx/shadeless.vrt","dat/gfx/shadeless.frg");
    ShaderManager::AddShader("skybox","dat/gfx/skybox.vrt","dat/gfx/skybox.frg");
    ShaderManager::AddShader("flat_skybox","dat/gfx/flat_skybox.vrt","dat/gfx/flat_skybox.frg");
    ModelManager::Init();
    ModelManager::Register(PLAYER,"dat/models/gargoyle.glb");

    ui.Load();
    scene_renderer.Load();
    scene_renderer.camera.SetShader("basic_lighting");
    
    scene_renderer.camera.ortho=false;
    scene_renderer.camera.z = 10.0f;
    scene_renderer.camera.y = 2.0f;
    
    LoadScene(0);
    if(scene.level.entrance_count > 0){
        SpawnPlayer(scene.level.entrances[0]);
    }
    else{
        Entrance default_spawn;
            default_spawn.name="default";
            default_spawn.position={0,0,0};
            default_spawn.style=0;
            default_spawn.target_pos={0,0,0};
        SpawnPlayer(default_spawn);
    }
}


void Client::LoadScene(int scene_id){
    scene.Load();
    scene_renderer.Add(&scene.level);
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
    my_player = scene.AddEntity(0);
    my_player->type = 1;
    my_player->name= cstr::new_copy("Chowzang");
    my_player->state = new State();
        my_player->state->Set(IDLE);
    my_player->anim_state = new AnimationState(GROUND_UNIT);
    my_player->models = new ModelSet();
        my_player->models->Add(PLAYER);
    my_player->phys_data = new PhysicsData();
    my_player->colliders = new ColliderSet();
        //my_player->colliders->Add();
    my_player->movement = new MovementData();
    my_player->player_data = new PlayerData();
    my_player->camera_target = new CameraTarget(&scene_renderer.camera,{0,4,12},{0,1},{20,40});
    my_player->unit_data = new UnitData();
    PlayerInput::Track(my_player);

    if(my_player->models != null){scene_renderer.Add(my_player->models);}  
}


void Client::Paint(){
    CameraManager::Update(&scene,0);
    scene_renderer.Draw();
    ui.Paint();
}

void Client::UpdatePosition(int ms){
    for(Entity* e:scene.entities){        
        e->x += e->velocity.x *(float)ms/1000.0f;
        e->y += e->velocity.y *(float)ms/1000.0f;
        e->z += e->velocity.z *(float)ms/1000.0f;

        if(e->models != null){
            e->models->x=e->x;
            e->models->y=e->y;
            e->models->z=e->z;
            e->models->rotation=e->rotation;
        }
    }
}

void Client::Update(int ms){
    PlayerInput::Update(&scene,ms);
    AnimationController::Update(&scene,ms);
    //NPCController::Update(&scene,ms);
    Movement::Update(&scene,ms);
    //EntityCollision::Update(&scene,ms);
    //Physics::Update(&scene,ms); 
    //TransitionManager::Update(&scene,ms);
    //Network::RunSync(this,delta);
    UpdatePosition(ms);
    ui.Update(ms,&scene);
}