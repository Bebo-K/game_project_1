#ifndef SKELETON_H
#define SKELETON_H

#include <game_project_1/types/primitives.hpp>
#include <game_project_1/types/math_types.hpp>
#include <game_project_1/gfx/animation.hpp>
#include <game_project_1/types/arrays.hpp>
#include <game_project_1/types/transform.hpp>
#include <game_project_1/gfx/animation.hpp>

//https://www.youtube.com/watch?v=aBN_dEA4jO0



struct Bone{
    char* name;
    int parent_index;
    mat4 bind_transform;
    
    void DebugPrint();
};

//Shared armature info across model instances
class Skeleton{
    public:
    Array<Bone>             bones;
    Array<Animation::Clip>  animations;
	mat4*                   inverse_bind_mats;
     
    Skeleton(int num_bones);
    ~Skeleton();

    void SetBoneName(int bone_id,char* bone_name);
    void DebugPrint();

    Animation::Clip* GetAnimation(char* name);
};

//Instance of a skeleton that tracks bone transforms for a model instance
class Pose{
    public:
    int               bone_count;
    Transform*        transforms;
    mat4*             matrices;
    Skeleton*         skeleton;
    Animation::Target animation_target;


    Pose(Transform* parent, Skeleton* target);
    ~Pose();
    void StartAnimation(char* name);
    void StopAnimations();
    void QueueAnimation(char* name);
    void Calculate();//sets matrices;
};



#endif