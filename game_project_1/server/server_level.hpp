#ifndef SERVER_LEVEL_H
#define SERVER_LEVEL_H

#include <game_project_1/io/level.hpp>



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
    void LoadArea(int area_id);
    void Unload();
};




#endif