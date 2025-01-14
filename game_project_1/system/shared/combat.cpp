#include <game_project_1/system/shared/combat.hpp>
#include <game_project_1/gui/menu/developer_layer.hpp>

AttackType GetUnitAttackType(ClientEntity* e, ClientScene* s){
    MovementState* movement = e->Get<MovementState>();
    if(!movement)return 0;
    if(movement->move_goal.length_sqr() > 0.3f){
        return 2;//running attack
    }
    return 1;//standing attack
}

char* GetAnimationForAttackType(Entity* e, AttackType type){
    return "attack_1h_h";
}

int GetDurationForAttackType(ClientEntity* e, AttackType type){
    return 30*(*e->Get<ModelSet>())[0]->pose->skeleton->GetAnimation("attack_1h_h")->length;
}

int GetCooldownForAttackType(Entity* e, AttackType type){
    return 10;
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
    pattern->hitpaths[0]->collider.center_offset = {0,0.5f*height,0};
    pattern->hitpaths[0]->collider.scale = {1,1,1};// {1.0f*height,1.0f*height,1.0f*height};
    pattern->hitpaths[0]->collider.shape = Collider::SPHERE;
    pattern->hitpaths[0]->spawn_time=0.0f;
    pattern->hitpaths[0]->despawn_time=2.5f;
    pattern->hitpaths[0]->path =Animation::Clip::Builder()
    .Name("sphere_forward")
    .Duration(2.5f)
    .Loop(false)
    .AddChannel(Animation::ChannelBuilder()
        .ID(Animation::ChannelID("position"))
        .InterpolateMode(Animation::LINEAR)
        .Type(Animation::VECTOR3)
        .Keyframe(0.0f,{0,0.5,0})
        .Keyframe(2.5f,{0,0.5,-1.0f})
    ).Build();

    return pattern;
}

void Combat::ClientStartAttack(ClientEntity* e, ClientScene* s){
    CharacterInfo* char_info = e->Get<CharacterInfo>();
    StatBlock* stats = e->Get<StatBlock>();
    Equip* equip = e->Get<Equip>();
    ActionState* action_state = e->Get<ActionState>();
    ColliderSet* colliders = e->Get<ColliderSet>();

    HitBoxes* hitboxes = e->Get<HitBoxes>();
    //ModelSet* models = e->Get<ModelSet>();

    if(!colliders || !action_state || !stats || !char_info ||!hitboxes)return;
    if(action_state->action_cooldown != 0)return;
    action_state->action_id = Action::ID::ATTACK;
    action_state->action_impulse = true;

    AttackType attack_type = GetUnitAttackType(e,s);
    HitPattern* attack_pattern = GetHitPatternForAttackType(e, attack_type);
    char* attack_animation_name = GetAnimationForAttackType(e, attack_type);
    action_state->action_cooldown = GetCooldownForAttackType(e, attack_type);
    action_state->action_timer = GetDurationForAttackType(e,attack_type);

    if(attack_pattern){
        hitboxes->StartPattern(attack_pattern);
    }
    if(attack_animation_name){
        AnimationController::SetAnimationForEntity(e,attack_animation_name);
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

void Combat::ServerUpdate(Entity* e,Timestep delta){
    if(!e->Has<ColliderSet>()){return;}
}

bool FloatCrossesThreshhold(float base,float add,float threshhold){
    return base <= threshhold && (base+add) > threshhold;
}

void Combat::ClientUpdate(ClientEntity* e,Timestep delta){
    ActionState* action_state = e->Get<ActionState>();
    if(!action_state){return;}
    if(action_state->action_timer > 0){ //TODO: other action end conditions
        DeveloperLayer::SetLabelText(0,L"Action Timer: %d",action_state->action_timer);
        action_state->action_timer -= delta.frames;
        if(action_state->action_timer <=0){
            action_state->action_id = Action::ID::NONE;
            action_state->action_timer = 0;
        }
    }
    else if(action_state->action_cooldown > 0){
        action_state->action_cooldown -= delta.frames;
        if(action_state->action_cooldown <= 0){
            action_state->action_cooldown = 0;
        }
        DeveloperLayer::SetLabelText(0,L"Action Cooldown: %d",action_state->action_cooldown);
    }

    HitBoxes* hitboxes = e->Get<HitBoxes>();
    if(!hitboxes){return;}
     
    if(hitboxes->current_pattern != null){
        HitPattern* pattern = hitboxes->current_pattern;
        if(hitboxes->current_pattern_active_time < hitboxes->current_pattern->max_lifetime){
            for(HitPath* hitpath: pattern->hitpaths){
                if(FloatCrossesThreshhold(hitboxes->current_pattern_active_time,delta.seconds,hitpath->spawn_time)){
                    //create collider + start animation
                    ShapeCollider* box = new (hitboxes->hit_colliders.Allocate()) ShapeCollider(hitpath->collider);
                    Animation::Start(hitpath->path,
                        BuildAnimationTargetForShapeCollider(box,hitboxes->hit_collider_targets.Allocate()));
                }
                else if(FloatCrossesThreshhold(hitboxes->current_pattern_active_time,delta.seconds,hitpath->despawn_time)){
                    int pattern_index = pattern->hitpaths.IndexOf(hitpath);
                    Animation::Stop(hitboxes->hit_collider_targets[pattern_index]);
                    hitboxes->hit_colliders.Delete(pattern_index);
                    hitboxes->hit_collider_targets.Delete(pattern_index);
                }
            }
            hitboxes->current_pattern_active_time += delta.seconds;
        }
        else{ hitboxes->CleanupPattern(); }
    }
    if(action_state->action_impulse){
        action_state->action_impulse=false;
    }
}
