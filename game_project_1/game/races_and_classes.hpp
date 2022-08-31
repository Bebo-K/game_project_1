#ifndef RACES_AND_CLASSES_H
#define RACES_AND_CLASSES_H

#include <game_project_1/types/str.hpp>
#include <game_project_1/game/stats.hpp>

typedef int RaceID;
struct Race{
    RaceID id;
    wchar* name;
    int max_styles_1;
    int max_styles_2;
    int max_styles_3;
    float hitsphere_height;
    float hitsphere_radius;
    BaseStats stat_base;
};

typedef int ClassID;
struct Class{
    ClassID id;
    wchar* name;
    BaseStats stat_bonus;
};

namespace Races{
    const Race Human = {0,L"Human",3,3,3, 3.0f,0.75f, {12,8,10,9,11}};
    const Race Golem = {1,L"Golem",3,3,3, 3.0f,0.75f, {11,9,12,10,8}};
    const Race Impkin = {2,L"Impkin",3,3,3, 3.0f,0.75f, {10,11,8,9,12}};
    const Race Torag = {3,L"Torag",3,3,3, 3.0f,0.75f, {10,10,10,10,10}};
    extern int Max;
    Race GetRaceByID(RaceID id);
};

namespace Classes{
    const Class Warrior = {0,L"Warrior",{2,0,2,0,0}};
    const Class Archer = {1,L"Archer",{0,1,0,1,2}};
    const Class Mage = {2,L"Mage",{0,2,0,2,0}};
    const Class Thief = {3,L"Thief",{2,0,1,0,2} };
    extern int Max;
    Class GetClassByID(ClassID id);
};


#endif