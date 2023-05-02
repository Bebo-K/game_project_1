#include <game_project_1/io/entity_loader.hpp>

#include <game_project_1/types/str.hpp>
#include <game_project_1/component/shared/identity.hpp>

EntityLoader::EntityLoader(JSONObject* entity_json){
    e = entity_json;
}

void EntityLoader::AssignIfExists(char* prop,float* field){
     if(e_pos->HasFloat(prop)){(*field) = e_pos->GetFloat(prop);}
}

EntityLoader::LoadTo(ServerEntity* entity){
    if(e->HasBool("save")){entity->save_to_scene = current_entity->GetBool("save");}
    if(e->HasJObject("pos")){
        JSONObject* e_pos = e->GetJObject("pos");
        AssignIfExists("x",&entity->x);
        AssignIfExists("y",&entity->y);
        AssignIfExists("z",&entity->z);

        AssignIfExists("xr",&entity->rotation.x);
        AssignIfExists("yr",&entity->rotation.y);
        AssignIfExists("zr",&entity->rotation.z);

        AssignIfExists("xs",&entity->scale.x);
        AssignIfExists("ys",&entity->scale.y);
        AssignIfExists("zs",&entity->scale.z);
    }
    if(e->HasJObject("id")){
        JSONObject* id = e->GetJObject("id");
        Identity* identity = new Identity();
        if(id->HasString("name")){identity->name = wchar::from_cstr(id->GetString("name")->string);}
        if(id->HasString("type")){
            identity->type = 0;//TODO: GetEntityClassByName(id->GetString("type")->string);
        }
        entity->Add<Identity>();
    }


}