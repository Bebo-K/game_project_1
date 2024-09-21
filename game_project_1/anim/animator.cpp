#include <game_project_1/anim/animator.hpp>
#include <game_project_1/anim/animation_types.hpp>
#include <game_project_1/anim/animation_algos.hpp>
#include <game_project_1/anim/animation_sequence.hpp>
#include <game_project_1/io/log.hpp>
#include <math.h>

using namespace Animation;


List<ActiveClip> active_clips(8);
Dictionary<ActiveClip*,Clip*> queued_clips(8);

ActiveClip* GetActiveClipForTarget(Target* target){
    for(ActiveClip* ac: active_clips){
        if(ac->target == target){return ac;}
    }
    return null;
}

void SetActiveClip(ActiveClip* clip){//Overwrite active + queued animations
    if(clip->clip == null){
        logger::warn("Attempted to play empty clip",clip->clip->name);return;
    }
    if(clip->target == null){
        logger::warn("Cannot play clip '%s' for empty target",clip->clip->name);return;
    }
    ActiveClip* old = GetActiveClipForTarget(clip->target);
    if(old != null){
        queued_clips.Remove(old);
        active_clips.Remove(old);
        old->target->active_clip=nullptr;
        delete old;
    }
    active_clips.Add(clip);
    clip->target->active_clip=clip;
}

void EndActiveClip(ActiveClip* clip){
    active_clips.Remove(clip);
    clip->target->active_clip=nullptr;

    if(queued_clips.Has(clip)){
        ActiveClip* next = new ActiveClip(queued_clips.Get(clip),clip->target,0,clip->timescale,clip->parent_sequence);
        next->target->active_clip=next;
        active_clips.Add(next);
        if(clip->parent_sequence != null){
            clip->parent_sequence->OnAnimationFinish(clip,next);
        }
        queued_clips.Remove(clip);
    }
    else{
        clip->parent_sequence->OnAnimationFinish(clip,null);
    }
    delete clip;
}

void Animator::Init(){}
void Animator::Update(Timestep delta){
    Clip* clip; 
    Channel* channel;
    for(ActiveClip* current_clip: active_clips){
        current_clip->elapsed_time += delta.seconds*current_clip->timescale;

        if(current_clip->target->active_clip != current_clip){
            logger::warn("Stray animation clip was removed");//TODO: info on this?
            delete current_clip;
        }
        
        if(current_clip->target->enabled){
            clip = current_clip->clip;
            for(int j=0;j<clip->channel_count;j++){
                channel = &clip->channels[j];
                ChannelHook* hook = current_clip->target->hooks.Get(channel->id);
                AnimationAlgorithms::UpdateChannel(current_clip,channel,hook);
                if(hook == null){
                    logger::warn("\nFound hooks are (looking for %d):",channel->id);
                    for(Tuple<int,ChannelHook*> entry: current_clip->target->hooks){
                        logger::warn("\n%d: ",entry.key);
                        if(entry.value->name != null){
                            logger::warn(entry.value->name);
                        }
                    }
                    logger::warn("-----\n");
                }
            }
        }
        if(current_clip->elapsed_time > current_clip->clip->length && !current_clip->clip->loop){
            EndActiveClip(current_clip);
        }
    }
}

void Animator::Free(){
    active_clips.Clear();
    queued_clips.Clear();
}

void Animation::Start(Clip *clip, Target *target){ StartAt(clip,target,0.0f); }
void Animation::StartAt(Clip* clip,Target* target,float start_time){ StartAt(clip,target,start_time,1.0f); }
void Animation::StartAt(Clip* clip,Target* target,float start_time,float timescale){
    ActiveClip* existing_clip = GetActiveClipForTarget(target);
    if(existing_clip){active_clips.Remove(existing_clip);/*todo- allow for multiple layers*/}
    SetActiveClip(new ActiveClip(clip,target,start_time,timescale,null));
}

void Animation::Queue(Clip* clip,ActiveClip* after){
    queued_clips.Add(after,clip);
}
void Animation::Pause(Target *target){if(target->active_clip != null)target->active_clip->timescale=0.0f;}
void Animation::Stop(Target *target){
    if(target->active_clip != null){
        ActiveClip* active_clip = target->active_clip;
        active_clips.Remove(active_clip);
        queued_clips.Remove(active_clip);
        target->active_clip=null;
        delete active_clip;
    }
}
