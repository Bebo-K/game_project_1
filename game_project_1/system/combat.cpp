#include <game_project_1/system/combat.hpp>
#include <game_project_1/component/shared/collider_set.hpp>
#include <math.h>


void Combat::ServerUpdate(Entity* e, float delta){
    if(!e->Has<ColliderSet>()){return;}
     

}

void Combat::ClientUpdate(ClientEntity* e, float delta){
    if(!e->Has<ColliderSet>()){return;}
     

}
