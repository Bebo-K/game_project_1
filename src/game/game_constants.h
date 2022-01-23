#ifndef GAME_CONSTANTS_H
#define GAME_CONSTANTS_H

#include "../struct/str.h"

namespace GameConstants{

    enum Race{Human=0,Golem=1,Impkin=2,Torag=3};
    enum Class{Warrior=0,Archer=1,Mage=2,Thief=3};

    wchar* race_names[] = {L"Human",L"Golem",L"Impkin",L"Other"};
    int race_count=4;
    
    wchar* class_names[] =  {L"Warrior",L"Archer",L"Mage",L"Thief"};
    int class_count=4;




};





#endif