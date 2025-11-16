#include <game_project_1/phys/collider.hpp>
#include <game_project_1/types/math_types.hpp>
#include <game_project_1/phys/intersect.h>
#include <game_project_1/gfx/debugdraw.hpp>
#include <game_project_1/types/uuid.hpp>
#include <math.h>


ShapeCollider::ShapeCollider():transform(){
    shape=ColliderShape::SPHERE;
}
ShapeCollider::ShapeCollider(Transform* parent,ShapeCollider* copy):transform(copy->transform){
    shape=copy->shape;
}
ShapeCollider::ShapeCollider(Transform* parent,vec3 center,float r):transform(parent){
    shape=ColliderShape::SPHERE;
    transform.SetPosition(center);
    transform.scale={r,r,r};
    transform.rotation={0,0,0};
}
ShapeCollider::ShapeCollider(Transform* parent,vec3 center,float h, float r):transform(parent){
    shape=ColliderShape::CAPSULE;
    transform.SetPosition(center);
    transform.scale={r,h,r};
    transform.rotation={0,0,0};
}
ShapeCollider::ShapeCollider(Transform* parent,vec3 center,vec3 size):transform(parent){
    shape=ColliderShape::AABB;
    transform.SetPosition(center);
    transform.scale=size;
    transform.rotation={0,0,0};
}
ShapeCollider::ShapeCollider(Transform* parent,vec3 center,float h,float r,float arc_angle,vec3 rotation):transform(parent){
    shape=ColliderShape::ARC;
    transform.SetPosition(center);
    transform.scale={r,h,arc_angle};
    rotation=rotation;
}

bool ShapeCollider::Intersects(ShapeCollider *c2){
    return Intersects(c2,nullptr);
}


bool ShapeCollider::Intersects( ShapeCollider *c2,vec3* intersect_point){
        
    vec3 intersect;

    vec3 c1_origin = transform.GlobalPosition();
    vec3 c2_origin = c2->transform.GlobalPosition();

    vec3 c1_scale = transform.GlobalScale();
    vec3 c2_scale = c2->transform.GlobalScale();

    quaternion c1_rotation = transform.GlobalRotation();
    quaternion c2_rotation = c2->transform.GlobalRotation();

    vec3 offset = c2_origin - c1_origin;

    vec3 size_1 = c1_scale;
    if(shape == ColliderShape::ARC){size_1.z = transform.scale.z;/*z component is arc-length, it doesn't scale*/}
    
    vec3 size_2 = c2_scale;
    if(c2->shape == ColliderShape::ARC){size_2.z = c2->transform.scale.z;/*z component is arc-length, it doesn't scale*/}

    bool result = false;
    switch (shape){//Many TODOs
    case ColliderShape::CYLINDER:
        switch (c2->shape){
        case ColliderShape::CYLINDER:result = CylinderVCylinderIntersect(size_1.x,size_1.y,size_2.x,size_2.y,offset,intersect);break;
        case ColliderShape::CAPSULE:break;
        case ColliderShape::AABB:break;
        case ColliderShape::SPHERE:break;
        case ColliderShape::ARC:break;
        default:break;
        }break;
    case ColliderShape::CAPSULE:
        switch (c2->shape){
        case ColliderShape::CYLINDER:break;
        case ColliderShape::CAPSULE:result = CapsuleVCapsuleIntersect(size_1,size_2,c1_rotation,c2_rotation,offset,intersect);break;
        case ColliderShape::AABB:break;
        case ColliderShape::SPHERE:break;
        case ColliderShape::ARC:break;
        default:break;
        }break;
    case ColliderShape::AABB:
        switch (c2->shape){
        case ColliderShape::CAPSULE:break;
        case ColliderShape::AABB:result = AABBVAABBIntersect(size_1,size_2,offset,intersect);break;
        case ColliderShape::SPHERE:break;
        case ColliderShape::ARC:break;
        default:break;
        }break;
    case ColliderShape::SPHERE:
        switch (c2->shape){
        case ColliderShape::CAPSULE:break;
        case ColliderShape::AABB:break;
        case ColliderShape::SPHERE:result = SphereVSphereIntersect(size_1.x,size_2.x,offset,intersect);break;
        case ColliderShape::ARC:break;
        default:break;
        }break;
    case ColliderShape::ARC:
        switch (c2->shape){
        case ColliderShape::CAPSULE:break;
        case ColliderShape::AABB:break;
        case ColliderShape::SPHERE:break;
        case ColliderShape::ARC:break;
        default:break;
        }break;
    default:break;
    }
    if(intersect_point != nullptr){intersect_point->set(intersect.x,intersect.y,intersect.z);}
    return result;
}


void ShapeCollider::Draw(Camera* cam){
    color unique_color( seeded_rand(uuid(this)) );
    unique_color.a = 128;
    unique_color.r = 128 + (unique_color.r)/2;
    unique_color.g= 128 + (unique_color.g)/2;
    unique_color.b = 128 + (unique_color.b)/2;
    color_f unique_color_f(unique_color);

    vec3 world_pos = transform.GlobalPosition();
    vec3 world_scale = transform.GlobalScale();

    switch (shape)
    {
    case ColliderShape::SPHERE:
        DebugDraw::DrawSphere(cam, world_pos, world_scale.x, unique_color_f);
        break;
    case ColliderShape::AABB:
        DebugDraw::Draw3DRect(cam,world_pos, quaternion::identity(),world_scale, unique_color_f);
        break;
    case ColliderShape::CYLINDER:
    {
        DebugDraw::DrawCylinder(cam,world_pos, world_scale.y, world_scale.x, unique_color_f);
        break;
    }
    case ColliderShape::CAPSULE:
    {
        vec3 cap_a_offset = world_pos + transform.ToGlobalSpace((vec3){0, 1, 0});
        vec3 cap_b_offset = world_pos + transform.ToGlobalSpace((vec3){0, -1, 0});

        //Until we implment oriented capsules, just draw the caps as spheres
        DebugDraw::DrawSphere(cam,cap_a_offset, world_scale.x,unique_color_f);
        DebugDraw::DrawSphere(cam,cap_b_offset, world_scale.x,unique_color_f);
        DebugDraw::DrawLine(cam,cap_a_offset,cap_b_offset,unique_color_f);
        break;
    }
    case ColliderShape::ARC : // TODO:
        break;
    default:
        break;
    }
}