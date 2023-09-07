#ifndef SERVER_LEVEL_H
#define SERVER_LEVEL_H

#include <game_project_1/core/level.hpp>
#include <game_project_1/phys/level_colliders.hpp>
#include <game_project_1/io/level_loader.hpp>


class ServerLevel{
    private:
    static char* default_shader;
    public:

    List<MeshCollider> collmeshes;
    List<LevelEntrance> entrances;
    List<LevelExit> exits; 
    List<LevelTrigger> triggers;

    ServerLevel();
    ~ServerLevel();
    void Load(LevelLoader& loader);
    void Unload();
};




#endif