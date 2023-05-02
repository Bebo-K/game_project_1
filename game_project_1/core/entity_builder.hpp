#ifndef SERVER_ENTITY_BUILDER_H
#define SERVER_ENTITY_BUILDER_H

#include <game_project_1/core/entity.hpp>
#include <game_project_1/server/server_scene.hpp>
#include <game_project_1/client/client_scene.hpp>

typedef void (*ServerEntityBuilder)(ServerEntity*,ServerScene*);
typedef void (*ClientEntityBuilder)(ClientEntity*,ClientScene*);

namespace EntityBuilder{

    extern Map<int,ServerEntityBuilder> server_entity_builders;
    extern Map<int,ClientEntityBuilder> client_entity_builders;

    void RegisterServerEntityBuilder(EntityClass type, ServerEntityBuilder builder);
    ServerEntityBuilder GetServerEntityBuilder(EntityClass type);

    void RegisterClientEntityBuilder(EntityClass type, ClientEntityBuilder builder);
    ClientEntityBuilder GetClientEntityBuilder(EntityClass type);

    void Build(ServerEntity* entity, ServerScene* scene);
    void Build(ClientEntity* entity, ClientScene* scene);
}





#endif