#ifndef COLLIDER_SET_H
#define COLLIDER_SET_H

#include <game_project_1/component/component.hpp>
#include <game_project_1/types/pool.hpp>
#include <game_project_1/phys/collision_types.hpp>
#include <game_project_1/phys/collider.hpp>


//Component container for hitboxes
class ColliderSet: public Component, public Pool<ShapeCollider>{
    public:
    Ellipse_t bounds;

    ColliderSet();
    ~ColliderSet();

    Component* Clone();
    virtual void Clear();

    int SerializedLength();
    void Read(Deserializer& dat);
    void Write(Serializer& dat);
};


#endif