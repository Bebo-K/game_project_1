#include <game_project_1/gfx/animation.hpp>
#include <game_project_1/io/crc.hpp>
#include <game_project_1/io/log.hpp>
#include <math.h>
#include <game_project_1/types/interpolaters.hpp>

using namespace Animation;

List<ActiveClip> active_clips(8);
Dictionary<ActiveClip*,Clip*> queued_clips(8);
void AddActiveClip(ActiveClip* clip){
    active_clips.Add(clip);
    if(clip->target != null){
        if(clip->target->active_clip != null && clip->target->active_clip != clip){
            delete clip->target->active_clip;
        }
        clip->target->active_clip=clip;
    }
}

void RemoveActiveClip(ActiveClip* clip){
    active_clips.Remove(clip);
    if(clip->target != null && clip->target->active_clip==clip){clip->target->active_clip=nullptr;}
    if(queued_clips.Has(clip)){
        AddActiveClip(new ActiveClip(queued_clips.Get(clip),clip->target,0,clip->timescale));
        queued_clips.Remove(clip);
    }
}


void QueueClip(ActiveClip* after,Clip* newClip){
    if(active_clips.Has(after)){
        queued_clips.Add(after,newClip);
    }
}

void UnqueueClip(Clip* c){
    ActiveClip* before = queued_clips.ReverseLookup(c);
    if(before != null){queued_clips.Remove(before);}
}


int WidthOfAnimationType(ValueType type){//number of primitive types per value
    switch(type){
        case FLOAT: return 1;
        case VECTOR2:return 2;
        case VECTOR3:return 3;
        case QUATERNION:return 4;
        case OTHER:
        default: return 4;
    }
}

int Animation::ChannelID(char* name){
    return CRC((byte*)name,cstr::len(name));
}
int Animation::ChannelID(char* name,char* context){
    return CRC((byte*)name,cstr::len(name)) + CRC((byte*)context,cstr::len(context));
}

Animation::Channel::Channel(){
    id=0;
    value_type=FLOAT;
    interpolate_mode=LINEAR;
    keyframe_count=0;
    keyframe_times=nullptr;
    keyframe_values=nullptr;
}
Animation::Channel::Channel(int cid,ValueType channel_type,int keyframes){
    id=cid;
    value_type=channel_type;
    interpolate_mode=LINEAR;
    keyframe_count=keyframes;
    keyframe_times=(float*)malloc(sizeof(float)*WidthOfAnimationType(channel_type)*keyframes);
    keyframe_values=(float*)malloc(sizeof(float)*keyframes);
}
Animation::Channel::~Channel(){
    DEALLOCATE(keyframe_times)
    DEALLOCATE(keyframe_values)
}
Animation::ChannelBuilder& Channel::Builder(){return *new ChannelBuilder();}


Animation::ChannelHook::ChannelHook(float* val,char* name,ValueType value_type){
    //logger::warn("creating hook with name %s\n",name);
    value=val;
    name=cstr::new_copy(name);
    type=value_type;
}
Animation::ChannelHook::~ChannelHook(){
    //logger::warn("Deallocating channel hook of name %s\n",name);
    DEALLOCATE(name)}

Animation::Target::Target(int channel_count):hooks(channel_count){
    enabled=true;
    active_clip=nullptr;
}

void Animation::Target::AddHook(float* val,char* name,ValueType type){
    new (hooks.Add(ChannelID(name))) ChannelHook(val,name,type);
}
void Animation::Target::AddHook(float* val,char* name,char* context, ValueType type){
    char* target_name = cstr::append(name,'_',context);
    new (hooks.Add(ChannelID(name,context))) ChannelHook(val,target_name,type);
    free(target_name);//hook creates a copy
}
Animation::Target::~Target(){}


Animation::Clip::Clip(){
    name=nullptr;
    length=0;
    loop=false;
    channel_count=0;
    channels=nullptr;
}
Animation::Clip::Clip(char *anim_name, int num_channels){
    name = cstr::new_copy(anim_name);
    length=0;
    channel_count=num_channels;
    channels = (num_channels==0)? null : new Channel[num_channels];
}
Animation::Clip::~Clip(){
    DEALLOCATE(name);
    if(channels != null){delete[] channels;channels=null;}
    channel_count=0;
}
void Animation::Clip::SetChannelCount(int num_channels){
    SAFE_DELETE(channels);
    channel_count=num_channels;
    channels = new Channel[num_channels];
}
Animation::ClipBuilder& Clip::Builder(){return *new ClipBuilder();}

