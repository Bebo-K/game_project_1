#ifndef ENTITY_HUMANOID_H
#define ENTITY_HUMANOID_H

#include <game_project_1/game/entity.hpp>
#include <game_project_1/client/client_scene.hpp>
#include <game_project_1/server/server_scene.hpp>


void HumanoidClientBuilder(ClientEntity* entity, ClientScene* scene);
void HumanoidServerBuilder(ServerEntity* entity, ServerScene* scene);
void HumanoidLevelClientCollisionHandler(ClientEntity* entity,LevelCollision::CollisionResult collision,ClientScene* scene);
void HumanoidLevelServerCollisionHandler(ServerEntity*entity,LevelCollision::CollisionResult collision,ServerScene* scene);
void HumanoidEntityClientCollisionHandler(ClientEntity* entity,ShapeCollider*c1,
    ClientEntity* entity_2,ShapeCollider* c2,ClientScene* scene,vec3 point);
void HumanoidEntityServerCollisionHandler(ServerEntity* entity,ShapeCollider* c1,
     ServerEntity* entity_2,ShapeCollider* c2,ServerScene* scene,vec3 point);
     
#endif