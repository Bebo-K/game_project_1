#include "scene.h"
#include "../io/log.h"

Sprite* defaultSprite;
void Scene::Load(){
    logger::info("loading default scene...");

    defaultSprite = new Sprite("dat/img/atlas_1.png",1024,1024,1,1);

    renderer.Add(defaultSprite);

}
void Scene::Load(int area_id){
    logger::info("loading scene for area id %d...",area_id);

}
void Scene::Unload(){
    logger::info("unloading scene...");

    renderer.Remove(defaultSprite);

}
void Scene::Update(int delta){

}
void Scene::Paint(){

}