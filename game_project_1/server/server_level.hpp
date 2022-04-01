#ifndef SERVER_LEVEL_H
#define SERVER_LEVEL_H

#include <game_project_1/phys/level_colliders.hpp>

struct LevelEntrance{
    vec3 pos;
    vec3 size;
    int style;
};

struct LevelExit{
    int mesh_index;
    int new_area_id;
    int style;
};

struct LevelTrigger{
    int mesh_index;
    int effect_id;
};


class ServerLevel{
    public:

    int             collmesh_count; 
    CollisionMesh*  collmeshes;

    int             entrance_count;
    LevelEntrance*  entrances;
    int             exit_count;
    LevelExit*      exits;
    int             trigger_count;
    LevelTrigger*   triggers;

    ServerLevel();
    ~ServerLevel();
    void LoadFromStream(Stream* stream);
    void Unload();
};




#endif