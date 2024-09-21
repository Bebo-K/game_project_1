#ifndef ANIMATION_TYPES_H
#define ANIMATION_TYPES_H

#include <game_project_1/types/timestep.hpp>
#include <game_project_1/types/primitives.hpp>
#include <game_project_1/types/arrays.hpp>
#include <game_project_1/types/map.hpp>
#include <game_project_1/types/list.hpp>

const int ANIMATION_LAYER_COUNT = 4;

const int ANIMATION_LAYER_GLOBAL = 0;
const int ANIMATION_LAYER_SCENE = 1;
const int ANIMATION_LAYER_UI = 2;
const int ANIMATION_LAYER_USER = 3;


namespace Animation{

    enum ValueType : short {
        FLOAT,
        VECTOR2,
        VECTOR3,
        QUATERNION,
        OTHER,
    };

    int WidthOfAnimationType(ValueType type);

    enum InterpolateMode : short {
        LINEAR,
        STEP,
        SPHERICAL,
        CUBICORSOMETHING
    };

    int ChannelID(char* name);
    int ChannelID(char* name,char* context);

    // A channel contains keyframe data for a block of one or more float values
    class ChannelBuilder;
    struct Channel{
        int id;
        ValueType value_type;
        InterpolateMode interpolate_mode;
        int keyframe_count;
        float *keyframe_times;
        float *keyframe_values;
        Channel();
        Channel(int cid,ValueType channel_type,int keyframes);
        ~Channel();
        static ChannelBuilder& Builder();
    };

    struct ActiveClip;
    struct ChannelHook{
        float* value;
        char* name;
        ValueType type;

        ChannelHook(float* val,char* name,ValueType value_type);
        ~ChannelHook();
    };

    struct Target{// Contains information to hook float values to animation channels, and the current clip being applied to those values
        public:
        bool enabled;//If not enabled running clips will not modify hooked values
        Map<int,ChannelHook> hooks;
        ActiveClip *active_clip;//TODO: store as list with blending info

        Target(int channel_count);
        void AddHook(float* val,char* name,ValueType type);
        void AddHook(float* val,char* name,char* context,ValueType type);
        ~Target();
    };

    class ClipBuilder;
    class Clip{
    public:
        char *name;
        float length;
        bool loop;
        int channel_count;
        Channel *channels;
        Dictionary <int,char*> channel_names;

        Clip();
        Clip(char *anim_name, int num_channels);
        ~Clip();

        void SetChannelCount(int num_channels);
        static ClipBuilder& Builder();
    };

    class Sequence;
    struct ActiveClip
    { // Info about the currently running animation
        Clip *clip;
        Target *target;
        float elapsed_time;
        float timescale;
        Sequence* parent_sequence;

        ActiveClip(Clip* clip,Target* target);
        ActiveClip(Clip* clip,Target* target,float time,float timescale,Sequence* parent);
        ~ActiveClip();
        private: 
    };


}



// TODO: behaviors for starting clips on existings targets. Replace clip/Layer clips/Start new clip from same timepoint
// TODO: event system integration
// TODO: pause layer


// TODO: AnimationBuilder
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
    void AddKeyframe(float time, vec2 val);
    void AddKeyframe(float time, vec3 val);
    void AddKeyframe(float time, vec4 val);
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