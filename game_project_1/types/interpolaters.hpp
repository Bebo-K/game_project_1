#ifndef INTERPOLATORS_H
#define INTERPOLATORS_H


#include <game_project_1/types/primitives.hpp>
#include <game_project_1/types/transform.hpp>

namespace Interpolators
{

    //multivalue interpolators used in animation system
    //nlerp, consider slerp if appropriate
    void MultiQInterpolate(float* from, float* to, float weight, float* values, int values_count);
    void MultiInterpolate(float* from, float* to, float weight, float* values, int values_count);

    //weight-based interpolator that blends [to] + [from] based on weight
    //quaternion cutoff in in degrees between the two angles
    float WeightInterpolate(float from, float to,float weight);
    quaternion WeightQInterpolate(quaternion from, quaternion to,float weight);
    
    //step-based interpolators that adds [step] to [from] until [to] is reached
    float StepInterpolate(float from, float to,float step);
    float StepQInterpolate(quaternion from, quaternion to,float theta_step);

    //Weight based transform interpolator
    Transform TransformInterpolate(Transform* from, Transform* to, float weight,float pos_snap,float angle_snap,float scale_snap);
    //TODO step-based transform interpolator?



};








#endif