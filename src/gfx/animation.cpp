#include "animation.h"
#include "../data_structs.h"
#include "../log.h"
#include <string.h>


DataArray active_clips(8,sizeof(ClipInfo));
int active_layer=0;
bool layer_is_active[ANIMATION_LAYER_COUNT];


bool AnimationTarget::Compare(AnimationTarget other){
    return  value_type == other.value_type && 
            num_values == other.num_values &&
            cstr::compare(object_name,other.object_name);
}

void Animation::Destroy(){
    if(name != nullptr){free(name);name=nullptr;}
    for(int i=0;i<channel_count;i++){
        channels[i].Destroy();
    }
    if(channels != nullptr){free(channels);channels=nullptr;}
}
void AnimationHook::Destroy(){
    if(targets != nullptr){
        for(int i=0;i<num_targets;i++){
            targets[i].Destroy();
        }
        free(targets);targets=nullptr;}
    if(values != nullptr){free(values);values=nullptr;}
}
void AnimationChannel::Destroy(){
    target.Destroy();
    if(keyframe_times != nullptr){free(keyframe_times);keyframe_times=nullptr;}
    if(keyframe_values != nullptr){free(keyframe_values);keyframe_values=nullptr;}
}
void AnimationTarget::Destroy(){
    if(object_name!= nullptr){
        free(object_name);
        object_name==nullptr;
    }
}

float* AnimationHook::GetTarget(AnimationTarget target){
    for(int i=0;i< num_targets;i++){
        if(targets[i].Compare(target)){
            return values[i];
        }
    }
    return null;
} 

void AnimationManager::Init(){
    for(int i=0;i< ANIMATION_LAYER_COUNT;i++){
        layer_is_active[i] = true;
    }
}
void AnimationManager::SetActiveLayer(int layer){
    if(layer > 0 && layer < ANIMATION_LAYER_COUNT){active_layer=layer;}
}
int AnimationManager::GetActiveLayer(){
    return active_layer;
}

void AnimationManager::StartClip(Animation* clip, AnimationHook* hook){
    StartClip(clip,hook,AnimationEndAction::STOP);
}
void AnimationManager::StartClip(Animation* clip, AnimationHook* hook, AnimationEndAction end_action){
    ClipInfo *start_info = nullptr;
    ClipInfo* existing_clip_info;

    for(int i=0;i<active_clips.slots;i++){
        existing_clip_info = (ClipInfo*)active_clips.Get(i);
        if(existing_clip_info==nullptr)continue;
        if(existing_clip_info->hook == hook){
            start_info = existing_clip_info;
            break;
        }
    }
    if(start_info == nullptr){
        start_info = (ClipInfo*)active_clips.Add();
    }

    start_info->animation=clip;
    start_info->hook=hook;
    start_info->elapsed_time=0.0f;
    start_info->end_action=end_action;
    start_info->timescale=1.0f;
    start_info->layer=active_layer;
}
void AnimationManager::StopClip(AnimationHook* hook){
    ClipInfo* existing_clip_info;
    for(int i=0;i<active_clips.slots;i++){
        existing_clip_info = (ClipInfo*)active_clips.Get(i);
        if(existing_clip_info==nullptr)continue;
        if(existing_clip_info->hook == hook){
            active_clips.Remove(i);
            break;
        }
    }
}
ClipInfo AnimationManager::GetClipInfo(AnimationHook* hook){
    ClipInfo ret;
    ret.animation=nullptr;
    ret.hook=nullptr;
    ret.layer=-1;
    ret.end_action=-1;
    ret.timescale=0.0f;
    ret.elapsed_time=0.0f;
    ClipInfo* existing_clip_info;
    for(int i=0;i<active_clips.slots;i++){
        existing_clip_info = (ClipInfo*)active_clips.Get(i);
        if(existing_clip_info==nullptr)continue;
        if(existing_clip_info->hook == hook){
            memcpy(&ret,existing_clip_info,sizeof(ClipInfo));
            break;
        }
    }
    return ret;
}

void LinearInterpolate(float* from, float* to, float weight, float* values, int values_count){
    for(int i=0;i<values_count;i++){
        values[i] = to[i]*weight + from[i]*(1.0f-weight);
    }
}
void Interpolate(int type, float* from, float* to, float weight, float* values, int values_count){
    switch (type)
    {
    case 0:
        LinearInterpolate(from, to, weight, values, values_count);
        break;
    
    default:
        LinearInterpolate(from, to, weight, values, values_count);
        break;
    }
}

void UpdateChannel(ClipInfo* current_clip,float* target,AnimationChannel* channel){
    int last_keyframe =0;
    int next_keyframe=0;
    int channel_width=channel->target.num_values;

    for(int k=0;k < channel->keyframe_count; k++){
        if(channel->keyframe_times[k] < current_clip->elapsed_time){last_keyframe = k;}
    }

    next_keyframe = last_keyframe+1;
    if(next_keyframe >= channel->keyframe_count){
        //TODO: end of animation event
        switch(current_clip->end_action){
            case 0: //end animation + stop
                memcpy(target,&channel->keyframe_values[last_keyframe*channel_width],sizeof(float)*channel_width);//set target to last keyframe position exactly
                AnimationManager::StopClip(current_clip->hook);
                break;
            case 1: //loop animation.
                while(current_clip->elapsed_time >= channel->keyframe_times[last_keyframe]){
                    current_clip->elapsed_time -= channel->keyframe_times[last_keyframe];
                }
                UpdateChannel(current_clip,target,channel);
                break;
                

            default: //case 0
                memcpy(target,&channel->keyframe_values[last_keyframe*channel_width],sizeof(float)*channel_width);//set target to last keyframe position exactly
                AnimationManager::StopClip(current_clip->hook);
            break;
        } 
    }
    else{
        float last_time = channel->keyframe_times[last_keyframe];
        float next_time = channel->keyframe_times[next_keyframe];
        float weight = (current_clip->elapsed_time - last_time) / (next_time-last_time);

        Interpolate(channel->interpolate_mode,
            &channel->keyframe_values[last_keyframe*channel_width],
            &channel->keyframe_values[next_keyframe*channel_width],
            weight,
            target,
            channel_width
            );
    }
}

void AnimationManager::Update(float seconds){
    ClipInfo* current_clip;
    Animation *animation;
    AnimationHook *hook;
    AnimationChannel *channel;
    float* value;

    for(int i=0;i<active_clips.slots;i++){
        current_clip = (ClipInfo*)active_clips.Get(i);
        if(current_clip==nullptr)continue;
        if(!layer_is_active[current_clip->layer])continue;

        current_clip->elapsed_time += seconds;

        hook = current_clip->hook;
        animation = current_clip->animation;   

        for(int j=0;j<animation->channel_count;j++){
            channel = &animation->channels[j];
            value = hook->GetTarget(channel->target);
            if(hook == null)continue;
            UpdateChannel(current_clip,value,channel);
        }
    }
}