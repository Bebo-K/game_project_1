#ifndef COMPONENT_H
#define COMPONENT_H

#include <game_project_1/types/data_types.hpp>

class Component{
    public:
    virtual ~Component();
    byte* Serialize(int* size);
    void  Deserialize(byte* data,int size);

};


#endif