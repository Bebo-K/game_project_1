#include <game_project_1/anim/animation_builders.hpp>
#include <game_project_1/io/crc.hpp>
#include <game_project_1/io/log.hpp>
#include <math.h>

using namespace Animation;



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