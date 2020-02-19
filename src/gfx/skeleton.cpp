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
    for(int i=0;i<bone_count;i++){
        bones[i].parent_index=-1;
        bones[i].bind_transform.identity();
        bones[i].child_count=0;
        pose_transforms[i].Clear();
        pose_matrices[i].identity();
        inverse_bind_mats[i].identity();
    }
}

Skeleton* Skeleton::Clone(){
    Skeleton* ret = new Skeleton();
    ret->bone_count=bone_count;
    ret->bones=bones;
    ret->inverse_bind_mats=inverse_bind_mats;
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
}

void Skeleton::CalculatePose(){
    mat4 bone_matrix;
    for(int i=0; i < bone_count;i++){
        bone_matrix.identity(); 
        int parent = bones[i].parent_index;
        if(parent >= 0){
            bone_matrix.set(&pose_matrices[parent]);
        }
        if(parent > i){
            logger::exception("Skeleton is using a non-ascending bone order. Who would do this?");
        }
        bone_matrix.multiply_by(&bones[i].bind_transform);

        bone_matrix.translate(pose_transforms[i].x,pose_transforms[i].y,pose_transforms[i].z);
        bone_matrix.rotate(pose_transforms[i].rotation);

        pose_matrices[i].set(&bone_matrix);
    }
    for(int i=0; i < bone_count;i++){
        pose_matrices[i].multiply_by(&inverse_bind_mats[i]);
    }
}
