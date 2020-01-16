#include "scene.h"
#include "../io/log.h"

void Scene::Load(){
    logger::info("loading default scene...");
    renderer.Load();

    defaultSprite = new Sprite((char*)"dat/img/atlas_1.png",1024,1024,1,1,512,912);
    //defaultSprite->y += 100;

    renderer.Add(defaultSprite);

}
void Scene::Load(int area_id){
    logger::info("loading scene for area id %d...",area_id);
    renderer.Load();

}
void Scene::Unload(){
    logger::info("unloading scene...");

    renderer.Remove(defaultSprite);

}
void Scene::Update(int delta){

}
void Scene::Paint(){
    renderer.Draw(); 
}