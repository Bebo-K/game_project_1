#include <game_project_1/phys/collider.hpp>




bool CapsuleCollider::Intersects(vec3 p,CapsuleCollider* c2,vec3 p2){
    Intersects(p,c2,p2,nullptr);
}

bool CapsuleCollider::Intersects(vec3 p,CapsuleCollider *c2,vec3 p2,vec3* point){
    //TODO: does not do a swept sphere check, so cases with high velocity may be missed.

    vec3 intersection_point = p + (p2 - p)*0.5;


    float height_scale_factor = height*c2->height;
    
    vec3 collider1_global = p + offset;
    vec3 collider2_global = p2 + c2->offset;
    
    vec3 distance = (collider2_global-collider1_global);
    distance.y *= height_scale_factor;

    float dist_squared = distance.length_sqr();
    
    float combined_radius = radius+c2->radius;
    float combined_radius_squared = combined_radius*combined_radius;
    
    if(dist_squared < combined_radius_squared){
        if(point != nullptr){
            point->x = intersection_point.x;
            point->y = intersection_point.y;
            point->z = intersection_point.z;
        }
        return true;
    }

    return false;
}