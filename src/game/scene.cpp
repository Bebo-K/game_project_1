#include "scene.h"
#include "../io/gltf.h"
#include "../log.h"
#include "../input.h"

void Scene::Load(){
    logger::info("loading default scene...\n");
    renderer.Load();
    ShaderManager::AddShader("basic_lighting","dat/gfx/basic_lighting.vrt","dat/gfx/basic_lighting.frg");
    renderer.camera.SetShader(ShaderManager::GetShader("basic_lighting"));

    renderer.camera.ortho=false;
    renderer.camera.y += 2.0f;




    //"dat/img/atlas_1.png"
    //my_cube = new WirePrimitive(CUBE,{.4,1.0,.4},2,1,1);
    //my_cube->rotation.y=45
    //renderer.Add(my_cube);
    
    
    //File modelFile("dat/models/placeholder_person.glb");
    //GLTFScene gltf_file(modelFile);
    //AnimationOptions options;
    //options.timescale = 0.05f;
    //options.end_action = LOOP;
    //my_model= gltf_file.LoadAsModel("person");
    //my_model->skeleton->StartAnimation("Run",options);
    //renderer.Add(my_model);
}
void Scene::Load(int area_id){
    logger::info("loading scene for area id %d...\n",area_id);
    renderer.Load();

}
void Scene::Unload(){
    logger::info("unloading scene...\n");

    renderer.Unload();
}
void Scene::Update(int delta){
	//PlayerActionManager::Update(this,ms);
    //NPCMovementManager::Update(this,ms);
	//MovementController::Update(this,ms);
	//Physics::Update(this,ms);
	//CameraManager::Update(this,ms);
}

void Scene::Paint(){
    renderer.Draw(); 
}