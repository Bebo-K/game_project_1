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

    ShapeCollider* AddEmptyCollider();
    ShapeCollider* AddSphereCollider(vec3 center,float r);
    ShapeCollider* AddCapsuleCollider(vec3 center,float h, float r);
    ShapeCollider* AddAABBCollider(vec3 center,vec3 size);
    ShapeCollider* AddArcCollider(vec3 center,float h,float r,float arc_angle,vec3 rotation);
    
	virtual void Draw(Camera* cam);
};


#endif