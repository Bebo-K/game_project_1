#ifndef _3DSTRUCTS_H
#define _3DSTRUCTS_H

#include "types.h"

struct AABB{
    vec3 lo_corner;
    vec3 hi_corner;

    inline float North(){return hi_corner.z;}
    inline float North(vec3 offset){return hi_corner.z+offset.z;}

    inline float South(){return lo_corner.z;}
    inline float South(vec3 offset){return lo_corner.z+offset.z;}

    inline float East(){return hi_corner.x;}
    inline float East(vec3 offset){return hi_corner.x+offset.x;}

    inline float West(){return lo_corner.x;}
    inline float West(vec3 offset){return lo_corner.x+offset.x;}

    inline float Top(){return hi_corner.y;}
    inline float Top(vec3 offset){return hi_corner.y+offset.y;}

    inline float Bottom(){return lo_corner.y;}
    inline float Bottom(vec3 offset){return lo_corner.y+offset.y;}

    void Union(AABB box2);

};







#endif