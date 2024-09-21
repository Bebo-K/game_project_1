#ifndef ANIMATION_SEQUENCE_H
#define ANIMATION_SEQUENCE_H
#include <game_project_1/anim/animation_types.hpp>

namespace Animation{
    class Sequence{
        public:
        int a;
        virtual void OnAnimationFinish(ActiveClip* finished,ActiveClip* next)=0;
        virtual void OnHint(char* hint)=0;
    };

    class ExampleSequence : public Sequence{
        void OnAnimationFinish(ActiveClip* finished,ActiveClip* next);
        void OnHint(char* hint);
    };
}






#endif