Animation::ActiveClip::ActiveClip(Clip* clip,Target* target,float time,float timescale){
    this->clip=clip;
    this->target=target;
    elapsed_time=time;
    this->timescale=timescale;
}
Animation::ActiveClip::~ActiveClip(){ RemoveActiveClip(this);}


void UpdateChannel(ActiveClip* current_clip,Channel* channel,ChannelHook* hook){
    if(hook == null){
        logger::warn("Hook does not exist for animation channel '%s' of clip '%s'",
        current_clip->clip->channel_names.Get(channel->id),
        current_clip->clip->name);
        return;
    }
    if(hook->type != channel->value_type){
        logger::warn("Animation channel '%s' of clip '%s' does not have a matching value type with target channel '%s'",
        current_clip->clip->channel_names.Get(channel->id),
        current_clip->clip->name,
        hook->name);
        return;
    }

    int last_keyframe =0;
    int next_keyframe=0;
    float weight=0.0f;
    int channel_width=WidthOfAnimationType(channel->value_type);
    bool loop = current_clip->clip->loop;
    float elapsed_time = current_clip->elapsed_time;
    float clip_length = current_clip->clip->length;

    while(loop && elapsed_time > clip_length){ elapsed_time -= clip_length; }

    for(int i=0;i < channel->keyframe_count; i++){
        if(channel->keyframe_times[i] < elapsed_time){last_keyframe = i;}
    }
    next_keyframe = last_keyframe+1;

    if(next_keyframe >= channel->keyframe_count){
        if(loop){
            next_keyframe = 0;
            float last_time = channel->keyframe_times[last_keyframe];
            float next_time = clip_length;
            weight = (elapsed_time - last_time) / (next_time-last_time);
        }
        else{ weight=0.0f; }
    }
    else{
        float last_time = channel->keyframe_times[last_keyframe];
        float next_time = channel->keyframe_times[next_keyframe];
        weight = (elapsed_time - last_time) / (next_time-last_time);
        if(weight > 1.0f){weight = 1.0f;}
        if(weight < 0.0f){weight = 0.0f;}
    }

    switch(channel->value_type){
        case QUATERNION :
            Interpolators::MultiQInterpolate(
                &channel->keyframe_values[last_keyframe*channel_width],
                &channel->keyframe_values[next_keyframe*channel_width],
                weight,hook->value,channel_width);
            break;
        default:
            Interpolators::MultiInterpolate(
                &channel->keyframe_values[last_keyframe*channel_width],
                &channel->keyframe_values[next_keyframe*channel_width],
                weight,hook->value,channel_width);
            break;
    }
}

///////////Animation / Animation Manager

