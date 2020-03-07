#include "skeleton.h"
#include "../log.h"
#include <stdlib.h>

Skeleton::Skeleton(){}

Skeleton::~Skeleton(){
    if(pose_transforms != nullptr){
        free(pose_transforms);
        pose_transforms=nullptr;
    }
    if(pose_matrices != nullptr){
        free(pose_matrices);
        pose_matrices=nullptr;
    }
}

//Allocate and clear inital data once we know the number of bones.
void Skeleton::AllocateBoneCount(int num_bones){
    bone_count=num_bones;
    bones=(Bone*)calloc(num_bones,sizeof(Bone));
    pose_matrices=(mat4*)calloc(num_bones,sizeof(mat4));
    pose_transforms=(Transform*)calloc(num_bones,sizeof(Transform));
    inverse_bind_mats=(mat4*)calloc(num_bones,sizeof(mat4));
    pose_hook.num_targets = num_bones*3;
    pose_hook.targets = (AnimationTarget*)calloc(num_bones*3,sizeof(AnimationHook));

    for(int i=0;i<bone_count;i++){
        bones[i].parent_index=-1;
        bones[i].bind_transform.identity();
        bones[i].child_count=0;
        pose_transforms[i].Clear();
        pose_matrices[i].identity();
        inverse_bind_mats[i].identity();

        pose_hook.targets[i*3].value_type = AnimationType::TRANSFORM;
        pose_hook.targets[i*3].num_values=3;
        pose_hook.values[i*3]= &pose_transforms[i].x;//,y,z

        pose_hook.targets[i*3+1].value_type = AnimationType::ROTATION;
        pose_hook.targets[i*3+1].num_values=3;
        pose_hook.values[i*3+1]= &pose_transforms[i].rotation.x;//,y,z

        pose_hook.targets[i*3+2].value_type = AnimationType::SCALE;
        pose_hook.targets[i*3+2].num_values=3;
        pose_hook.values[i*3+2]= &pose_transforms[i].scale.x;//,y,z
    }
}

void Skeleton::SetBoneName(int bone_id, char* bone_name){
    bones[bone_id].name = cstr::new_copy(bone_name);

    pose_hook.targets[bone_id*3    ].object_name = bones[bone_id].name;
    pose_hook.targets[bone_id*3 + 1].object_name = bones[bone_id].name;
    pose_hook.targets[bone_id*3 + 2].object_name = bones[bone_id].name;
}


Skeleton* Skeleton::Clone(){
    Skeleton* ret = new Skeleton();
    ret->bone_count=bone_count;
    ret->bones=bones;
    ret->inverse_bind_mats=inverse_bind_mats;
    ret->animations = animations;
    ret->pose_transforms = (Transform*)calloc(bone_count,sizeof(Transform));
    for(int i=0;i<bone_count;i++){ret->pose_transforms->Clear();}
    ret->pose_matrices = (mat4*)calloc(bone_count,sizeof(mat4));
    return ret;
}

void Skeleton::DestroySharedData(){
    for(int i=0;i<bone_count;i++){
        free(bones[i].name);
        free(bones[i].child_indices);
    }
    free(bones);
    bones=nullptr;
    bone_count=0;

    if(inverse_bind_mats != nullptr){
        free(inverse_bind_mats);
        inverse_bind_mats=nullptr;
    }
    if(animations != nullptr){
        for(int i=0;i<animation_count;i++){
            animations[i].Destroy();
        }
        free(animations);
        animations=nullptr;
    }
    if(pose_hook.num_targets > 0){
        pose_hook.Destroy();
    }
}

void Skeleton::CalculatePose(){
    mat4 bone_matrix;
    for(int i=0; i < bone_count;i++){
        int bone_index = i;//pose_order[i];
        Bone* bone = &bones[bone_index];
        bone_matrix.identity(); 
        int parent = bone->parent_index;
        if(parent >= 0){
            bone_matrix.set(&pose_matrices[parent]);
        }
        if(parent > i){
            logger::exception("Bone ordered non-heirarchically. who would to this?!");
        }
        bone_matrix.multiply_by(&bone->bind_transform);
        //bone_matrix.translate(pose_transforms[bone_index].x,
        //                        pose_transforms[bone_index].y,
        //                        pose_transforms[bone_index].z);
        //bone_matrix.rotate(pose_transforms[bone_index].rotation);

        pose_matrices[bone_index].set(&bone_matrix);
    }
    for(int i=0; i < bone_count;i++){
        pose_matrices[i].multiply_by(&inverse_bind_mats[i]);
    }
}


Animation* Skeleton::GetAnimation(char* name){
    for(int i=0;i<animation_count;i++){
        if(cstr::compare(name,animations[i].name)){
            return &animations[i];
        }
    }
    return null;
}

void Skeleton::StartAnimation(char* name){
    for(int i=0;i<animation_count;i++){
        if(cstr::compare(name,animations[i].name)){
            AnimationManager::StartClip(&animations[i],&pose_hook);
        }
    }
}