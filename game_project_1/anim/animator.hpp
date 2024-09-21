#ifndef ANIMATOR_H
#define ANIMATOR_H

#include <game_project_1/anim/animation_types.hpp>

class Animator{
    public:
    static void Init();
    static void Update(Timestep delta);
    static void Free();
};

namespace Animation{
    void Start(Clip *clip, Target *target);
    void StartAt(Clip* clip,Target* target,float start_time);
    void StartAt(Clip* clip,Target* target,float start_time,float timescale);
    void Queue(Clip* clip, ActiveClip* after);
    void Stop(Target *target);
    void Pause(Target *target);
}


#endif