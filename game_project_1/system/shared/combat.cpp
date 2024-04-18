#include <game_project_1/system/shared/combat.hpp>


AttackType GetUnitAttackType(ClientEntity* e, ClientScene* s){
    MovementState* movement = e->Get<MovementState>();
    if(!movement)return 0;
    if(movement->move_goal.length_sqr() > 0.3f){
        return 2;//running attack
    }
    return 1;//standing attack
}

HitPattern* GetHitPatternForAttackType(Entity* e,AttackType attack_type){
    if(attack_type ==0) return null;

    float height=1.0f;
    float width=1.0f;

    if(e->Has<PhysicsProperties>()){
        Ellipse_t world_size = e->Get<PhysicsProperties>()->world_hitsphere;
        height=world_size.height;
        width=world_size.radius;
    }

    HitPattern* pattern = new HitPattern(1);

    pattern->max_lifetime=60;
    pattern->hitpaths[0]->collider.center_offset = {0,0.5*height,0};
    pattern->hitpaths[0]->collider.scale = {1,1,1};// {1.0f*height,1.0f*height,1.0f*height};
    pattern->hitpaths[0]->collider.shape = Collider::SPHERE;

    pattern->hitpaths[0]->path->SetChannelCount(1);
    pattern->hitpaths[0]->spawn_time=0.0f;
    pattern->hitpaths[0]->despawn_time=0.5f;
    pattern->hitpaths[0]->path->length=0.5f;
    pattern->hitpaths[0]->path->name="sphere_forward";
    AnimationChannel* positionChannel = &pattern->hitpaths[0]->path->channels[0];
        positionChannel->id = ChannelID("position");
        positionChannel->interpolate_mode = LINEAR;
        positionChannel->keyframe_count=2;
        positionChannel->keyframe_times = new float[2];
            positionChannel->keyframe_times[0] = 0.0f;
            positionChannel->keyframe_times[1] = 0.5f;
    vec3* values = new vec3[2];
        values[0]={0,0,0};
        values[1]={0,0,-1.0f};

    positionChannel->keyframe_values.fval = (float*)values;

    return pattern;
}

void Combat::ClientStartAttack(ClientEntity* e, ClientScene* s){
    CharacterInfo* char_info = e->Get<CharacterInfo>();
    StatBlock* stats = e->Get<StatBlock>();
    Equip* equip = e->Get<Equip>();
    ActionState* action_state = e->Get<ActionState>();
    ColliderSet* colliders = e->Get<ColliderSet>();


    HitBoxes* hitboxes = e->Get<HitBoxes>();
    ModelSet* models = e->Get<ModelSet>();

    if(!colliders || !action_state || !stats || !char_info)return;
    if(action_state->action_cooldown != 0)return;
    action_state->action_impulse = true;
    action_state->action_cooldown = 100;

    AttackType attack_type = GetUnitAttackType(e,s);
    HitPattern* attack_pattern = GetHitPatternForAttackType(e, attack_type);
    char* attack_animation_name = GetAnimationForAttackType(e, attack_type);

    if(attack_pattern){
        //terminate current pattern?
        for(AnimationTarget* running_hitpath_anim: hitboxes->hit_collider_targets){
            AnimationManager::StopClip(running_hitpath_anim);
        }
        hitboxes->hit_colliders.Clear();
        hitboxes->hit_collider_targets.Clear();
        hitboxes->current_pattern_active_time=0.0f;
        hitboxes->current_pattern = attack_pattern;
    }
    if(attack_animation_name){
        AnimationController::SetAnimationForEntity(e,attack_animation_name,/*windup*/false,/*loop*/false);
    }

    //Start here
    

    //TODO-BUT-NOT-NOW: async attack (windup, draw/aim + fire)
    //Client initiates attack
    //Client-side hurtbox is generated + motion started                         X
        //On anim completion, destroy all remaining pattern hitboxes            X
    //Client-side accompanying model animations are played                      ?
        //On anim completion, return to idle                                    X
    

}

void Combat::ServerStartAttack(ServerEntity* e, ServerScene* s){}

void Combat::ServerUpdate(Entity* e, float delta){
    if(!e->Has<ColliderSet>()){return;}
}

bool FloatCrossesThreshhold(float base,float add,float threshhold){
    return base <= threshhold && (base+add) > threshhold;
}

void Combat::ClientUpdate(ClientEntity* e, float delta){
    if(!e->Has<HitBoxes>()){return;}
     
    HitBoxes* hitboxes = e->Get<HitBoxes>();
    if(hitboxes->current_pattern != null){
        HitPattern* pattern = hitboxes->current_pattern;
        if(hitboxes->current_pattern_active_time < hitboxes->current_pattern->max_lifetime){
            for(HitPath* hitpath: pattern->hitpaths){
                if(FloatCrossesThreshhold(hitboxes->current_pattern_active_time,delta,hitpath->spawn_time)){
                    //create collider + start animation
                    AnimationOptions anim_options;
                        anim_options.timescale=1.0f;
                        anim_options.end_action=AnimationEndAction::END;
                        anim_options.next_anim=null;
                        
                    ShapeCollider* box = new (hitboxes->hit_colliders.Allocate()) ShapeCollider(hitpath->collider);
                    AnimationTarget* target = 
                        BuildAnimationTargetForShapeCollider(box,hitboxes->hit_collider_targets.Allocate());
                    AnimationManager::StartClip(hitpath->path,target, anim_options);
                }
                else if(FloatCrossesThreshhold(hitboxes->current_pattern_active_time,delta,hitpath->despawn_time)){
                    int pattern_index = pattern->hitpaths.IndexOf(hitpath);
                    AnimationManager::StopClip(hitboxes->hit_collider_targets[pattern_index]);
                    hitboxes->hit_colliders.Delete(pattern_index);
                    hitboxes->hit_collider_targets.Delete(pattern_index);
                }
            }
            hitboxes->current_pattern_active_time += delta;
        }
        else{ hitboxes->CleanupPattern(); }
    }
}
