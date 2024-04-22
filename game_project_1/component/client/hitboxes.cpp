#include "game_project_1/component/client/hitboxes.hpp"


Animation::Target* BuildAnimationTargetForShapeCollider(ShapeCollider* s,Animation::Target* slot){
    Animation::Target* hitbox_pos_target = new (slot) Animation::Target(3);
        hitbox_pos_target->hooks.Add(Animation::ChannelID::FromObject("position"),(float*)&s->center_offset.x);
        hitbox_pos_target->hooks.Add(Animation::ChannelID::FromObject("rotation"),(float*)&s->rotation.x);
        hitbox_pos_target->hooks.Add(Animation::ChannelID::FromObject("scale"),(float*)&s->scale.x);

    return hitbox_pos_target;
}

HitPath::HitPath():collider(){
    path=nullptr;
    spawn_time=0.0f;
    despawn_time=0.0f;
}
HitPath::HitPath(ShapeCollider hitcollider,Animation::Clip* animation_path,float spawn_time,float despawn_time){
    collider = hitcollider;
    path = animation_path;
    spawn_time = spawn_time;
    despawn_time = despawn_time;
}


HitPattern::HitPattern(int hitpath_count):hitpaths(hitpath_count){
    max_lifetime=-1;
}

HitPattern::~HitPattern(){}


HitBoxes::HitBoxes():hit_colliders(2),hit_collider_targets(2){
    current_pattern=null;
}

HitBoxes::~HitBoxes(){

}

void HitBoxes::CleanupPattern(){
    current_pattern=nullptr;
    current_pattern_active_time=0;
    hit_colliders.Clear();
    for(Animation::Target* active_collider_anim: hit_collider_targets){
        Animation::Stop(active_collider_anim);
    }
    hit_collider_targets.Clear();
}

void HitBoxes::Clear(){
    CleanupPattern();
}

Component* HitBoxes::Clone(){
    HitBoxes* copy = new HitBoxes();
    copy->current_pattern = nullptr;
    copy->current_pattern_active_time = 0.0f;
    /*for now don't try to copy the original's animation to the clone (where would we even get the anims from?)*/

    //for(ShapeCollider* original_hit_collider: hit_colliders){
    //    ShapeCollider* new_hit_collider = new (copy->hit_colliders.Allocate()) ShapeCollider(original_hit_collider);
    //    AnimationTarget* new_anim_target = 
    //        BuildAnimationTargetForShapeCollider(new_hit_collider,copy->hit_collider_targets.Allocate());
    //    AnimationManager::StartClipWithOffset(new_anim_target,)
    //}
    return copy;
}
