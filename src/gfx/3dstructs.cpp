#include "3dstructs.h"


void AABB::Union(AABB box2){
    if(box2.East() > East()){hi_corner.x = box2.hi_corner.x;}
    if(box2.Top() > Top()){hi_corner.y = box2.hi_corner.y;}
    if(box2.North() > North()){hi_corner.z = box2.hi_corner.z;}
    if(box2.West() < West()){lo_corner.x = box2.lo_corner.x;}
    if(box2.Bottom() < Bottom()){lo_corner.y = box2.lo_corner.y;}
    if(box2.South() < South()){lo_corner.z = box2.lo_corner.z;}
}