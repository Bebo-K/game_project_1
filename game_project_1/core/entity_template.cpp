#include <game_project_1/core/entity_template.hpp>

Dictionary<EntityType,ClientEntityBuilder> EntityTemplate::client_entity_builders;
void EmptyClientEntityBuilder(ClientEntity* e,ClientScene* s){}

Dictionary<EntityType,ServerEntityBuilder> EntityTemplate::server_entity_builders;
void EmptyServerEntityBuilder(ServerEntity* e,ServerScene* s){}

Dictionary<char*,int> EntityTemplate::template_ids;

void EntityTemplate::Register(char* name, EntityType id){template_ids.Add(name,id);}
EntityType EntityTemplate::FromName(char* name){
    if(!template_ids.Has(name)){
        logger::warn("Entity template of name %s does not correspond to a registered type\n",name);
        return 0;    
    }
    return template_ids.Get(name);
}

void EntityTemplate::RegisterBuilders(EntityType type, ServerEntityBuilder server_builder,ClientEntityBuilder client_builder){
    if(!server_entity_builders.Has(type)){server_entity_builders.Add(type,server_builder);}
    if(!client_entity_builders.Has(type)){client_entity_builders.Add(type,client_builder);}
}

void EntityTemplate::Build(EntityType type, ServerEntity* entity, ServerScene* scene){
    if(!server_entity_builders.Has(type)){
        logger::warn("Unable to build server entity of type %d, no builder found",type);
        return;
    }
    server_entity_builders.Get(type)(entity,scene);
}

void EntityTemplate::Build(EntityType type, ClientEntity* entity, ClientScene* scene){
    if(!client_entity_builders.Has(type)){logger::warn("Unable to build client entity of type %d, no builder found",type);}
    client_entity_builders.Get(type)(entity,scene);
}

