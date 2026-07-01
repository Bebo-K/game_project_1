export module Interpolator;

import <math>
import Primitives;
import Transform;

export quaternion QuaternionLinearInterpolate(quaternion from,quaternion to, float weight){
    return (quaternion{
        to.x*weight + from.x*(1.0f-weight),
        to.y*weight + from.y*(1.0f-weight),
        to.z*weight + from.z*(1.0f-weight),
        to.w*weight + from.w*(1.0f-weight)}).normalized();
}

export quaternion QuaternionSphericalInterpolate(quaternion from,quaternion to, float weight){
    float dot = to.dot(from);
    if(dot < 0){
        dot *= -1;
        from = from*-1;
    }
    float theta_0 = acosf(dot);
    float theta = theta_0*weight;
    float sin_theta = sinf(theta);
    float sin_theta_0 = sinf(theta_0);
    float s0 = cos(theta) - dot * sin_theta / sin_theta_0;
    float s1 = sin_theta / sin_theta_0;

    return (from*s0 + to*s1).normalized();
}

export quaternion QInterpolate(quaternion from,quaternion to, float weight){
    float dot = to.dot(from);
    if(dot < 0){
        dot *= -1;
        from = from*-1;
    }
    if(dot > 0.9995f){
        return QuaternionLinearInterpolate(from,to,weight);
    }
    return QuaternionSphericalInterpolate(from,to,weight);
}

namespace Interpolators
{

    //multivalue interpolators used in animation system
    //nlerp, consider slerp if appropriate
    export void MultiQInterpolate(float* from, float* to, float weight, float* values, int values_count){
        for(int i=0;i<values_count;i+=4){
            quaternion res = QInterpolate({from[i],from[i+1],from[i+2],from[i+3]},{to[i],to[i+1],to[i+2],to[i+3]},weight);
            values[i]=res.x;
            values[i+1]=res.y;
            values[i+2]=res.z;
            values[i+3]=res.w;
        }
    }
    export void MultiInterpolate(float* from, float* to, float weight, float* values, int values_count){
        for(int i=0;i<values_count;i++){
            values[i] = to[i]*weight + from[i]*(1.0f-weight);
        }
    }

    //weight-based interpolator that blends [to] + [from] based on weight
    //quaternion cutoff in in degrees between the two angles
    export float WeightInterpolate(float from, float to,float weight){
        return to*weight + from*(1.0f-weight);
    }
    export quaternion WeightQInterpolate(quaternion from, quaternion to,float weight){
        return QInterpolate(from,to,weight);
    }

    //step-based interpolators that adds [step] to [from] until [to] is reached
    export float StepInterpolate(float from, float to,float step){
        if(abs(to-from) < abs(step))return to;
        return (to > from == step > 0)?  from+step : from-step;
    }
    export quaternion StepQInterpolate(quaternion from, quaternion to,float theta_step){
        float theta_dist = from.theta_difference(to);
        if(theta_dist < theta_step)return to;
        return QInterpolate(from,to, theta_step/theta_dist);
    }

    //Weight based transform interpolator
    export Transform TransformInterpolate(Transform* from, Transform* to, float weight,float pos_snap,float angle_snap,float scale_snap){
        Transform res(
            WeightInterpolate(from->x,to->x,weight),
            WeightInterpolate(from->y,to->y,weight),
            WeightInterpolate(from->z,to->z,weight),

            QInterpolate(from->rotation,to->rotation,weight),

            {WeightInterpolate(from->scale.x,to->scale.x,weight),
            WeightInterpolate(from->scale.y,to->scale.y,weight),
            WeightInterpolate(from->scale.z,to->scale.z,weight)}
        );
        if( (res.Position() - to->Position()).length() < pos_snap){
            res.x=to->x;
            res.y=to->y;
            res.z=to->z;
        }
        if( (res.scale - to->scale).length() < scale_snap){
            res.scale = to->scale;
        }
        if( res.rotation.theta_difference(to->rotation) < angle_snap){
            res.rotation = to->rotation;
        }
        res.parent=from->parent;
        return res;
    }
    //TODO step-based transform interpolator?
}