#include <game_project_1/component/client/hitboxes.hpp>
#include <game_project_1/gfx/debugdraw.hpp>
#include <game_project_1/types/uuid.hpp>

HitPath::HitPath() : collider()
{
    path = nullptr;
    spawn_time = 0.0f;
    despawn_time = 0.0f;
}
HitPath::HitPath(ShapeCollider hitcollider, Animation::Clip *animation_path, float spawn_time, float despawn_time)
{
    collider = hitcollider;
    path = animation_path;
    spawn_time = spawn_time;
    despawn_time = despawn_time;
}

HitPattern::HitPattern(int hitpath_count) : hitpaths(hitpath_count)
{
    max_lifetime = -1;
}

HitPattern::~HitPattern() {}

HitBoxes::HitBoxes(Transform* parent) : Drawable(parent), hit_colliders(2), hit_collider_targets(2)
{
    current_pattern = null;
    current_pattern_active_time = 0.0f;
}

HitBoxes::~HitBoxes()
{
}

void HitBoxes::StartPattern(HitPattern *pattern)
{
    // terminate current pattern
    for (Animation::Target *running_hitpath_anim : hit_collider_targets)
    {
        Animation::Stop(running_hitpath_anim);
    }
    hit_colliders.Clear();
    hit_collider_targets.Clear();
    current_pattern_active_time = 0.0f;
    current_pattern = pattern;
}

void HitBoxes::CleanupPattern()
{
    current_pattern = nullptr;
    current_pattern_active_time = 0;
    hit_colliders.Clear();
    for (Animation::Target *active_collider_anim : hit_collider_targets)
    {
        Animation::Stop(active_collider_anim);
    }
    hit_collider_targets.Clear();
}

void HitBoxes::Clear()
{
    CleanupPattern();
}

Component* HitBoxes::Clone(ComponentParentContext context){
    HitBoxes *copy = new HitBoxes(copy->offset.parent);
    copy->current_pattern = nullptr;
    copy->current_pattern_active_time = 0.0f;
    /*for now don't try to copy the original's animation to the clone (where would we even get the anims from?)*/

    // for(ShapeCollider* original_hit_collider: hit_colliders){
    //     ShapeCollider* new_hit_collider = new (copy->hit_colliders.Allocate()) ShapeCollider(original_hit_collider);
    //     AnimationTarget* new_anim_target =copy->hit_collider_targets.Allocate()
    //      new_anim_target->AddTransformHooks(&new_hit_collider->transform);
    //     AnimationManager::StartClipWithOffset(new_anim_target,new_anim_target)
    // }
    return copy;
}

void HitBoxes::Draw(Camera *cam)
{
    for (ShapeCollider *collider : hit_colliders)
    {
        collider->Draw(cam);
    }
}