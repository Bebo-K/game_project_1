#include <game_project_1/core/level.hpp>
#include <stdlib.h>


LevelExit::LevelExit(){trigger_collider=nullptr;}
LevelExit::~LevelExit(){
    DEALLOCATE(trigger_collider)
}

LevelTrigger::LevelTrigger(){trigger_collider=nullptr;}
LevelTrigger::~LevelTrigger(){
    DEALLOCATE(trigger_collider)
}