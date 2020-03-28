#include "collider.h"



bool Collider::Intersects(vec3 p,Collider c2,vec3 p2){
    //Note: does not do a swept sphere check, so cases with high velocity may be missed.
    float height_scale_factor = height*c2.height;
    
    vec3 collider1_global = p + offset;
    vec3 collider2_global = p2 + c2.offset;
    
    vec3 distance = (collider2_global-collider1_global);
    distance.y *= height_scale_factor;

    float dist_squared = distance.length_sqr();
    
    float combined_radius = radius+c2.radius;
    float combined_radius_squared = combined_radius*combined_radius;
    
    if(dist_squared < combined_radius_squared){return true;}

    return false;
}