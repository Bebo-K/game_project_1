#include <game_project_1/component/shared/movement_properties.hpp>


MovementProperties::MovementProperties(){
	base_speed = 1.0f;
	jump_speed = 20.0f;

    jump_hold_boost =25.0f;

	accel_speed = 80.0f;
    jump_accel_speed = 20.0f;
    
	multiplier = 1.0f;
}

MovementProperties::~MovementProperties(){}

int MovementProperties::SerializedLength(){
    return sizeof(float)*6;
}

void MovementProperties::Read(Deserializer& dat){
    base_speed = dat.GetFloat();
    jump_speed = dat.GetFloat();
    jump_hold_boost = dat.GetFloat();
    accel_speed = dat.GetFloat();
    jump_accel_speed = dat.GetFloat();
    multiplier = dat.GetFloat();
}
void MovementProperties::Write(Serializer& dat){
    dat.PutFloat(base_speed);
    dat.PutFloat(jump_speed);
    dat.PutFloat(jump_hold_boost);
    dat.PutFloat(accel_speed);
    dat.PutFloat(jump_accel_speed);
    dat.PutFloat(multiplier);
}

Component* MovementProperties::Clone(ComponentParentContext context){
    MovementProperties* copy = new MovementProperties();
    copy->base_speed = base_speed;
    copy->jump_speed = jump_speed;
    copy->jump_hold_boost = jump_hold_boost;
    copy->accel_speed = accel_speed;
    copy->jump_accel_speed = jump_accel_speed;
    copy->multiplier = multiplier;
    return copy;
}

void MovementProperties::Clear(){
    base_speed = 0;
    jump_speed = 0;
    jump_hold_boost = 0;
    accel_speed = 0;
    jump_accel_speed = 0;
    multiplier = 0;
}


float MovementProperties::MaxSpeed(){return base_speed*multiplier;}
float MovementProperties::WalkSpeed(){return base_speed*multiplier*0.25;}