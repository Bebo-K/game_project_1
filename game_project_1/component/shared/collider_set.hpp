#ifndef COLLIDER_SET_H
#define COLLIDER_SET_H

#include <game_project_1/component/component.hpp>
#include <game_project_1/types/pool.hpp>
#include <game_project_1/phys/collision_types.hpp>
#include <game_project_1/phys/collider.hpp>
#include <game_project_1/gfx/camera.hpp>
#include <game_project_1/gfx/drawable.hpp>

//Component container for hitboxes
class ColliderSet: public Component, public Pool<ShapeCollider>,public Drawable{
    public:
    Ellipse_t   bounds;
    int         entity_collision_handler_id;//handler ID for entity/entity collision

    ColliderSet(Transform* parent);
    ~ColliderSet();

    Component* Clone(ComponentParentContext context);
    virtual void Clear();

    int SerializedLength();
    void Read(Deserializer& dat);
    void Write(Serializer& dat);
    
	virtual void Draw(Camera* cam);
};


#endif