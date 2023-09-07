#include <game_project_1/io/entity_loader.hpp>

#include <game_project_1/types/str.hpp>
#include <game_project_1/core/entity_template.hpp>
#include <game_project_1/component/shared/identity.hpp>

EntityLoader::EntityLoader(JSONObject* entity_json){
    e = entity_json;
}

void AssignIfExists(JSONObject* e_pos,char* prop,float* field){
     if(e_pos->HasFloat(prop)){(*field) = e_pos->GetFloat(prop);}
}

void EntityLoader::LoadTo(ServerEntity* entity,ServerScene* scene){
    if(e->HasString("template")){
        EntityType type = EntityTemplate::FromName(e->GetString("template")->string);
        EntityTemplate::Build(type,entity,scene);
    }
    if(e->HasJObject("pos")){
        JSONObject* e_pos = e->GetJObject("pos");
        AssignIfExists(e_pos,"x",&entity->x);
        AssignIfExists(e_pos,"y",&entity->y);
        AssignIfExists(e_pos,"z",&entity->z);

        AssignIfExists(e_pos,"xr",&entity->rotation.x);
        AssignIfExists(e_pos,"yr",&entity->rotation.y);
        AssignIfExists(e_pos,"zr",&entity->rotation.z);

        AssignIfExists(e_pos,"xs",&entity->scale.x);
        AssignIfExists(e_pos,"ys",&entity->scale.y);
        AssignIfExists(e_pos,"zs",&entity->scale.z);
    }
    if(e->HasJObject("id")){
        JSONObject* id = e->GetJObject("id");
        Identity* identity = new Identity();
        if(id->HasString("name")){identity->name = wstr::from_cstr(id->GetString("name")->string);}
        if(id->HasInt("type")){identity->type = (EntityType)id->GetInt("type");}
        entity->Set(identity);
    }
    //TODO: finish me


}