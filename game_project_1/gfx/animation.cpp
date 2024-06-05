#include <game_project_1/gfx/animation.hpp>
#include <game_project_1/io/crc.hpp>
#include <game_project_1/io/log.hpp>
#include <math.h>

using namespace Animation;

List<ActiveClip> active_clips(8);
Dictionary<ActiveClip*,Clip*> queued_clips(8);
void AddActiveClip(ActiveClip* clip){
    active_clips.Add(clip);
    if(clip->target != null){
        if(clip->target->active_clip != null && queued_clips.Has(clip->target->active_clip)){ 
            queued_clips.Remove(clip->target->active_clip);
        }
        clip->target->active_clip=clip;
    }
}
void QueueClip(ActiveClip* after,Clip* newClip){
    if(active_clips.GetIndex(after) >= 0){
        queued_clips.Add(after,newClip);
    }
}
void RemoveActiveClip(ActiveClip* clip){
    active_clips.Remove(clip);
    if(clip->target != null && clip->target->active_clip==clip){clip->target->active_clip=nullptr;}
    if(queued_clips.Has(clip)){
        Clip* transition_clup = queued_clips.Get(clip);
        AddActiveClip(new ActiveClip(queued_clips.Get(clip),clip->target,0,clip->timescale));
        queued_clips.Remove(clip);
    }
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
    logger::warn("creating hook with name %s\n",name);
    value=val;
    name=cstr::new_copy(name);
    type=value_type;
}
Animation::ChannelHook::~ChannelHook(){
    logger::warn("Deallocating channel hook of name %s\n",name);
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
    SAFE_DELETE(channels);
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


//nlerp, consider slerp if appropriate
void QuaternionLinearInterpolate(quaternion* from,quaternion* to, float weight,quaternion* values, int values_count){
    for(int i=0;i<values_count;i++){ 
        values[i].x = to[i].x*weight + from[i].x*(1.0f-weight);
        values[i].y = to[i].y*weight + from[i].y*(1.0f-weight);
        values[i].z = to[i].z*weight + from[i].z*(1.0f-weight);
        values[i].w = to[i].w*weight + from[i].w*(1.0f-weight);
        values[i].normalize();
    }
}
void QuaternionSphericalInterpolate(quaternion* q1,quaternion* q2, float weight,quaternion* values, int values_count){
    for(int i=0;i<values_count;i++){ 
        quaternion from = q1[i];
        quaternion to = q2[i];

        float dot = to.dot(from);
        if(dot < 0){
            dot *= -1;
            from = from*-1;
        }
        if(dot > 0.9995f){
            QuaternionLinearInterpolate(&from,&to,weight,&values[i],1);
            return;
        } 

        float theta_0 = acosf(dot);
        float theta = theta_0*weight;
        float sin_theta = sinf(theta);
        float sin_theta_0 = sinf(theta_0);
        float s0 = cos(theta) - dot * sin_theta / sin_theta_0;
        float s1 = sin_theta / sin_theta_0;

        values[i] = from*s0 + to*s1;
    }
}
void QInterpolate(float* from, float* to, float weight, float* values, int values_count){
    QuaternionSphericalInterpolate((quaternion*)from,(quaternion*)to, weight,(quaternion*)values, values_count/4);
}

void Interpolate(float* from, float* to, float weight, float* values, int values_count){
    for(int i=0;i<values_count;i++){
        values[i] = to[i]*weight + from[i]*(1.0f-weight);
    }
}

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

    switch(channel->interpolate_mode){
        case QUATERNION:
            QInterpolate(
                &channel->keyframe_values[last_keyframe*channel_width],
                &channel->keyframe_values[next_keyframe*channel_width],
                weight,hook->value,channel_width);
            break;
        case LINEAR:
        default:
            Interpolate(
                &channel->keyframe_values[last_keyframe*channel_width],
                &channel->keyframe_values[next_keyframe*channel_width],
                weight,hook->value,channel_width);
            break;
    }
}

///////////Animation / Animation Manager

