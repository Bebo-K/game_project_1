#include "scene.h"
#include "../io/log.h"

void Scene::Load(){
    logger::info("loading default scene...");
    renderer.Load();
    ShaderManager::AddShader("basic_lighting","dat/gfx/basic_lighting.vrt","dat/gfx/basic_lighting.frg");
    renderer.camera.SetShader(ShaderManager::GetShader("basic_lighting"));

    renderer.camera.ortho=false;

    my_cube = new CubePrimitive("dat/img/atlas_1.png",1,1,1);
    my_cube->rotation.y=45;

    renderer.Add(my_cube);
}
void Scene::Load(int area_id){
    logger::info("loading scene for area id %d...",area_id);
    renderer.Load();

}
void Scene::Unload(){
    logger::info("unloading scene...");
    renderer.Remove(my_cube);
    delete my_cube;
}
void Scene::Update(int delta){

}
void Scene::Paint(){
    renderer.Draw(); 
}