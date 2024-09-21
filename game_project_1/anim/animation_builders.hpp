#ifndef ANIMATION_BUILDERS_H
#define ANIMATION_BUILDERS_H

#include <game_project_1/anim/animation_types.hpp>

namespace Animation{
    struct KeyframeBuilder{
        float time;
        vec4 value;
        KeyframeBuilder(float time,vec4 value);
    };
    
    class ChannelBuilder{
        private:
        int id;
        ValueType value_type;
        InterpolateMode interpolate_mode;
        List<KeyframeBuilder> keyframes;

        public:
        ChannelBuilder();
        ChannelBuilder& ID(int cid);
        ChannelBuilder& Type(ValueType type);
        ChannelBuilder& InterpolateMode(InterpolateMode mode);
        ChannelBuilder& Keyframe(float time, float val);
        ChannelBuilder& Keyframe(float time, vec2 val);
        ChannelBuilder& Keyframe(float time, vec3 val);
        ChannelBuilder& Keyframe(float time, vec4 val);
        ChannelBuilder& Keyframe(float time, float* values);
        Channel* Build();
        void BuildTo(Channel* target);
    };

    class ClipBuilder{
        private:
        char *name;
        float length;
        bool loop;
        List<Channel> channels;

        public:
        ClipBuilder();
        ClipBuilder& Name(char* name);
        ClipBuilder& Duration(float length);
        ClipBuilder& Loop(bool loop);
        ClipBuilder& AddChannel(ChannelBuilder& channelbuilder);
        Clip* Build();
        void BuildTo(Clip* target);
    };
}

#endif