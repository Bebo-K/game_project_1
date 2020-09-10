#ifndef MATH_TYPES_H
#define MATH_TYPES_H

#include "3d_types.h"

struct float_range
{
    float min;
    float max;

    float_range();
    float_range(float min,float max);
    float Clamp(float f);
    float Average();
    bool Contains(float f);
    bool Overlaps(float_range r2);
    float_range Clamp(float_range r2);
    float_range Union(float_range r2);
    float ScaleTo(float f);
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

struct plane{
    public:
    vec3 normal;
    float distance;

    plane();
    plane(float a,float b,float c,float d);
    //plane(vec3 tangent_point,vec3 normal);
    void SetFromPointNormal(vec3 tangent_point,vec3 normal);
    float DistanceTo(vec3 pos);
    void Invert();
    float YIntersect(float x,float z);

};


#endif