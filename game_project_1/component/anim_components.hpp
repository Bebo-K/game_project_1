#ifndef ANIM_COMPONENTS_H
#define ANIM_COMPONENTS_H

#include <game_project_1/component/component.hpp>
#include <game_project_1/io/serializer.hpp>

typedef int AnimationControllerType;


class AnimationState: public ClientComponent{
    public:
	int ClientComponentID=2;

    int                     movement_state;
    int                     action_state;
    char*                   anim_name;
    AnimationControllerType controller_type;
    
	inline int ID(){return ModelSet::ClientComponentID;}
    AnimationState(AnimationControllerType type);
    ~AnimationState();
};

#endif