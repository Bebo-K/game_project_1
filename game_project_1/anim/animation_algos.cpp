#include <game_project_1/anim/animation_algos.hpp>
#include <math.h>

void AnimationAlgorithms::UpdateChannel(Animation::ActiveClip* current_clip,Animation::Channel* channel,Animation::ChannelHook* hook){
    if(hook == null){
        logger::warn("Hook does not exist for animation channel '%s' of clip '%s'",
        current_clip->clip->channel_names.Get(channel->id),
        current_clip->clip->name);
        return;
    }
    if(hook->type != channel->value_type){
        logger::warn("Animation channel '%s' of clip '%s' does not have a matching value type with target channel '%s'",
        current_clip->clip->channel_names.Get(channel->id),
        current_clip->clip->name,
        hook->name);
        return;
    }

    int last_keyframe =0;
    int next_keyframe=0;
    float weight=0.0f;
    int channel_width=WidthOfAnimationType(channel->value_type);
    bool loop = current_clip->clip->loop;
    float elapsed_time = current_clip->elapsed_time;
    float clip_length = current_clip->clip->length;

    while(loop && elapsed_time > clip_length){ elapsed_time -= clip_length; }

    for(int i=0;i < channel->keyframe_count; i++){
        if(channel->keyframe_times[i] < elapsed_time){last_keyframe = i;}
    }
    next_keyframe = last_keyframe+1;

    if(next_keyframe >= channel->keyframe_count){
        if(loop){
            next_keyframe = 0;
            float last_time = channel->keyframe_times[last_keyframe];
            float next_time = clip_length;
            weight = (elapsed_time - last_time) / (next_time-last_time);
        }
        else{ weight=0.0f; }
    }
    else{
        float last_time = channel->keyframe_times[last_keyframe];
        float next_time = channel->keyframe_times[next_keyframe];
        weight = (elapsed_time - last_time) / (next_time-last_time);
        if(weight > 1.0f){weight = 1.0f;}
        if(weight < 0.0f){weight = 0.0f;}
    }

    switch(channel->value_type){
        case Animation::QUATERNION :
            QInterpolate(
                &channel->keyframe_values[last_keyframe*channel_width],
                &channel->keyframe_values[next_keyframe*channel_width],
                weight,hook->value,channel_width);
            break;
        default:
            Interpolate(
                &channel->keyframe_values[last_keyframe*channel_width],
                &channel->keyframe_values[next_keyframe*channel_width],
                weight,hook->value,channel_width);
            break;
    }
}

void AnimationAlgorithms::QuaternionLinearInterpolate(quaternion* from,quaternion* to, float weight,quaternion* values, int values_count){
    for(int i=0;i<values_count;i++){ 
        values[i].x = to[i].x*weight + from[i].x*(1.0f-weight);
        values[i].y = to[i].y*weight + from[i].y*(1.0f-weight);
        values[i].z = to[i].z*weight + from[i].z*(1.0f-weight);
        values[i].w = to[i].w*weight + from[i].w*(1.0f-weight);
        values[i].normalize();
    }
}

void AnimationAlgorithms::QuaternionSphericalInterpolate(quaternion* q1,quaternion* q2, float weight,quaternion* values, int values_count){
    for(int i=0;i<values_count;i++){ 
        quaternion from = q1[i];
        quaternion to = q2[i];

        float dot = to.dot(from);
        if(dot < 0){
            dot *= -1;
            from = from*-1;
        }
        if(dot > 0.9995f){
            QuaternionLinearInterpolate(&from,&to,weight,&values[i],1);
            return;
        } 

        float theta_0 = acosf(dot);
        float theta = theta_0*weight;
        float sin_theta = sinf(theta);
        float sin_theta_0 = sinf(theta_0);
        float s0 = cos(theta) - dot * sin_theta / sin_theta_0;
        float s1 = sin_theta / sin_theta_0;

        values[i] = from*s0 + to*s1;
    }
}

void AnimationAlgorithms::QInterpolate(float* from, float* to, float weight, float* values, int values_count){
    QuaternionSphericalInterpolate((quaternion*)from,(quaternion*)to, weight,(quaternion*)values, values_count/4);
}

void AnimationAlgorithms::Interpolate(float* from, float* to, float weight, float* values, int values_count){
    for(int i=0;i<values_count;i++){
        values[i] = to[i]*weight + from[i]*(1.0f-weight);
    }
}

