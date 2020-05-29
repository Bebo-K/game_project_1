#ifndef ANIMATION_H
#define ANIMATION_H

#include "../structs/data_types.h"

const int ANIMATION_LAYER_COUNT =4;

const int ANIMATION_LAYER_GLOBAL= 0;
const int ANIMATION_LAYER_SCENE= 1;
const int ANIMATION_LAYER_UI= 2;
const int ANIMATION_LAYER_USER=3;


enum AnimationType{
    TRANSLATION=0,ROTATION=1,SCALE=2,WEIGHT=3,USER=4,EVENT=5
};
enum AnimationInterpolateMode{
    LINEAR=0,STEP=1,CUBICORSOMETHING=2
};
enum AnimationEndAction{
    STOP=0,LOOP=1
};

struct AnimationTarget{//Identifying information correlating Channels to Hooks. All fields must match for a hook to be compatible with a channel. 
    char* object_name;//Not freed. Should always point to an existing cstr
    short value_type;
    short num_values;
    bool Compare(AnimationTarget other);
};

struct AnimationChannel{//A channel contains keyframe data for a block of one or more float values
    AnimationTarget target;
    AnimationInterpolateMode interpolate_mode;
    int    keyframe_count;
    float* keyframe_times;
    float* keyframe_values;
    AnimationChannel();
    AnimationChannel(AnimationTarget anim_target,int keyframes);
    void   SetTarget(AnimationTarget anim_target);
    void   SetKeyframeCount(int keyframes);
    ~AnimationChannel();
};

struct ClipInfo;
struct AnimationHook{//Contains information to hook float values to animation channels, and the state of those values.
    int                 num_targets;
    AnimationTarget*    targets;
    float**             values;
    ClipInfo*           active_clip;

    AnimationHook(int target_count);
    ~AnimationHook();

    float* GetTarget(AnimationTarget target);
};

class Animation{
    public:

    char* name;
    float length;
    int channel_count;
    AnimationChannel* channels;

    Animation();
    Animation(char* anim_name,int num_channels);
    ~Animation();

    void SetName(char* anim_name);
    void SetChannelCount(int num_channels);
};

struct ClipInfo{ //Info about the currently running animation
    Animation*  animation;
    AnimationHook* hook;
    float       elapsed_time;
    float       timescale;
    int         end_action;
    int         layer;

    ~ClipInfo();
};

struct AnimationOptions{
    float timescale;
    AnimationEndAction end_action;

    AnimationOptions();
};

namespace AnimationManager{
    void Init();

    void StartClip(Animation* animation, AnimationHook* hook);
    void StartClip(Animation* animation, AnimationHook* hook,AnimationOptions end_action);
    //TODO: behaviors for starting clips on existings targets. Replace clip/Layer clips/Start new clip from same timepoint
    //TODO: event system integration
    //TODO: pause layer
    ClipInfo GetClipInfo(AnimationHook* hook);
    void StopClip(AnimationHook* hook);

    void SetActiveLayer(int layer);//So we don't have to pass a layer param every time we start a clip.
    int GetActiveLayer();

    void Update(float seconds);

};


//TODO: AnimationBuilder
/*
struct AnimationBuilderKeyframe{
    float time;
    float* values;
};

class AnimationBuilderChannel{
    char* name;
    AnimationTarget target;
    int interpolate_mode;
    DataArray Keyframes;

    void AddKeyframe(float time, float val);
    void AddKeyframe(float time, float a, float b, float c);
    void AddKeyframe(float time, float* values);

    void BuildTo(AnimationChannel* dest);
};


class AnimationBuilder{
    char* name;
    float length;
    int channel_count;
    AnimationChannel* channels;
    DataArray channels();
    private:

    AnimationBuilderChannel* AddChannel(char* object,AnimationType channel_type,int channel_width,AnimationInterpolateMode interpolate_mode);

    Animation Finish();
};
*/

#endif