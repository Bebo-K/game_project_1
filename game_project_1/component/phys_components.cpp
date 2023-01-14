#include <game_project_1/component/phys_components.hpp>

PhysBody::PhysBody():world_hitsphere(1.0f,1.0f){}
PhysBody::~PhysBody(){}
bool PhysBody::IsInBounds(){return !out_of_bounds;}
void PhysBody::SetInBounds(bool inside){out_of_bounds=!inside;}
bool PhysBody::IsMidair(){return is_midair;}
void PhysBody::SetMidair(bool midair){is_midair=midair;}
int PhysBody::SerializedLength(){return sizeof(float)*5 + 1;}
void PhysBody::Read(Deserializer& dat){
    world_hitsphere.radius=dat.GetFloat();
    world_hitsphere.height=dat.GetFloat();
    midair_velocity_damper=dat.GetFloat();  
    ground_velocity_damper=dat.GetFloat();   
    y_velocity_damper=dat.GetFloat();   

    byte flags =dat.GetByte();
    world_collision_enabled = (flags & 1) > 0;
    is_midair = (flags & 2) > 0;
    apply_gravity = (flags & 4) > 0;
    lock_rotation = (flags & 8) > 0;
    dampen_velocity = (flags & 16) > 0;
    out_of_bounds= (flags & 32) > 0;
}
void PhysBody::Write(Serializer& dat){
    dat.PutFloat(world_hitsphere.radius);
    dat.PutFloat(world_hitsphere.height);
    dat.PutFloat(midair_velocity_damper);  
    dat.PutFloat(ground_velocity_damper);   
    dat.PutFloat(y_velocity_damper);   

    byte flags =0;
    if(world_collision_enabled){flags |= 1;}
    if(is_midair){flags |= 2;}
    if(apply_gravity){flags |= 4;}
    if(lock_rotation){flags |= 8;}
    if(dampen_velocity){flags |= 16;}
    if(out_of_bounds){flags |= 32;}
    dat.PutByte(flags);
}
void PhysBody::Copy(PhysBody* p2){
    world_hitsphere=p2->world_hitsphere;
	world_collision_enabled = p2->world_collision_enabled;
	apply_gravity = p2->apply_gravity;
	lock_rotation = p2->lock_rotation;
    dampen_velocity = p2->dampen_velocity;
	midair_velocity_damper = p2->midair_velocity_damper;
	ground_velocity_damper = p2->ground_velocity_damper;
	y_velocity_damper = p2->y_velocity_damper;
	is_midair	= p2->is_midair;
    out_of_bounds = p2->out_of_bounds;
}


ColliderSet::~ColliderSet(){}
int ColliderSet::SerializedLength(){
    return sizeof(float)*2 + sizeof(int) + (Count()*(sizeof(int)+sizeof(float)*7));
}
void ColliderSet::Read(Deserializer& dat){
    bounds_xz = dat.GetFloat();
    bounds_y = dat.GetFloat();
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
    dat.PutFloat(bounds_xz);
    dat.PutFloat(bounds_y);
    int num_colliders = Count();
    dat.PutInt(num_colliders);
    for(ShapeCollider* c: *this){
        dat.PutInt(c->shape);
        dat.PutFloat(c->center_offset.x); dat.PutFloat(c->center_offset.y); dat.PutFloat(c->center_offset.z);
        dat.PutFloat(c->scale.x);dat.PutFloat(c->scale.y);dat.PutFloat(c->scale.z);
        dat.PutFloat(c->turn_offset);
    }
}
void ColliderSet::Copy(ColliderSet* c2){
    for(ShapeCollider* coll2:(*c2)){
        new (Allocate()) ShapeCollider(coll2);
    }
}

MoveParams::MoveParams(){
	base_speed = 1.0f;
	jump_speed = 20.0f;

    jump_hold_boost =25.0f;

	accel_speed = 80.0f;
    jump_accel_speed = 20.0f;
    
	multiplier = 1.0f;
}
MoveParams::~MoveParams(){}
int MoveParams::SerializedLength(){
    return sizeof(float)*6;
}
void MoveParams::Read(Deserializer& dat){
    base_speed = dat.GetFloat();
    jump_speed = dat.GetFloat();
    jump_hold_boost = dat.GetFloat();
    accel_speed = dat.GetFloat();
    jump_accel_speed = dat.GetFloat();
    multiplier = dat.GetFloat();
}
void MoveParams::Write(Serializer& dat){
    dat.PutFloat(base_speed);
    dat.PutFloat(jump_speed);
    dat.PutFloat(jump_hold_boost);
    dat.PutFloat(accel_speed);
    dat.PutFloat(jump_accel_speed);
    dat.PutFloat(multiplier);
}
void MoveParams::Copy(MoveParams* p2){
    base_speed=p2->base_speed;
    jump_speed=p2->jump_speed;
    jump_hold_boost=p2->jump_hold_boost;
    accel_speed=p2->accel_speed;
    jump_accel_speed=p2->jump_accel_speed;
    multiplier=p2->multiplier;
}

float MoveParams::MaxSpeed(){return base_speed*multiplier;}