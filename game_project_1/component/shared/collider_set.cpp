#include <game_project_1/component/shared/collider_set.hpp>
#include <game_project_1/gfx/debugdraw.hpp>


ColliderSet::ColliderSet(Transform* parent):bounds(0,0),Drawable(parent){
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
        c->shape = (ColliderShape)dat.GetInt();
        dat.ReadTransform(&c->transform);
    }
}

void ColliderSet::Write(Serializer& dat){
    dat.PutFloat(bounds.radius);
    dat.PutFloat(bounds.height);
    dat.PutInt(entity_collision_handler_id);
    int num_colliders = Count();
    dat.PutInt(num_colliders);
    for(ShapeCollider* c: *this){
        dat.PutInt((int)c->shape);
        dat.WriteTransform(&c->transform);
    }
}

Component* ColliderSet::Clone(ComponentParentContext context){
    ColliderSet* copy = new ColliderSet(context.transform);
    copy->bounds = bounds;
    copy->entity_collision_handler_id = entity_collision_handler_id;
    for(ShapeCollider* coll:(*this)){
        ShapeCollider* new_coll = new (copy->Allocate()) ShapeCollider(&copy->offset,coll);
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
    vec3 world_sphere_center = offset.GlobalPosition() + vec3{0.0f,bounds.radius,0.0f};
    DebugDraw::DrawEllipse(cam,world_sphere_center,bounds.height,bounds.radius,color_f(0.0f,1.0f,0.0f,0.3f));
    for(ShapeCollider* coll:(*this)){
        coll->Draw(cam);
    }
}