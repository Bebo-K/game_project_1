#ifndef COMPONENT_H
#define COMPONENT_H

#include "../structs/data_types.h"

class Component{
    public:
    virtual ~Component();
    byte* Serialize(int* size);
    void  Deserialize(byte* data,int size);

};


#endif