#include "client.h"
#include "../log.h"
#include <stdio.h>

Client::Client() : scene(), scene_renderer(), ui(){}

void Client::Start(){
    logger::info("Initializing client...\n");

    ShaderManager::AddShader("basic_lighting","dat/gfx/basic_lighting.vrt","dat/gfx/basic_lighting.frg");
    ShaderManager::AddShader("shadeless","dat/gfx/shadeless.vrt","dat/gfx/shadeless.frg");

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
    my_player->models = new ModelSet();
    //Model Registry add Player model
    //    my_player->models->Add("dat/models/placeholder_person.glb");
    //if(e->models != null){scene_renderer.Add(e->models);}    

}


void Client::Paint(){
    scene_renderer.Draw();
    ui.Paint();
}

void Client::Update(int ms){
	//PlayerActionManager::Update(this,delta);
    //NPCMovementManager::Update(this,ms);
	//MovementController::Update(this,delta);
	//Physics::Update(this,delta);
	//CameraManager::Update(this,delta);

    //Network::RunSync(this,delta);
    ui.Update(ms,&scene);
}