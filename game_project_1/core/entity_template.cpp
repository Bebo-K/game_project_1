#include <game_project_1/core/entity_template.hpp>

Map<char*,ClientEntityBuilder> EntityTemplate::client_entity_builders;
void EmptyClientEntityBuilder(ClientEntity* e,ClientScene* s){}

Map<char*,ServerEntityBuilder> EntityTemplate::server_entity_builders;
void EmptyServerEntityBuilder(ServerEntity* e,ServerScene* s){}

void EntityTemplate::Register(char* type, ServerEntityBuilder server_builder,ClientEntityBuilder client_builder){
    if(!server_entity_builders.Has(type)){server_entity_builders.Add(type,server_builder);}
    if(!client_entity_builders.Has(type)){client_entity_builders.Add(type,client_builder);}
}

void EntityTemplate::Build(char* type, ServerEntity* entity, ServerScene* scene){
    if(!server_entity_builders.Has(type)){logger::warn("Unable to build server entity of type %s, no builder found",type);}
    server_entity_builders.Get(type)(entity,scene);
}

void EntityTemplate::Build(char* type, ClientEntity* entity, ClientScene* scene){
    if(!client_entity_builders.Has(type)){logger::warn("Unable to build client entity of type %s, no builder found",type);}
    client_entity_builders.Get(type)(entity,scene);
}