void AnimationManager::Init(){}
void AnimationManager::Update(float seconds){
    Clip* clip; 
    Channel* channel;
    for(ActiveClip* current_clip: active_clips){
        current_clip->elapsed_time += seconds*current_clip->timescale;

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
        if(current_clip->elapsed_time > clip->length && !clip->loop){
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
    AddActiveClip(new ActiveClip(clip,target,start_time,timescale));
}

void Animation::Queue(Clip* clip,ActiveClip* after){
    QueueClip(after,clip);
}
void Animation::Pause(Target *target){if(target->active_clip != null)target->active_clip->timescale=0.0f;}
void Animation::Stop(Target *target){if(target->active_clip != null)delete target->active_clip;}


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

















































/*


List<ClipInfo> managed_clips(8);
int active_layer=0;
bool layer_is_active[ANIMATION_LAYER_COUNT];

int WidthOfAnimationType(AnimationType type){//number of primitive types per value
    switch(type){
        case SINGLE_FLOAT: return 1;
        case SINGLE_INT:return 1;
        case VECTOR3:return 3;
        case QUATERNION:return 4;
        case VECTOR2:return 2;
    }
    return 1;
}
size_t SizeOfAnimationType(AnimationType type){
    switch(type){
        case SINGLE_FLOAT: return sizeof(float);
        case SINGLE_INT:return sizeof(int);
        case VECTOR3:return sizeof(vec3);
        case QUATERNION:return sizeof(quaternion);
        case VECTOR2:return sizeof(vec2);
    }
    return sizeof(float);
}

bool AnimationTypeIsOfInteger(AnimationType type){
    switch(type){
        case SINGLE_FLOAT: return false;
        case SINGLE_INT:return true;
        case VECTOR3:return false;
        case QUATERNION:return false;
        case VECTOR2:return false;
    }
    return false;
}

ChannelID::ChannelID(char* name){
    id= CRC((byte*)name,cstr::len(name));
    optional_channel_name = name;
}

bool ChannelID::Compare(ChannelID& other){
    return other.id==id;
}


ClipInfo::ClipInfo(){
    animation = null;
    target = null;
    elapsed_time=0;
    timescale=0;
    end_action=0;
    next_anim = null;
    layer=0;
}
ClipInfo::~ClipInfo(){
    managed_clips.Remove(this);
}

AnimationOptions::AnimationOptions(){
    next_anim=null;
    timescale = 1.0f;
    end_action = AnimationEndAction::STOP;
}

AnimationChannel::AnimationChannel():id(nullptr){
    value_type = SINGLE_FLOAT;
    interpolate_mode = LINEAR;

    keyframe_count=0;
    keyframe_times=null;
    keyframe_values.fval=null;
}

AnimationChannel::AnimationChannel(char* channel_name,AnimationType channel_type,int keyframes):id(channel_name){
    value_type = channel_type;
    interpolate_mode=LINEAR;
    keyframe_times = (float*)calloc(keyframes,sizeof(float));
    keyframe_values.fval = (float*)calloc(keyframes,SizeOfAnimationType(channel_type));
}

AnimationChannel::~AnimationChannel(){
    DEALLOCATE(keyframe_times)
    DEALLOCATE(keyframe_values.fval)
}

void AnimationChannel::SetKeyframeCount(int keyframes){
    if(keyframe_times != null){free(keyframe_times);keyframe_times=null;}
    if(keyframe_values.fval != null){free(keyframe_values.fval);keyframe_values.fval=null;}
    keyframe_count = keyframes;
    keyframe_times = (float*)calloc(keyframes,sizeof(float));
    keyframe_values.fval = (float*)calloc(keyframes,SizeOfAnimationType(value_type));
}


AnimationTarget::AnimationTarget(int channel_count):values(channel_count){
    active_clip = null;
    active=false;
}

AnimationTarget::~AnimationTarget(){
    if(active_clip != null){delete active_clip; active_clip=null;}
}

ChannelValue* AnimationTarget::GetValueForChannel(ChannelID& id){
    for(ChannelValue* val: values){
        if(val->channel_id.Compare(id)) return val;
    }
    return null;
}

Animation::Animation(){
    name=null;
    channels=null;
    channel_count=0;
}

Animation::Animation(char* anim_name,int num_channels){
    if(anim_name==null){name=null;}
    else{name = cstr::new_copy(anim_name);}
    channel_count = num_channels;
    channels = new AnimationChannel[num_channels]();
}

Animation::~Animation(){
    if(name != null){free(name);name=null;}
    delete[] channels;
    channels=null;
}

void Animation::SetName(char* anim_name){
    if(anim_name==null){name=null;}
    else{name = cstr::new_copy(anim_name);}
}
void Animation::SetChannelCount(int num_channels){
    channel_count = num_channels;
    channels = new AnimationChannel[num_channels]();
}

void Animation::DebugPrint(){
    logger::info("....Animation Name: %s\n",name);
    logger::info("....Channel Count: %d\n",channel_count);
    //for(int i=0; i< channel_count;i++){
    //   logger::info("Animation %d:\n",i);
    //    channels[i].DebugPrint();
    //}
}

//------------------------------------------------//
//            Animation manager code              //
//------------------------------------------------//


void AnimationManager::Init(){
    for(int i=0;i< ANIMATION_LAYER_COUNT;i++){
        layer_is_active[i] = true;
    }
}
void AnimationManager::Free(){
    managed_clips.Clear();
}

void AnimationManager::SetActiveLayer(int layer){
    if(layer > 0 && layer < ANIMATION_LAYER_COUNT){active_layer=layer;}
}
int AnimationManager::GetActiveLayer(){
    return active_layer;
}

void AnimationManager::StartClip(Animation* anim, AnimationTarget* target){
    if(target == null)return;
    if(target->active_clip != null){
        delete target->active_clip;
        target->active_clip = null;
    }
    if(anim == null)return;//just stop active clip if no animation is supplied.
    ClipInfo* clip = new ClipInfo();
    
    clip->animation=anim;
    clip->target=target;
    clip->elapsed_time=0.0f;
    clip->end_action=AnimationEndAction::STOP;
    clip->timescale=1.0f;
    clip->layer=active_layer; 
    clip->next_anim=null;

    target->active_clip = clip;
    managed_clips.Add(clip);
}
void AnimationManager::StartClip(Animation* anim, AnimationTarget* target, AnimationOptions options){
    if(target == null)return;
    if(target->active_clip != null){
        delete target->active_clip;
        target->active_clip = null;
    }
    if(anim == null)return;//just stop active clip if no animation is supplied.
    ClipInfo* clip = new ClipInfo();
    
    clip->animation=anim;
    clip->target=target;
    clip->elapsed_time=0.0f;
    clip->end_action=options.end_action;
    clip->timescale=options.timescale;
    clip->layer=active_layer; 
    clip->next_anim = options.next_anim;

    target->active_clip = clip;
    managed_clips.Add(clip);
}

void AnimationManager::StopClip(AnimationTarget* target){
    if(target->active_clip != null){delete target->active_clip;target->active_clip=null;}
}

void UpdateChannel(ClipInfo* current_clip,valptr target,AnimationChannel* channel){
    int last_keyframe =0;
    int next_keyframe=0;
    AnimationType value_type = channel->value_type;
    int channel_width=WidthOfAnimationType(value_type);
    bool value_type_is_integer = AnimationTypeIsOfInteger(value_type);

    for(int k=0;k < channel->keyframe_count; k++){
        if(channel->keyframe_times[k] < current_clip->elapsed_time){last_keyframe = k;}
    }

    next_keyframe = last_keyframe+1;
    if(next_keyframe >= channel->keyframe_count){
        //TODO: end of animation event
        switch(current_clip->end_action){
            case AnimationEndAction::END: //end animation and stop
                //set target to last keyframe position exactly
                if(value_type_is_integer){
                    //TODO: lazy dev doesn't care about animating integers 
                }
                else{
                    memcpy(target.fval,&channel->keyframe_values.fval[last_keyframe*channel_width],sizeof(float)*channel_width);
                }
                current_clip->target->active=false;
                AnimationManager::StopClip(current_clip->target);
                break;
            case AnimationEndAction::STOP: //end animation, pause on last frame.
                if(value_type_is_integer){ }
                else{
                    memcpy(target.fval,&channel->keyframe_values.fval[last_keyframe*channel_width],sizeof(float)*channel_width);
                }
                AnimationManager::StopClip(current_clip->target);
                break;
            case AnimationEndAction::LOOP: //loop animation.
                while(current_clip->elapsed_time >= channel->keyframe_times[last_keyframe]){
                    current_clip->elapsed_time -= channel->keyframe_times[last_keyframe];
                }
                UpdateChannel(current_clip,target,channel);
                break;
            case AnimationEndAction::GOTO: //switch to another animation and loop.
                while(current_clip->elapsed_time >= channel->keyframe_times[last_keyframe]){
                    current_clip->elapsed_time -= channel->keyframe_times[last_keyframe];
                }
                current_clip->end_action= AnimationEndAction::LOOP;
                if(current_clip->next_anim != null){current_clip->animation = current_clip->next_anim;}
                current_clip->next_anim = null;
                break;
                

            default: //case 0
                if(value_type_is_integer){ }
                else{
                    memcpy(target.fval,&channel->keyframe_values.fval[last_keyframe*channel_width],sizeof(float)*channel_width);
                }
                AnimationManager::StopClip(current_clip->target);
            break;
        } 
    }
    else{
        float last_time = channel->keyframe_times[last_keyframe];
        float next_time = channel->keyframe_times[next_keyframe];
        float weight = (current_clip->elapsed_time - last_time) / (next_time-last_time);
        if(weight > 1.0f){weight = 1.0f;}
        if(weight < 0.0f){weight = 0.0f;}
        //weight = 0;
        //quaternion interpolation
        if(value_type == AnimationType::QUATERNION){
            QInterpolate(channel->interpolate_mode,
            &channel->keyframe_values.fval[last_keyframe*channel_width],
            &channel->keyframe_values.fval[next_keyframe*channel_width],
            weight,
            target.fval,
            channel_width
            );
        }
        else if(value_type_is_integer){
            //IntInterpolate();//TODO: also how do you abbreviate "integer interpolation"?
        }
        else{
            //floating point interpolation(s)
            Interpolate(channel->interpolate_mode,
                &channel->keyframe_values.fval[last_keyframe*channel_width],
                &channel->keyframe_values.fval[next_keyframe*channel_width],
                weight,
                target.fval,
                channel_width
                );
        }
    }
}

void AnimationManager::Update(float seconds){
    Animation *animation;
    AnimationTarget *target;
    AnimationChannel *channel;
    valptr value;

    for(ClipInfo* current_clip: managed_clips){
        if(!layer_is_active[current_clip->layer])continue;

        current_clip->elapsed_time += seconds*current_clip->timescale;

        target = current_clip->target;
        if(target == null)continue;
        target->active=true;
        
        animation = current_clip->animation;   

        for(int j=0;j<animation->channel_count;j++){
            channel = &animation->channels[j];
            value = target->GetValueForChannel(channel->id)->value;
            UpdateChannel(current_clip,value,channel);
            if(target->active_clip==null)break;
            if(animation != current_clip->animation){
                animation = current_clip->animation;  
                j=0;
            }
        }
    }
}


/*
void Animation::Destroy(){
    if(name != nullptr){free(name);name=nullptr;}
    for(int i=0;i<channel_count;i++){
        channels[i].Destroy();
    }
    if(channels != nullptr){free(channels);channels=nullptr;}
}
void AnimationTarget::Destroy(){
    if(targets != nullptr){
        for(int i=0;i<num_targets;i++){targets[i].Destroy();}
        free(targets);targets=nullptr;}
    if(values != nullptr){free(values);values=nullptr;}
}
void AnimationChannel::Destroy(){
    target.Destroy();
    if(keyframe_times != nullptr){free(keyframe_times);keyframe_times=nullptr;}
    if(keyframe_values != nullptr){free(keyframe_values);keyframe_values=nullptr;}
}
void AnimationTarget::Destroy(){
     This would result in a lot of extra copies of bone names being needed
    if(object_name!= nullptr){
        free(object_name);
        object_name=nullptr;
    }
    
}
*/