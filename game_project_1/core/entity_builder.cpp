#include <game_project_1/core/entity_builder.hpp>


Map<EntityClass,ClientEntityBuilder> EntityBuilder::client_entity_builders;
void EmptyClientEntityBuilder(ClientEntity* e,ClientScene* s){}

Map<EntityClass,ServerEntityBuilder> EntityBuilder::server_entity_builders;
void EmptyServerEntityBuilder(ServerEntity* e,ServerScene* s){}

void EntityBuilder::RegisterClientEntityBuilder(EntityClass type, ClientEntityBuilder builder){
    if(!client_entity_builders.Has(type)){client_entity_builders.Add(type,builder);}
}

ClientEntityBuilder EntityBuilder::GetClientEntityBuilder(EntityClass type){
    return (client_entity_builders.Has(type))? client_entity_builders.Get(type) : &EmptyClientEntityBuilder;
}

void EntityBuilder::Build(ClientEntity* entity, ClientScene* scene){
    if(entity->Has<Identity>()){
        GetClientEntityBuilder(entity->Get<Identity>()->type)(entity,scene);
    }
}

void EntityBuilder::RegisterServerEntityBuilder(EntityClass type, ServerEntityBuilder builder){
    if(!server_entity_builders.Has(type)){server_entity_builders.Add(type,builder);}
}

ServerEntityBuilder EntityBuilder::GetServerEntityBuilder(EntityClass type){
    return (server_entity_builders.Has(type))? server_entity_builders.Get(type) : &EmptyServerEntityBuilder;
}

void EntityBuilder::Build(ServerEntity* entity, ServerScene* scene){
    if(entity->Has<Identity>()){
        GetServerEntityBuilder(entity->Get<Identity>()->type)(entity,scene);
    }
}

