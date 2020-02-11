#ifndef SKELETON_H
#define SKELETON_H

#include "types.h"

//https://www.youtube.com/watch?v=aBN_dEA4jO0



struct Bone{
    char* name;
    int parent_index;
    int child_count;
    int* child_indices;
    mat4 bind_transform;
};

struct Skeleton{
    int         bone_count;
	Bone*       bones;//Pointer shared across clones
	mat4*       inverse_bind_mats;//Pointer shared across clones
    Transform*  pose_transforms;
	mat4*       pose_matrices;//matrix form of bones transform.
	//Map<String,Animation> Animations=null; Shared across clones

    Skeleton();
    ~Skeleton();

    void Setup(int num_bones);

    Skeleton* Clone();
    void DestroySharedData();
    void CalculatePose();
};



#endif