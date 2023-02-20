#ifndef COMPONENT_H
#define COMPONENT_H

#include <game_project_1/io/serializer.hpp>
#include <game_project_1/types/3d_types.hpp>


class ClientComponent{
    static ClientComponent* NewInstance(int component_id);
    virtual int ID()=0;
    virtual ~ClientComponent()=0;
};
class ServerComponent{
    static ServerComponent* NewInstance(int component_id);
    virtual int ID()=0;
    virtual ~ServerComponent()=0;
};
class SharedComponent{
    static SharedComponent* NewInstance(int component_id);

    int last_update;
    virtual int ID()=0;
    virtual int SerializedLength()=0;
    virtual void Read(Deserializer& dat)=0;
    virtual void Write(Serializer& dat) =0;

    virtual ~SharedComponent()=0;
};


typedef int EntityClass;
class Identity: public SharedComponent{
    public: 
    const static int ComponentID = 0;

    EntityClass         type;
    wchar*              name;

    Identity(EntityClass type, wchar* name);
    ~Identity();
    void Clear();

    inline int ID(){return Identity::ComponentID;}
    int SerializedLength();
    void Read(Deserializer& dat);
    void Write(Serializer& dat);
}

class Position: public SharedComponent{
    public: 
    const static int ComponentID = 1;

    float               x,y,z;
    vec3                velocity;
    vec3                scale;
    vec3                rotation;

    Position(vec3 pos,vec3 rot,vec3 vel);
    ~Position();
    void Reset();

    inline int ID(){return Position::ComponentID;}
    int SerializedLength();
    void Read(Deserializer& dat);
    void Write(Serializer& dat);
}


class ServerDelta: public ServerComponent{
    public: 
    const static int ServerComponentID = 0;

    bitmask             changed_components;//mask of entity chunks updated since last delta was sent out
    vec3                last_update_position;//position as sent out from last delta
    vec3                last_update_velocity;//velocity as ^
    
    ServerDelta();
    ~ServerDelta();
    void Clear();
    
    inline int ID(){return ServerDelta::ServerComponentID;}
}



#endif