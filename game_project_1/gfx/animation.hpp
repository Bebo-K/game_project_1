#ifndef ANIMATION_H
#define ANIMATION_H

#include <game_project_1/types/data_types.hpp>
#include <game_project_1/types/arrays.hpp>

const int ANIMATION_LAYER_COUNT = 4;

const int ANIMATION_LAYER_GLOBAL = 0;
const int ANIMATION_LAYER_SCENE = 1;
const int ANIMATION_LAYER_UI = 2;
const int ANIMATION_LAYER_USER = 3;

namespace Animation{

    enum Type : short {
        SINGLE_FLOAT,
        SINGLE_INT,
        VECTOR3,
        QUATERNION,
        VECTOR2,
        OTHER,
    };

    enum InterpolateMode : short {
        LINEAR,
        STEP,
        CUBICORSOMETHING
    };

    enum EndAction : short{
        END = 0,
        STOP = 1,
        LOOP = 2,
        GOTO = 3
    };
    
    //Pointer to a keyframe's worth of animation data. Length of datablock pointed to is known by owning AnimationChannel
    union valptr{
        float* fval;
        int* ival;
    };

    struct ChannelID{
        int id;
        char* optional_channel_name;
        ChannelID(char* channel_name);
        bool Compare(ChannelID& id);
    };


    // A channel contains keyframe data for a block of one or more float values
    struct Channel{
        ChannelID id;
        Animation::Type value_type;
        Animation::InterpolateMode interpolate_mode;
        int keyframe_count;
        float *keyframe_times;
        valptr keyframe_values;
        Channel();
        Channel(char* channel_name,Type channel_type,int keyframes);
        void SetKeyframeCount(int keyframes);
        ~Channel();
    };

    struct ChannelValue{
        ChannelID channel_id;
        Animation::Type value_type;
        valptr value;
    };

    struct ClipInfo;
    struct Target
    { // Contains information to hook float/int values to animation channels, and the current clip being applied to those values
        Array<ChannelValue> values;
        ClipInfo *active_clip;
        bool active;

        Target(int channel_count);
        ~Target();

        ChannelValue* GetValueForChannel(ChannelID& id);
    };

    class Clip{
    public:
        char *name;
        float length;
        int channel_count;
        Channel *channels;

        Clip();
        Clip(char *anim_name, int num_channels);
        ~Clip();

        void SetName(char *anim_name);
        void SetChannelCount(int num_channels);
        void DebugPrint();
    };

    struct RunningClip
    { // Info about the currently running animation
        Clip *clip;
        Target *target;
        float elapsed_time;
        float timescale;
        int end_action;
        Clip *next_clip;
        int layer;

        RunningClip();
        ~RunningClip();
    };


    struct PlayOptions
    {
        float timescale;
        EndAction end_action;
        Clip *next_anim;

        PlayOptions();
    };


    void InitManager();
    void DestroyManager();

    void StartClip(Clip *clip, Target *target);
    void StartClip(Clip *clip, Target *target, PlayOptions options);
    // TODO: behaviors for starting clips on existings targets. Replace clip/Layer clips/Start new clip from same timepoint
    // TODO: event system integration
    // TODO: pause layer
    RunningClip* GetClipInfo(Target *target);

    void StopClip(Target *target);
    void StopClip(RunningClip *target);

    void SetActiveLayer(int layer); // So we don't have to pass a layer param every time we start a clip.
    int GetActiveLayer();

    void Update(float seconds);

}



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