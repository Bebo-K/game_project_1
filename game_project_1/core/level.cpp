#include <game_project_1/core/level.hpp>


Location LevelEntrance::GenerateLocation(){
    return {pos,{0,y_turn,0},{1,1,1}};
}

LevelExit::LevelExit(){trigger_collider=nullptr;}
LevelExit::~LevelExit(){
    if(trigger_collider != nullptr){free(trigger_collider);trigger_collider=nullptr;}
}

LevelTrigger::LevelTrigger(){trigger_collider=nullptr;}
LevelTrigger::~LevelTrigger(){
    if(trigger_collider != nullptr){free(trigger_collider);trigger_collider=nullptr;}
}