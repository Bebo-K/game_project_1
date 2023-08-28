#ifndef ENTITY_LOADER_H
#define ENTITY_LOADER_H

#include <game_project_1/io/json.hpp>
#include <game_project_1/core/server_entity.hpp>
#include <game_project_1/server/server_scene.hpp>

class EntityLoader{

    public:
    JSONObject* e; 
    EntityLoader(JSONObject* entity_json);

    void LoadTo(ServerEntity* entity,ServerScene* scene);

};





#endif