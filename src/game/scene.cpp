#include "scene.h"
#include "../io/log.h"

Scene::Scene(){
    renderer = Renderer();

}

void Scene::Load(){
    logger::info("loading default scene...");

}
void Scene::Load(int area_id){
    logger::info("loading scene for area id %d...",area_id);

}
void Scene::Unload(){
    logger::info("unloading scene...");

}
void Scene::Update(int delta){

}
void Scene::Paint(){

}