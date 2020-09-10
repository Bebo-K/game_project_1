#include "skeleton.h"
#include "../log.h"
#include <stdlib.h>

Skeleton::Skeleton(int num_bones){
    bone_count=num_bones;
    if(num_bones <= 0){
        bones= null;
        inverse_bind_mats=null;
    }
    bones=(Bone*)calloc(num_bones,sizeof(Bone));
    inverse_bind_mats=(mat4*)calloc(num_bones,sizeof(mat4));

    for(int i=0;i<bone_count;i++){
        bones[i].parent_index=-1;
        bones[i].bind_transform.identity();
        inverse_bind_mats[i].identity();
    }
}

Skeleton::~Skeleton(){
    if(bones != nullptr){
        for(int i=0;i<bone_count;i++){free(bones[i].name);}
        free(bones);
        bones = nullptr;
    }
    if(inverse_bind_mats != nullptr){free(inverse_bind_mats);inverse_bind_mats=null;}
    if(animations != nullptr){
        delete[] animations;
    }
}

void Skeleton::SetBoneName(int bone_id, char* bone_name){
    bones[bone_id].name = cstr::new_copy(bone_name);
}

Animation* Skeleton::GetAnimation(char* name){
    for(int i=0;i<animation_count;i++){
        if(cstr::compare(name,animations[i].name)){
            return &animations[i];
        }
    }
    return null;
}

Pose::Pose(Skeleton* target):anim_hook(target->bone_count*3){
    skeleton=target;
    bone_count=target->bone_count;
    transforms= (Transform*)calloc(bone_count,sizeof(Transform));
    matrices =  (mat4*)calloc(bone_count,sizeof(mat4));

    for(int i=0;i<bone_count;i++){
        transforms[i].Clear();
        matrices[i].identity();
        
        anim_hook.targets[i*3].object_name = skeleton->bones[i].name;
        anim_hook.targets[i*3].value_type = AnimationType::TRANSLATION;
        anim_hook.targets[i*3].num_values=3;
        anim_hook.values[i*3]= &transforms[i].x;//,y,z

        anim_hook.targets[i*3+1].object_name = skeleton->bones[i].name;
        anim_hook.targets[i*3+1].value_type = AnimationType::ROTATION;
        anim_hook.targets[i*3+1].num_values=4;
        anim_hook.values[i*3+1]= &transforms[i].rotation.x;//,y,z

        anim_hook.targets[i*3+2].object_name = skeleton->bones[i].name;
        anim_hook.targets[i*3+2].value_type = AnimationType::SCALE;
        anim_hook.targets[i*3+2].num_values=3;
        anim_hook.values[i*3+2]= &transforms[i].scale.x;//,y,z
    }
}

Pose::~Pose(){
    if(skeleton != null){skeleton= null;}
    if(transforms != null){free(transforms);transforms=null;}
    if(matrices != null){free(matrices);matrices=null;}
}

void Pose::Calculate(){
    mat4 bone_matrix;
    for(int i=0; i < bone_count;i++){
        int bone_index = i;//pose_order[i];
        Bone* bone = &skeleton->bones[bone_index];
        bone_matrix.identity(); 
        int parent = bone->parent_index;
        if(parent >= 0 && parent < bone_count){
            bone_matrix.set(&matrices[parent]);
        }
        if(parent > i){
            logger::exception("Bone ordered non-heirarchically. who would do this?!");
        }
        //bone_matrix.multiply_by(&bone->bind_transform);
        bone_matrix.translate(transforms[bone_index].x,
                                transforms[bone_index].y,
                                transforms[bone_index].z);
        bone_matrix.rotate(transforms[bone_index].rotation);

        matrices[bone_index].set(&bone_matrix);
    }
    if(anim_hook.active){
        for(int i=0; i < bone_count;i++){
            matrices[i].multiply_by(&skeleton->inverse_bind_mats[i]);
        }
    }
    else{
        for(int i=0; i < bone_count;i++){
            matrices[i].identity();
        }
    }
}

void Pose::StartAnimation(char* name){
    Animation* target_anim = skeleton->GetAnimation(name);
    AnimationManager::StartClip(target_anim,&anim_hook);
}
void Pose::StartAnimation(char* name,AnimationOptions options){
    Animation* target_anim = skeleton->GetAnimation(name);
    AnimationManager::StartClip(target_anim,&anim_hook, options);
}


