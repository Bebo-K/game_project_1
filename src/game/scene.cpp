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
    Entity* entity = entities.New();
    entity->eid=eid;
    return entity;
}

Entity* Scene::GetEntity(int eid){
    for(int i=entities.Begin();entities.Has(i);i=entities.Next(i)){
        if(entities[i]->eid == eid){
            return entities[i];
        }
    }
    return null;
}

void Scene::RemoveEntity(int eid){
    for(int i=entities.Begin();entities.Has(i);i=entities.Next(i)){
        if(entities[i]->eid == eid){
            entities.Delete(i);
            break;
        }
    }
}