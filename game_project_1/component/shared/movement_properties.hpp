#ifndef MOVE_PROPERTIES_H
#define MOVE_PROPERTIES_H

#include <game_project_1/component/component.hpp>


//Information about how an entity can move: move/jump speeds, run/walk, etc. 
class MovementProperties: public Component{
	public:

    float	base_speed;
    float	jump_speed;

    float   jump_hold_boost;

    float	accel_speed;
    float   jump_accel_speed;

    float	multiplier;


    float MaxSpeed();
    float WalkSpeed();

    MovementProperties();
    ~MovementProperties();

    Component* Clone();
    virtual void Clear();

    int SerializedLength();
    void Read(Deserializer& dat);
    void Write(Serializer& dat);
};

#endif