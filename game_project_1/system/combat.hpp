#ifndef COMBAT_H
#define COMBAT_H

#include <game_project_1/game/entity.hpp>

namespace Combat{

    void ServerUpdate(Entity* e, float delta);
    void ClientUpdate(ClientEntity* e, float delta);

};



#endif