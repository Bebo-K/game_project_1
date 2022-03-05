#include <game_project_1/game/scene.hpp>
#include <game_project_1/log.hpp>

Scene::Scene() :entities(){
    area_id=-1;
    loaded=false;
    global_timer=0;
}

Scene::~Scene(){}

void Scene::Unload(){
    logger::info("unloading scene...\n");
    level.Unload();
    entities.Clear();
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