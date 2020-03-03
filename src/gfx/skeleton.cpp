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
    if(pose_order != nullptr){
        free(pose_order);
        pose_order=nullptr;
    }
}

void Skeleton::SolvePoseOrder(){
    pose_order = (int*)calloc(bone_count,sizeof(int));
    int start_of_heirarchy=0;
    int end_of_list=0;
    while(end_of_list < bone_count){
        for(int i=0;i<bone_count;i++){
            if(bones[i].parent_index < 0){
                pose_order[end_of_list] = i;
                start_of_heirarchy=end_of_list;
                end_of_list += 1;
                break;
            }
        }
        for(int i=start_of_heirarchy;i<end_of_list;i++){
            int bone_index = pose_order[i];
            for(int j=0;j<bones[bone_index].child_count;j++){
                pose_order[end_of_list]=bones[bone_index].child_indices[j];
                end_of_list++;
            }
        }
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
