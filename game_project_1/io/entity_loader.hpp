#ifndef ENTITY_LOADER_H
#define ENTITY_LOADER_H

#include <game_project_1/io/json.hpp>
#include <game_project_1/core/entity.hpp>

class EntityLoader{

    private:
    void AssignIfExists(char* prop,float* field);
    
    public:
    JSONObject* e; 
    EntityLoader(JSONObject* entity_json);

    void LoadTo(ServerEntity* entity);

}





#endif