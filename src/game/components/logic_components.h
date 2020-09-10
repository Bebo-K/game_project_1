#ifndef LOGIC_COMPONENTS_H
#define LOGIC_COMPONENTS_H

#include "component.h"



class PlayerData : Component{
    public:

    
    ~PlayerData();
};
class NPCData : Component{
    public:
    int patience=483;

    ~NPCData();
};
class UnitData : Component{
    public:


    ~UnitData();
};


#endif