#include <game_project_1/component/shared/collider_set.hpp>
#include <game_project_1/gfx/debugdraw.hpp>


ColliderSet::ColliderSet():bounds(0,0){
    entity_collision_handler_id=0;
    layer=0;
}

ColliderSet::~ColliderSet(){}

int ColliderSet::SerializedLength(){
    return sizeof(float)*2 + sizeof(int)*2 + (Count()*(sizeof(ShapeCollider)));
}

void ColliderSet::Read(Deserializer& dat){
    Clear();
    bounds.radius = dat.GetFloat();
    bounds.height = dat.GetFloat();
    entity_collision_handler_id = dat.GetInt();
    int colliders = dat.GetInt();
    for(int i=0;i<colliders;i++){
        ShapeCollider* c = Add();
        c->shape = (Collider::Shape)dat.GetInt();
        c->center_offset={dat.GetFloat(),dat.GetFloat(),dat.GetFloat()};
        c->scale={dat.GetFloat(),dat.GetFloat(),dat.GetFloat()};
    }
}

void ColliderSet::Write(Serializer& dat){
    dat.PutFloat(bounds.radius);
    dat.PutFloat(bounds.height);
    dat.PutInt(entity_collision_handler_id);
    int num_colliders = Count();
    dat.PutInt(num_colliders);
    for(ShapeCollider* c: *this){
        dat.PutInt(c->shape);
        dat.PutFloat(c->center_offset.x); dat.PutFloat(c->center_offset.y); dat.PutFloat(c->center_offset.z);
        dat.PutFloat(c->scale.x);dat.PutFloat(c->scale.y);dat.PutFloat(c->scale.z);
    }
}

Component* ColliderSet::Clone(){
    ColliderSet* copy = new ColliderSet();
    copy->bounds = bounds;
    copy->entity_collision_handler_id = entity_collision_handler_id;
    for(ShapeCollider* coll:(*this)){
        new (copy->Allocate()) ShapeCollider(coll);
    }
    return copy;
}

void ColliderSet::Clear(){
    this->Pool<ShapeCollider>::Clear();
    bounds.height=0.0f;
    bounds.radius=0.0f;
    entity_collision_handler_id=0;
}

void ColliderSet::Draw(Camera* cam){
    Transform base_transform;
        base_transform.x = x; base_transform.y = y; base_transform.z = z;
        base_transform.rotation.set_euler(rotation.x,rotation.y,rotation.z);
        base_transform.scale=scale;

    Transform center_transform = base_transform;
        center_transform.y += bounds.height/2.0f;
    
    for(ShapeCollider* coll:(*this)){
        Transform collider_transform = center_transform;
        collider_transform.x += coll->center_offset.x;
        collider_transform.y += coll->center_offset.y;
        collider_transform.z += coll->center_offset.z;
        collider_transform.rotation.rotate_by(coll->rotation.x,coll->rotation.y,coll->rotation.z);

        switch(coll->shape){
            case Collider::Shape::AABB: DebugDraw::Draw3DRect(cam,center_transform,
                coll->scale, {0.8f,0.85f,1.0f,0.7f} ); break;
            case Collider::Shape::ARC:break;//?
            case Collider::Shape::CAPSULE: DebugDraw::DrawCapsule(cam,center_transform,
                coll->scale.y,coll->scale.x,{0.8f,0.85f,1.0f,0.7f} ); break;
            case Collider::Shape::SPHERE: DebugDraw::DrawSphere(cam,center_transform,
                coll->scale.x,{0.8f,0.85f,1.0f,0.7f} ); break;
            default: break;
        }
    }

        //DebugDraw::DrawCapsule(cam,center_transform,bounds.height,bounds.radius, {0.5f,0.3f,0.8f,0.3f} );
}