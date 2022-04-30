#ifndef GAME_CONSTANTS_H
#define GAME_CONSTANTS_H

#include <game_project_1/types/str.hpp>
#include <game_project_1/game/stats.hpp>

namespace GameConstants{

    enum Race{Human=0,Golem=1,Impkin=2,Torag=3};
    enum Class{Warrior=0,Archer=1,Mage=2,Thief=3};
    enum EntityClassID{Empty=0,Player=1,PlayerRaceEnemy=2};

    const int STARTING_SCENE=1;
    const int STARTING_SCENE_ENTRANCE=0;
    const int DEFAULT_SCENE=1;

    const int race_count=4;
    extern wchar* race_names[race_count];
    
    const int class_count=4;
    extern wchar* class_names[class_count];

    extern int race_max_style1[race_count];
    extern int race_max_style2[race_count];
    extern int race_max_style3[race_count];

    extern Stats racial_base_stats[race_count];
    extern Stats class_stat_bonus[class_count];

    char* GetMapFileForAreaID(int area_id);
};





#endif