#ifndef LOGIC_COMPONENTS_H
#define LOGIC_COMPONENTS_H

#include <game_project_1/component/component.hpp>



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