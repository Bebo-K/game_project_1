#ifndef SERVER_LEVEL_H
#define SERVER_LEVEL_H

#include <game_project_1/core/level.hpp>
#include <game_project_1/phys/level_colliders.hpp>
#include <game_project_1/io/level_loader.hpp>



class ServerLevel{
    private:
    static char* default_shader;
    public:

    Array<MeshCollider> collmeshes;
    Array<LevelEntrance> entrances;
    Array<LevelExit> exits;
    Array<LevelTrigger> triggers;

    ServerLevel();
    ~ServerLevel();
    void Load(LevelLoader& loader);
    void Unload();
};




#endif