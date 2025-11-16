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
    DEALLOCATE(inverse_bind_mats)
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
        //animations[i]->DebugPrint();
    }
}

void Bone::DebugPrint(){
    logger::info("Name %s:\n",name);
}

Animation::Clip* Skeleton::GetAnimation(char* name){
    for(Animation::Clip* anim:animations){
        if(cstr::compare(name,anim->name))return anim;
    }
    return null;
}

Pose::Pose(Transform* parent,Skeleton* target):animation_target(target->bones.length*3){
    skeleton=target;
    bone_count=target->bones.length;
    transforms= (Transform*)calloc(bone_count,sizeof(Transform));
    matrices =  (mat4*)calloc(bone_count,sizeof(mat4));
    for(int i=0;i<bone_count;i++){
        transforms[i].Clear();

        int parent_index = target->bones[i]->parent_index;
        if(parent_index >= 0 && parent_index < bone_count){
            transforms[i].parent = &transforms[target->bones[i]->parent_index];
        }
        else{
            //Should the parent be null or the entity transform?
            transforms[i].parent = parent;
        }

        matrices[i].identity();
        animation_target.AddTransformHooks(&transforms[i],skeleton->bones[i]->name);
    }
}

Pose::~Pose(){
    if(skeleton != null){skeleton= null;}
    DEALLOCATE(transforms)
    DEALLOCATE(matrices)
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
        bone_matrix.scale(transforms[bone_index].scale);

        matrices[bone_index].set(&bone_matrix);
    }
    if(animation_target.active_clip != null){
        for(int i=0; i < bone_count;i++){
            matrices[i].multiply_by(&skeleton->inverse_bind_mats[i]);
        }
    }
    else{//this shouldn't be neccesary if all transforms are zeroed
        for(int i=0; i < bone_count;i++){
            matrices[i].identity();
        }
    }
}

void Pose::StartAnimation(char* name){
    Animation::Clip* c = skeleton->GetAnimation(name);
    if(c != null){
        Animation::Start(c,&animation_target);
    }
    else{
        logger::warn("Cannot start animation '%s', no matching clip found\n", name);
    }
}

void Pose::QueueAnimation(char* name){
        Animation::Clip* c = skeleton->GetAnimation(name);
    if(c != null){
        Animation::Queue(c,animation_target.active_clip);
    }
    else{
        logger::warn("Cannot queue animation '%s', no matching clip found\n", name);
    }
}

void Pose::StopAnimations(){
    Animation::Stop(&animation_target);
    for(int i=0;i< bone_count;i++){
        transforms[i].Clear();
    }
}


