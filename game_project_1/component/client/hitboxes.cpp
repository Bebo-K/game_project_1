#include <game_project_1/component/client/hitboxes.hpp>
#include <game_project_1/gfx/debugdraw.hpp>
#include <game_project_1/types/uuid.hpp>

Animation::Target *BuildAnimationTargetForShapeCollider(ShapeCollider *s, Animation::Target *slot)
{
    Animation::Target *hitbox_pos_target = new (slot) Animation::Target(3);
    hitbox_pos_target->AddHook((float *)&s->center_offset.x, "position", Animation::VECTOR3);
    hitbox_pos_target->AddHook((float *)&s->rotation.x, "rotation", Animation::VECTOR3);
    hitbox_pos_target->AddHook((float *)&s->scale.x, "scale", Animation::VECTOR3);

    return hitbox_pos_target;
}

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

HitBoxes::HitBoxes() : hit_colliders(2), hit_collider_targets(2)
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

Component *HitBoxes::Clone()
{
    HitBoxes *copy = new HitBoxes();
    copy->current_pattern = nullptr;
    copy->current_pattern_active_time = 0.0f;
    /*for now don't try to copy the original's animation to the clone (where would we even get the anims from?)*/

    // for(ShapeCollider* original_hit_collider: hit_colliders){
    //     ShapeCollider* new_hit_collider = new (copy->hit_colliders.Allocate()) ShapeCollider(original_hit_collider);
    //     AnimationTarget* new_anim_target =
    //         BuildAnimationTargetForShapeCollider(new_hit_collider,copy->hit_collider_targets.Allocate());
    //     AnimationManager::StartClipWithOffset(new_anim_target,)
    // }
    return copy;
}

void DrawShapeCollider(Camera *cam,Transform transform, ShapeCollider *collider)
{
    vec3 rotated_offset = (collider->center_offset*).rotate(worldspace_transform.rotation);
    worldspace_transform += rotated_offset;

    color unique_color( seeded_rand(uuid(collider)) );
    unique_color.a = 128;
    unique_color.r = 128 + (unique_color.r)/2;
    unique_color.g= 128 + (unique_color.g)/2;
    unique_color.b = 128 + (unique_color.b)/2;
    color_f unique_color_f(unique_color);

    switch (collider->shape)
    {
    case Collider::SPHERE:
        DebugDraw::DrawSphere(cam, worldspace_transform, collider->scale.x,unique_color_f); 
        break;
    case Collider::AABB:
        DebugDraw::Draw3DRect(cam,worldspace_transform, collider->scale,unique_color_f);
        break;
    case Collider::CAPSULE:
    {
        DebugDraw::DrawSphere(cam, worldspace_transform, collider->scale.x,unique_color_f);
        DebugDraw::DrawCylinder(cam, worldspace_transform, collider->scale.y,collider->scale.x,unique_color_f);
        //
        Transform cap_transform(worldspace_transform);
        vec3 cap_offset = ((vec3){0,collider->scale.y,0}).rotate(cap_transform.rotation);
        cap_transform += cap_offset;
        DebugDraw::DrawSphere(cam, cap_transform, collider->scale.x,unique_color_f);
        
        break;
    }
    case Collider::ARC : // I'll get back to this
        break;
    default:
        break;
    }
}

void HitBoxes::Draw(Camera *cam)
{
    Transform transform = GetTransform();
    for (ShapeCollider *collider : hit_colliders)
    {
        DrawShapeCollider(cam,transform, collider);
    }
}