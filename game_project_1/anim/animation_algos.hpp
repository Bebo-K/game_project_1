#ifndef ANIMATION_ALGORITHMS_H
#define ANIMATION_ALGORITHMS_H

#include <game_project_1/anim/animation_types.hpp>

namespace AnimationAlgorithms{

    void UpdateChannel(Animation::ActiveClip* current_clip,Animation::Channel* channel,Animation::ChannelHook* hook);
    void Interpolate(float* from, float* to, float weight, float* values, int values_count);
    void QInterpolate(float* from, float* to, float weight, float* values, int values_count);
    void QuaternionSphericalInterpolate(quaternion* q1,quaternion* q2, float weight,quaternion* values, int values_count);
    void QuaternionLinearInterpolate(quaternion* from,quaternion* to, float weight,quaternion* values, int values_count);

}




#endif