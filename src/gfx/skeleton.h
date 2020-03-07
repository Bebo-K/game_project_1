#ifndef SKELETON_H
#define SKELETON_H

#include "types.h"
#include "animation.h"

//https://www.youtube.com/watch?v=aBN_dEA4jO0



struct Bone{
    char* name;
    int parent_index;
    int child_count;
    int* child_indices;
    mat4 bind_transform;
};

class Skeleton{
    public:
    int         bone_count;
	Bone*       bones;//Pointer shared across clones
	mat4*       inverse_bind_mats;//Pointer shared across clones
    Transform*  pose_transforms;
	mat4*       pose_matrices;//matrix form of bones transform.
    int         animation_count;
    Animation*  animations;//Pointer shared across clones
    AnimationHook pose_hook;
     
    Skeleton();
    ~Skeleton();

    void AllocateBoneCount(int num_bones);
    void SetBoneName(int bone_id,char* bone_name);

    Skeleton* Clone();
    void DestroySharedData();
    void CalculatePose();

    Animation* GetAnimation(char* name);
    void StartAnimation(char* name);

};



#endif