#ifndef MATH_TYPES_H
#define MATH_TYPES_H

#include <game_project_1/types/primitives.hpp>

//Transforms represent a transformation matrix consisting of a translation, rotation, and scale
// They should be used for data close to the rendering pipeline, like skeleton animations
struct Transform{
    float x,y,z;
    quaternion rotation;
    vec3 scale;

    vec3 Position();
    void Clear();
};

// Representation of an orientation in world space- generally scale is constant or managed separately
// rotation.x = pitch of the object (tilt forward being positive axis)
// rotation.y = turn of the object (clockwise being positive axis)
// rotation.z = yaw of the object (towards object's right side being positive axis)
// Where Transform.rotation has no such mappings
struct Location{
    float x,y,z;
    vec3 rotation;

    Location();
    vec3 Position();
    Location(vec3 pos,vec3 rot);
    Transform ToTransform();
};

struct LinearGradient{
    float a,b;

    LinearGradient();
    LinearGradient(float v1,float v2);
    float Percent(float value);//returns a number that represents where value is on the gradient. 0 to 1.0 lies on gradient, negative and results > 1.0 lie outside.
    float ScaleTo(float value,LinearGradient g2);//Scale value to this gradient, then return that percentage value of g2.
    float Value(float percent);//returns the point at percent in the gradient between a and b (a*percent + b*(1.0-percent))
};


//FloatGradient where min always < max
struct FloatRange{
    float min;
    float max;

    FloatRange();
    FloatRange(float min,float max);
    float Clamp(float f);
    float Average();
    bool Contains(float f);
    bool Overlaps(FloatRange r2);
    FloatRange Clamp(FloatRange r2);
    FloatRange Union(FloatRange r2);
    float ScaleTo(float f1, FloatRange r1);
    float ScaleTo(float f1, LinearGradient r1);
    float Overlap_Center(FloatRange r2);
};


struct AABB{
    vec3 lo_corner;
    vec3 hi_corner;

    inline float North(){return hi_corner.z;}
    inline float North(vec3 offset){return hi_corner.z+offset.z;}
    inline void SetNorth(float top){hi_corner.z=top;if(lo_corner.z > hi_corner.z){lo_corner.z=hi_corner.z;}}

    inline float South(){return lo_corner.z;}
    inline float South(vec3 offset){return lo_corner.z+offset.z;}
    inline void SetSouth(float bottom){lo_corner.z=bottom;if(hi_corner.z < lo_corner.z){hi_corner.z=lo_corner.z;}}

    inline float East(){return hi_corner.x;}
    inline float East(vec3 offset){return hi_corner.x+offset.x;}
    inline void SetEast(float east){hi_corner.x=east;if(lo_corner.x > hi_corner.x){lo_corner.x=hi_corner.x;}}

    inline float West(){return lo_corner.x;}
    inline float West(vec3 offset){return lo_corner.x+offset.x;}
    inline void SetWest(float west){lo_corner.x=west;if(hi_corner.x < lo_corner.x){hi_corner.x=lo_corner.x;}}

    inline float Top(){return hi_corner.y;}
    inline float Top(vec3 offset){return hi_corner.y+offset.y;}
    inline void SetTop(float top){hi_corner.y=top;if(lo_corner.y > hi_corner.y){lo_corner.y=hi_corner.y;}}

    inline float Bottom(){return lo_corner.y;}
    inline float Bottom(vec3 offset){return lo_corner.y+offset.y;}
    inline void SetBottom(float bottom){lo_corner.y=bottom;if(hi_corner.y < lo_corner.y){hi_corner.y=lo_corner.y;}}

    
    void Union(AABB box2);
    void Move(vec3 offset);
    bool PointInside(vec3 point);
    bool EllipseInside(vec3 point,vec3 ellipse);
    bool ContainsCircle_XZ(vec3 position, float width);
    vec3 Center();
    void EncompassPoint(vec3 point);
};

struct Plane{
    public:
    vec3 normal;
    float distance;

    Plane();
    Plane(float a,float b,float c,float d);
    //plane(vec3 tangent_point,vec3 normal);
    void SetFromPointNormal(vec3 tangent_point,vec3 normal);
    float DistanceTo(vec3 pos);
    void Invert();
    float YIntersect(float x,float z);

};


#endif