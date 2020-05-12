#include "scene.h"
#include "../log.h"

void Scene::Load(){
    logger::info("loading default scene...\n");
    level.LoadDefault();
    loaded = true;
}

void Scene::Load(int area_id){
    logger::info("loading scene for area id %d...\n",area_id);
    loaded = true;
}

void Scene::Unload(){
    logger::info("unloading scene...\n");
    loaded = false;
}

Entity* Scene::AddEntity(int eid){
    Entity* entity = entities.Add();
    entity->eid=eid;
    return entity;
}

Entity* Scene::GetEntity(int eid){
    for(Entity* e:entities){
        if(e->eid== eid){return e;}
    }
    return null;
}

void Scene::RemoveEntity(int eid){
    for(Entity* e:entities){
        if(e->eid== eid){entities.Delete(e);return;}
    }
}