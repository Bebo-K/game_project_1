#include <game_project_1/component/shared/physics_state.hpp>
#include <game_project_1/types/data_types.hpp>

PhysicsState::PhysicsState(){Clear();}

PhysicsState::PhysicsState(PhysicsState* p2){
    in_bounds=p2->in_bounds;
    midair=p2->in_bounds;
}

PhysicsState::~PhysicsState(){}

int PhysicsState::SerializedLength(){
    return sizeof(byte);
}
void PhysicsState::Read(Deserializer& dat){
    byte flags = dat.GetByte();
    in_bounds = (flags & 1)!=0;
    midair = (flags & 2)!=0;
}

void PhysicsState::Write(Serializer& dat){
    byte flags = 0;
    if(in_bounds)flags |=1;
    if(midair)flags |=2;
    dat.PutByte(flags);
}

Component* PhysicsState::Clone(){
    PhysicsState* copy = new PhysicsState();
    copy->in_bounds =in_bounds;
    copy->midair =midair;
    return copy;
}

void PhysicsState::Clear(){
    in_bounds=true;
    midair=false;
}
