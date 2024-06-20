#ifndef COMBAT_H
#define COMBAT_H


#include <game_project_1/client/client_scene.hpp>
#include <game_project_1/server/server_scene.hpp>

#include <game_project_1/phys/collider.hpp>
#include <game_project_1/gfx/animation.hpp>
#include <game_project_1/component/client/hitboxes.hpp>


typedef int AttackType;

HitPattern* GetHitPatternForAttackType(Entity* e, AttackType type);
char* GetAnimationForAttackType(Entity* e, AttackType type);





namespace Combat{

    void ClientStartAttack(ClientEntity* e, ClientScene* s);
    void ServerStartAttack(ServerEntity* e, ServerScene* s);
    void ServerFrame(Entity* e, Timestep delta);
    void ClientFrame(ClientEntity* e, Timestep delta);

};



#endif