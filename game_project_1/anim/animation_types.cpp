#include <game_project_1/anim/animation_types.hpp>
#include <game_project_1/anim/animation_sequence.hpp>
#include <game_project_1/anim/animation_builders.hpp>
#include <game_project_1/io/crc.hpp>
#include <game_project_1/io/log.hpp>
#include <math.h>

using namespace Animation;


int Animation::WidthOfAnimationType(ValueType type){//number of primitive types per value
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
    value=val;
    name=cstr::new_copy(name);
    type=value_type;
}
Animation::ChannelHook::~ChannelHook(){
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

Animation::ActiveClip::ActiveClip(Clip* clip,Target* target){
    this->clip=clip;
    this->target=target;
    this->elapsed_time=0;
    this->timescale=1.0f;
    this->parent_sequence = null;
}
Animation::ActiveClip::ActiveClip(Clip* clip,Target* target,float time,float timescale,Sequence* parent){
    this->clip=clip;
    this->target=target;
    this->elapsed_time=time;
    this->timescale=timescale;
    this->parent_sequence = parent;
}
Animation::ActiveClip::~ActiveClip(){}

