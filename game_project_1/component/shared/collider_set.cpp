#include <game_project_1/component/shared/collider_set.hpp>


ColliderSet::ColliderSet():bounds(0,0){
    entity_collision_handler_id=0;
}

ColliderSet::~ColliderSet(){}

int ColliderSet::SerializedLength(){
    return sizeof(float)*2 + sizeof(int)*2 + (Count()*(sizeof(ShapeCollider)));
}

void ColliderSet::Read(Deserializer& dat){
    bounds.radius = dat.GetFloat();
    bounds.height = dat.GetFloat();
    entity_collision_handler_id = dat.GetInt();
    Clear();
    int colliders = dat.GetInt();
    for(int i=0;i<colliders;i++){
        ShapeCollider* c = Add();
        c->shape = (Collider::Shape)dat.GetInt();
        c->center_offset={dat.GetFloat(),dat.GetFloat(),dat.GetFloat()};
        c->scale={dat.GetFloat(),dat.GetFloat(),dat.GetFloat()};
        c->turn_offset=dat.GetFloat();
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
        dat.PutFloat(c->turn_offset);
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

