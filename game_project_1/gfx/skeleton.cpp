#include <game_project_1/gfx/skeleton.hpp>
#include <game_project_1/io/log.hpp>
#include <stdlib.h>

Skeleton::Skeleton(int num_bones):bones(num_bones),animations(){
    if(num_bones <= 0){
        inverse_bind_mats=null;
    }
    inverse_bind_mats=(mat4*)calloc(num_bones,sizeof(mat4));

    for(int i=0;i<num_bones;i++){
        bones[i]->parent_index=-1;
        bones[i]->bind_transform.identity();
        inverse_bind_mats[i].identity();
    }
}

Skeleton::~Skeleton(){
    for(Bone* bone:bones){free(bone->name);}
    bones.Destroy();
    animations.Destroy();
    if(inverse_bind_mats != nullptr){free(inverse_bind_mats);inverse_bind_mats=null;}
}

void Skeleton::SetBoneName(int bone_id, char* bone_name){
    bones[bone_id]->name = cstr::new_copy(bone_name);
}

void Skeleton::DebugPrint(){
    logger::info("Bone Count: %d\n",bones.length);
    for(int i=0; i< bones.length;i++){
        logger::info("Bone %d:\n",i);
        bones[i]->DebugPrint();
    }
    
    logger::info("..Animation Count: %d\n",animations.length);
    for(int i=0; i< animations.length;i++){
        logger::info("..Animation %d:\n",i);
        animations[i]->DebugPrint();
    }
}

void Bone::DebugPrint(){
    logger::info("Name %s:\n",name);
}

Animation* Skeleton::GetAnimation(char* name){
    for(Animation* anim:animations){
        if(cstr::compare(name,anim->name))return anim;
    }
    return null;
}

Pose::Pose(Skeleton* target):anim_target(target->bones.length*3){
    skeleton=target;
    bone_count=target->bones.length;
    transforms= (Transform*)calloc(bone_count,sizeof(Transform));
    matrices =  (mat4*)calloc(bone_count,sizeof(mat4));

    for(int i=0;i<bone_count;i++){
        transforms[i].Clear();
        matrices[i].identity();
        
        anim_target.values[i*3]->channel_id = {skeleton->bones[i]->name};
        anim_target.values[i*3]->value_type = AnimationType::VECTOR3;
        anim_target.values[i*3]->value.fval = &transforms[i].x;//,y,z

        anim_target.values[i*3+1]->channel_id(skeleton->bones[i]->name);
        anim_hook.targets[i*3+1].value_type =  AnimationType::QUATERNION;
        anim_hook.targets[i*3+1].num_values=4;
        anim_hook.values[i*3+1]= &transforms[i].rotation.x;//,y,z,w

        anim_hook.targets[i*3+2].object_name = skeleton->bones[i]->name;
        anim_hook.targets[i*3+2].value_type = AnimationType::VECTOR3;
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
        Bone* bone = skeleton->bones[bone_index];
        bone_matrix.identity(); 
        int parent = bone->parent_index;
        if(parent >= 0 && parent < bone_count){
            bone_matrix.set(&matrices[parent]);
        }
        if(parent > i){
            logger::exception("Bone ordered non-heirarchically. I should handle this case");
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
    AnimationManager::StartClip(target_anim,&anim_target);
}
void Pose::StartAnimation(char* name,AnimationOptions options){
    Animation* target_anim = skeleton->GetAnimation(name);
    AnimationManager::StartClip(target_anim,&anim_target, options);
}