void AnimationManager::Init(){}
void AnimationManager::Update(Timestep delta){
    Clip* clip; 
    Channel* channel;
    for(ActiveClip* current_clip: active_clips){
        current_clip->elapsed_time += delta.seconds*current_clip->timescale;

        if(current_clip->target != null &&
         current_clip->target->enabled &&
         current_clip->target->active_clip == current_clip){
            clip = current_clip->clip;
            for(int j=0;j<clip->channel_count;j++){
                channel = &clip->channels[j];
                ChannelHook* hook = current_clip->target->hooks.Get(channel->id);
                UpdateChannel(current_clip,channel,hook);
                
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
        else{
            logger::warn("Stray animation clip was removed");//TODO: info on this?
            delete current_clip;
        }
        if(current_clip->elapsed_time > current_clip->clip->length && !current_clip->clip->loop){
            //DoPostAnimAction(current_clip);
            delete current_clip;
        }
    }
}
void AnimationManager::Free(){
    active_clips.Clear();
    queued_clips.Clear();
}

void Animation::Start(Clip *clip, Target *target){ StartAt(clip,target,0.0f); }
void Animation::StartAt(Clip* clip,Target* target,float start_time){ StartAt(clip,target,start_time,1.0f); }
void Animation::StartAt(Clip* clip,Target* target,float start_time,float timescale){
    if(clip == null){
        logger::warn("Started empty clip for target");
    }
    AddActiveClip(new ActiveClip(clip,target,start_time,timescale));
}

void Animation::Queue(Clip* clip,ActiveClip* after){
    QueueClip(after,clip);
}
void Animation::Pause(Target *target){if(target->active_clip != null)target->active_clip->timescale=0.0f;}
void Animation::Stop(Target *target){
    if(target->active_clip != null){
        ActiveClip* active_clip = target->active_clip;
        queued_clips.Remove(active_clip);
        target->active_clip=null;
        delete active_clip;
        while(target->active_clip != null){
            delete target->active_clip;
        }
    }
}

KeyframeBuilder::KeyframeBuilder(float time,vec4 value){this->time=time;this->value=value;}
ChannelBuilder::ChannelBuilder(){
    id=0;
    value_type = ValueType::FLOAT;
    interpolate_mode = InterpolateMode::LINEAR;
}
ChannelBuilder& ChannelBuilder::ID(int cid){ id= cid;return *this;}
ChannelBuilder& ChannelBuilder::Type(ValueType type){this->value_type=type;return *this;}
ChannelBuilder& ChannelBuilder::InterpolateMode(Animation::InterpolateMode mode){this->interpolate_mode=mode;return *this;}
ChannelBuilder& ChannelBuilder::Keyframe(float time, float val){
    this->keyframes.Add(new KeyframeBuilder(time,{val,0,0,0}));return *this;}
ChannelBuilder& ChannelBuilder::Keyframe(float time, vec2 val){
    this->keyframes.Add(new KeyframeBuilder(time,{val.x,val.y,0,0}));return *this;}
ChannelBuilder& ChannelBuilder::Keyframe(float time, vec3 val){
    this->keyframes.Add(new KeyframeBuilder(time,{val.x,val.y,val.z,0}));return *this;}
ChannelBuilder& ChannelBuilder::Keyframe(float time, vec4 val){
    this->keyframes.Add(new KeyframeBuilder(time,val));return *this;}
ChannelBuilder& ChannelBuilder::Keyframe(float time, float* values){
    vec4 val= {0,0,0,0};
    for(int i=0;i<WidthOfAnimationType(this->value_type);i++){ ((float*)&val.x)[i] = values[i];}
    this->keyframes.Add(new KeyframeBuilder(time,val));return *this;}
Channel* ChannelBuilder::Build(){
    Channel* ret = new Channel(this->id,this->value_type,this->keyframes.Count());
    ret->interpolate_mode = this->interpolate_mode;
    int i=0;
    int width = WidthOfAnimationType(ret->value_type);
    for(KeyframeBuilder* frame: this->keyframes){
        ret->keyframe_times[i] = frame->time;
        for(int j=0;j<width;j++){
            ret->keyframe_values[i*width+j] = ((float*)&frame->value)[j];
        }
        i++;
    }
    return ret;
}
void ChannelBuilder::BuildTo(Channel* target){
    target->id=this->id;
    target->value_type=this->value_type;
    target->keyframe_count=this->keyframes.Count();
    target->interpolate_mode=this->interpolate_mode;
    target->keyframe_values=(float*)malloc(sizeof(float)*WidthOfAnimationType(target->value_type)*target->keyframe_count);
    target->keyframe_times=(float*)malloc(sizeof(float)*target->keyframe_count);
    int i=0;
    int width = WidthOfAnimationType(this->value_type);
    for(KeyframeBuilder* frame: this->keyframes){
        target->keyframe_times[i] = frame->time;
        for(int j=0;j<width;j++){
            target->keyframe_values[i*width+j] = ((float*)&frame->value)[j];
        }
        i++;
    }
}

ClipBuilder::ClipBuilder(){name=nullptr; loop=false; length=0.0f;}
ClipBuilder& ClipBuilder::Name(char* name){this->name = name;return *this;}
ClipBuilder& ClipBuilder::Duration(float length){this->length = length;return *this;}
ClipBuilder& ClipBuilder::Loop(bool loop){this->loop = loop;return *this;}
ClipBuilder& ClipBuilder::AddChannel(ChannelBuilder& channelbuilder){this->channels.Add(channelbuilder.Build());return *this;}
Clip* ClipBuilder::Build(){
    Clip* ret = new Clip(this->name,this->channels.Count());
    ret->loop=this->loop;
    ret->length=this->length;
    int i=0;
    for(Channel* channel: this->channels){
        ret->channels[i] = *channel;
        i++;
    }
    return ret;
}
void ClipBuilder::BuildTo(Clip* target){
    target->name=this->name;
    target->SetChannelCount(this->channels.Count());
    target->loop=this->loop;
    target->length=this->length;
    int i=0;
    for(Channel* channel: this->channels){
        target->channels[i] = *channel;
        i++;
    }
}