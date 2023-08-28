#ifndef ENTITY_TEMPLATE_H
#define ENTITY_TEMPLATE_H

#include <game_project_1/core/client_entity.hpp>
#include <game_project_1/core/server_entity.hpp>
#include <game_project_1/server/server_scene.hpp>
#include <game_project_1/client/client_scene.hpp>

typedef void (*ServerEntityBuilder)(ServerEntity*,ServerScene*);
typedef void (*ClientEntityBuilder)(ClientEntity*,ClientScene*);

namespace EntityTemplate{
    extern Map<char*,ServerEntityBuilder> server_entity_builders;
    extern Map<char*,ClientEntityBuilder> client_entity_builders;

    void Register(char* type, ServerEntityBuilder server_builder,ClientEntityBuilder client_builder);
    void Build(char* type,ServerEntity* e,ServerScene* s);
    void Build(char* type,ClientEntity* e,ClientScene* s);
};





#endif