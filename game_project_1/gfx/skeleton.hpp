#ifndef SKELETON_H
#define SKELETON_H

#include <game_project_1/types/3d_types.hpp>
#include <game_project_1/gfx/animation.hpp>

//https://www.youtube.com/watch?v=aBN_dEA4jO0



struct Bone{
    char* name;
    int parent_index;
    mat4 bind_transform;
    
    void DebugPrint();
};

class Skeleton{
    public:
    int         bone_count;
	Bone*       bones;//Pointer shared across clones
	mat4*       inverse_bind_mats;//Pointer shared across clones
    int         animation_count;
    Animation*  animations;//Pointer shared across clones
    //Transform*  pose_transforms;
	//mat4*       pose_matrices;//matrix form of bones transform.
     
    Skeleton(int num_bones);
    ~Skeleton();

    void SetBoneName(int bone_id,char* bone_name);
    void DebugPrint();

    //void Clone(Skeleton* dest);
    //void DestroySharedData();
    //void CalculatePose();

    Animation* GetAnimation(char* name);
};

class Pose{
    public:
    int             bone_count;
    Transform*      transforms;
    mat4*           matrices;
    Skeleton*       skeleton;
    AnimationHook   anim_hook;
    bool            animating;


    Pose(Skeleton* target);
    ~Pose();
    void StartAnimation(char* name);
    void StartAnimation(char* name,AnimationOptions options);
    //Stop Animation?
    void Calculate();//sets matrices;
};



#endif