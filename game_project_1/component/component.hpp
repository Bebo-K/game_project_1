#ifndef COMPONENT_H
#define COMPONENT_H

#include <game_project_1/io/serializer.hpp>
#include <game_project_1/types/3d_types.hpp>
#include <game_project_1/types/list.hpp>
#include <game_project_1/log.hpp>
#include <typeinfo>


class Component{
    public: 

    int last_updated;
    inline void MarkUpdated(int timestamp){last_updated=timestamp;}
    inline void MarkUpdated(){last_updated=OS::time_ms();}

    virtual ~Component()=0;

    virtual Component* Clone() =0;
    virtual void Clear()=0;

    virtual int SerializedLength()=0;
    virtual void Read(Deserializer& dat)=0;
    virtual void Write(Serializer& dat)=0;
};


#endif