#ifndef PERSISTENCE_H
#define PERSISTENCE_H

#include <game_project_1/core/savefile.hpp>
#include <game_project_1/component/component.hpp>

class Persistence: public Component{

    SaveEntity* save_entity;

    Persistence(SaveEntity* e);
    ~Persistence();

    Component* Clone();
    void Clear();

    //Cannot be serialized as it's a circular reference
    inline int SerializedLength(){return 0;}
    inline void Read(Deserializer& dat){}
    inline void Write(Serializer& dat){}
};



#endif