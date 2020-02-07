#include "scene.h"
#include "../io/gltf.h"
#include "../log.h"

void Scene::Load(){
    logger::info("loading default scene...");
    renderer.Load();
    ShaderManager::AddShader("basic_lighting","dat/gfx/basic_lighting.vrt","dat/gfx/basic_lighting.frg");
    renderer.camera.SetShader(ShaderManager::GetShader("basic_lighting"));

    renderer.camera.ortho=false;
    //"dat/img/atlas_1.png"
    //my_cube = new WirePrimitive(CUBE,{.4,1.0,.4},2,1,1);
    //my_cube->rotation.y=45
    //renderer.Add(my_cube);
    File modelFile("dat/models/placeholder_person.glb");
    GLTFScene gltf_file(modelFile);

    my_model=new Model();
    gltf_file.GetModels(my_model,1);


    renderer.Add(my_model);
}
void Scene::Load(int area_id){
    logger::info("loading scene for area id %d...",area_id);
    renderer.Load();

}
void Scene::Unload(){
    logger::info("unloading scene...");
    renderer.Remove(my_model);
    delete my_model;
}
void Scene::Update(int delta){
my_model->rotation.y += delta /10;
}
void Scene::Paint(){
    renderer.Draw(); 
}