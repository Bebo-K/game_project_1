
#include <game_project_1/component/shared/physics_properties.hpp>


PhysicsProperties::PhysicsProperties():world_hitsphere(1.0f,1.0f){}
PhysicsProperties::~PhysicsProperties(){}

int PhysicsProperties::SerializedLength(){return sizeof(float)*5 + sizeof(int) + 1;}

void PhysicsProperties::Read(Deserializer& dat){
    world_hitsphere.radius=dat.GetFloat();
    world_hitsphere.height=dat.GetFloat();
    midair_velocity_damper=dat.GetFloat();  
    ground_velocity_damper=dat.GetFloat();   
    y_velocity_damper=dat.GetFloat();
    collision_type = dat.GetInt();   

    byte flags =dat.GetByte();
    world_collision_enabled = (flags & 1) > 0;
    apply_gravity = (flags & 2) > 0;
    lock_rotation = (flags & 4) > 0;
    dampen_velocity = (flags & 8) > 0;
}

void PhysicsProperties::Write(Serializer& dat){
    dat.PutFloat(world_hitsphere.radius);
    dat.PutFloat(world_hitsphere.height);
    dat.PutFloat(midair_velocity_damper);  
    dat.PutFloat(ground_velocity_damper);   
    dat.PutFloat(y_velocity_damper); 
    dat.PutInt(collision_type);  

    byte flags =0;
    if(world_collision_enabled){flags |= 1;}
    if(apply_gravity){flags |= 2;}
    if(lock_rotation){flags |= 4;}
    if(dampen_velocity){flags |= 8;}
    dat.PutByte(flags);
}

Component* PhysicsProperties::Clone(ComponentParentContext context){
    PhysicsProperties* copy = new PhysicsProperties();
    copy->world_hitsphere = world_hitsphere;
    copy->collision_type = collision_type;
	copy->world_collision_enabled = world_collision_enabled;
	copy->apply_gravity = apply_gravity;
	copy->lock_rotation = lock_rotation;
    copy->dampen_velocity = dampen_velocity;
	copy->midair_velocity_damper = midair_velocity_damper;
	copy->ground_velocity_damper = ground_velocity_damper;
	copy->y_velocity_damper = y_velocity_damper;
    return copy;
}

void PhysicsProperties::Clear(){
    world_hitsphere.height=world_hitsphere.radius=1.0f;
    world_collision_enabled=false;
    collision_type=0;
    apply_gravity=false;
    lock_rotation=false;
    dampen_velocity=false;
    midair_velocity_damper=0.0f;
    ground_velocity_damper=0.0f;
    y_velocity_damper=0.0f;
}

