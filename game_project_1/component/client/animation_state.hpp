#ifndef ANIMATION_STATE_H
#define ANIMATION_STATE_H

#include <game_project_1/component/shared/movement_state.hpp>

typedef int AnimationControllerType;
//State information for coordinating animations for movement/action states
class AnimationState: public Component{
    public:
    MovementType            movement_type;
    int                     action_state;
    char*                   anim_name;//does not allocate/deallocate memory for this 
    AnimationControllerType controller_type;
    
    AnimationState();
    AnimationState(AnimationControllerType type);
    ~AnimationState();

    Component* Clone();
    virtual void Clear();

    inline int SerializedLength(){return 0;}
    inline void Read(Deserializer& dat){}
    inline void Write(Serializer& dat){}
};





#endif