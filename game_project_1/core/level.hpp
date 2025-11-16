#ifndef LEVEL_H
#define LEVEL_H

#include <game_project_1/types/primitives.hpp>
#include <game_project_1/types/math_types.hpp>


enum class LevelEntranceStyle{
    DEFAULT = 0,
    PATH_TO = 1,       //entrance_param = path to this entity ID's location
    WITH_VELOCITY = 2 //entrance_param = copy velocity of this entity ID
};

enum class LevelExitStyle{
    DEFAULT = 0,    
    PATH_TO = 1,    //exit_param = path to this entity ID's location
    USE_DOORWAY = 2 //exit_param = doorway entity ID
};

struct LevelEntrance{
    vec3 pos;
    vec3 size;
    float y_turn;
    //float y_turn deviation;
    LevelEntranceStyle style;
    int entrance_param;
};

struct LevelExit{
    char* trigger_collider;
    int mesh_index;
    int new_area_id;
    int entrance_id;
    LevelExitStyle style;
    int exit_param;
    LevelExit();
    ~LevelExit();
};

struct LevelTrigger{
    char* trigger_collider;
    int mesh_index;
    int effect_id;

    LevelTrigger();
    ~LevelTrigger();
};



#endif