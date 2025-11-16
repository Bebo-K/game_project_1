#include <game_project_1/phys/intersect.h>
#include <game_project_1/types/math_types.hpp>
#include <math.h>



bool CylinderVCylinderIntersect(float r1, float h1,float r2, float h2,vec3 offset,vec3& intersect_point){
    if(offset.xz().length_sqr() < r1*r1 + r2*r2 && abs(offset.y) < (h1 + h2)/2){
        vec2 intersect_xz = offset.xz().normalized()*r1;
        float intsersect_y = FloatRange(-h1/2,h1/2).Overlap_Center(FloatRange(offset.y - h2/2, offset.y + h2/2));
        intersect_point = {intersect_xz.x,intsersect_y,intersect_xz.y};

        vec3 intersect_dir = offset;
        intersect_dir.y = 0;
        intersect_point = intersect_dir.normalized() * sqrtf((offset.xz().length_sqr() + (r1+r2)*(r1+r2))/2);
        FloatRange h_range_1(-h1/2,h1/2), h_range_2(offset.y - h2/2, offset.y + h2/2);
        intersect_point.y = h_range_1.Overlap_Center(h_range_2);
        return true;
    }
    return false;
}

bool AABBVAABBIntersect(vec3 size_1,vec3 size_2,vec3 offset,vec3& intersect_point){
    FloatRange x_1(-size_1.x/2,size_1.x/2),x_2(offset.x - (size_2.x/2),offset.x + (size_2.x/2));
    FloatRange y_1(-size_1.y/2,size_1.y/2),y_2(offset.y - (size_2.y/2),offset.y + (size_2.y/2));
    FloatRange z_1(-size_1.z/2,size_1.z/2),z_2(offset.z - (size_2.z/2),offset.z + (size_2.z/2));

    if(x_1.Overlaps(x_2) && y_1.Overlaps(y_2) && z_1.Overlaps(z_2)){
        intersect_point.x = x_1.Overlap_Center(x_2);
        intersect_point.y = y_1.Overlap_Center(y_2);
        intersect_point.z = z_1.Overlap_Center(z_2);
        return true;
    }
    return false;
}

bool SphereVSphereIntersect(float r1, float r2,vec3 offset,vec3& intersect_point){
    float combined_r_sqr = (r1+r2)*(r1+r2), offset_len_sqr = offset.length_sqr();
    if(offset_len_sqr <= combined_r_sqr){
        intersect_point = offset.normalized() * r1;
        return true;
    }
    return false;
}

bool CapsuleVCapsuleIntersect(vec3 size_1,vec3 size_2,quaternion rotation_1, quaternion rotation_2, vec3 offset,vec3& intersect_point){
    //TODO
    return false;
}

bool ArcVArcIntersect(vec3 size_1,vec3 size_2,float y_rotation_1, float y_rotation_2, vec3 offset,vec3& intersect_point){
    //TODO
    return false;
}

bool ArcVSphereIntersect(vec3 size_1,float r2,float y_rotation_1, vec3 offset,vec3& intersect_point){
    //TODO
    return false;
}

bool ArcVCylinderIntersect(vec3 size_1,vec3 size_2,float y_rotation_1, vec3 offset,vec3& intersect_point){
    //TODO
    return false;
}
