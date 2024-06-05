#ifndef ENTITY_TEMPLATE_H
#define ENTITY_TEMPLATE_H

#include <game_project_1/core/client_entity.hpp>
#include <game_project_1/core/server_entity.hpp>
#include <game_project_1/server/server_scene.hpp>
#include <game_project_1/client/client_scene.hpp>

typedef void (*ServerEntityBuilder)(ServerEntity*,ServerScene*);
typedef void (*ClientEntityBuilder)(ClientEntity*,ClientScene*);


namespace EntityTemplate{
    extern Dictionary<char*,int> template_ids;
    extern Dictionary<EntityType,ServerEntityBuilder> server_entity_builders;
    extern Dictionary<EntityType,ClientEntityBuilder> client_entity_builders;

    void Register(char* name, EntityType id);
    EntityType FromName(char* name);

    void RegisterBuilders(EntityType type, ServerEntityBuilder server_builder,ClientEntityBuilder client_builder);
    void Build(EntityType type,ServerEntity* e,ServerScene* s);
    void Build(EntityType type,ClientEntity* e,ClientScene* s);
};





#endif