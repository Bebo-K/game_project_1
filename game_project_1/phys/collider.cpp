#include <game_project_1/phys/collider.hpp>
#include <game_project_1/types/math_types.hpp>
#include <math.h>


ShapeCollider::ShapeCollider(){
    shape=Collider::SPHERE;
    center_offset={0,0,0};
    scale.x = scale.y = scale.z = 0;
    turn_offset=0;
}
ShapeCollider::ShapeCollider(ShapeCollider* c2){
    shape=c2->shape;
    center_offset=c2->center_offset;
    scale=c2->scale;
    turn_offset=c2->turn_offset;
}
ShapeCollider::ShapeCollider(vec3 center,float r){
    shape=Collider::SPHERE;
    center_offset=center;
    scale.x = scale.y = scale.z = r;
    turn_offset=0;
}
ShapeCollider::ShapeCollider(vec3 center,float h, float r){
    shape=Collider::CAPSULE;
    center_offset=center;
    scale.x = scale.z = r;scale.y = h;
    turn_offset=0;
}
ShapeCollider::ShapeCollider(vec3 center,vec3 size){
    shape=Collider::AABB;
    center_offset=center;
    scale=size;
    turn_offset=0;
}
ShapeCollider::ShapeCollider(vec3 center,float h,float r,float arc_angle,float turn){
    shape=Collider::ARC;
    center_offset=center;
    scale.x = r, scale.y = h, scale.z= arc_angle;
    turn_offset=turn;
}

using namespace Collider;
bool ShapeCollider::Intersects(Location base, vec3 base_scale,ShapeCollider *c2,Location base2, vec3 base2_scale){
            return Intersects(base,base_scale,c2,base2,base2_scale,nullptr);}


bool CapsuleIntersect(vec3 size_1,vec3 size_2,vec3 offset,vec3& intersect_point){
    float horizontal_rad_sqr = (size_1.x+size_2.x)*(size_1.x+size_2.x), offset_xz_sqr = offset.xz().length_sqr();
    if(offset_xz_sqr <= horizontal_rad_sqr){
        intersect_point = offset.normalized() * sqrtf((horizontal_rad_sqr + offset_xz_sqr) / 2);
    }else return false;
    FloatRange h1(0,size_1.y),h2(offset.y,offset.y + size_2.y);
    if(h1.Overlaps(h2)){intersect_point.y = h1.Overlap_Center(h2);} else return false;
    return true;
}

bool AABBIntersect(vec3 size_1,vec3 size_2,vec3 offset,vec3& intersect_point){
    FloatRange w1(-size_1.x/2,size_1.x/2),w2(offset.x - (size_2.x/2),offset.x + (size_2.x/2));
    if(w1.Overlaps(w2)){intersect_point.x = w1.Overlap_Center(w2);} else return false;

    FloatRange h1(-size_1.y/2,size_1.y/2),h2(offset.y - (size_2.y/2),offset.y + (size_2.y/2));
    if(h1.Overlaps(h2)){intersect_point.y = h1.Overlap_Center(h2);} else return false;

    FloatRange d1(-size_1.z/2,size_1.z/2),d2(offset.z - (size_2.z/2),offset.z + (size_2.z/2));
    if(d1.Overlaps(d2)){intersect_point.z = d1.Overlap_Center(d2);} else return false;
    return true;
}

bool SphereIntersect(vec3 size_1,vec3 size_2,vec3 offset,vec3& intersect_point){
    float rad_sqr = (size_1.x+size_2.x)*(size_1.x+size_2.x), offset_len_sqr = offset.length_sqr();
    if(offset_len_sqr <= rad_sqr){
        intersect_point = offset.normalized() * sqrtf((rad_sqr + offset_len_sqr) / 2);
    }else return false;
    return true;
}

bool ShapeCollider::Intersects(Location base, vec3 base_scale, ShapeCollider *c2,
     Location base2, vec3 base2_scale, vec3* intersect_point){
        
    vec3 intersect;

    vec3 scaled_origin_1 = base.position + (center_offset * base_scale);
    vec3 scaled_origin_2 = base2.position + (c2->center_offset * base2_scale);

    vec3 size_1 = scale;
    if(shape != ARC){size_1 = size_1 * base_scale;}
    else{size_1.x *= base_scale.x; size_1.y *= base_scale.y;}
    
    vec3 size_2 = c2->scale;
    if(c2->shape != ARC){size_2 = size_2 * base2_scale;}
    else{size_2.x *= base2_scale.x; size_2.y *= base2_scale.y;}

    vec3 offset = scaled_origin_2 - scaled_origin_1;

    bool result = false;
    switch (shape){
    case CAPSULE:
        switch (c2->shape){
        case CAPSULE:result = CapsuleIntersect(size_1,size_2,offset,intersect);break;
        case Collider::AABB:break;
        case SPHERE:break;
        case ARC:break;
        default:break;
        }break;
    case Collider::AABB:
        switch (c2->shape){
        case CAPSULE:break;
        case Collider::AABB:result = AABBIntersect(size_1,size_2,offset,intersect);break;
        case SPHERE:break;
        case ARC:break;
        default:break;
        }break;
    case SPHERE:
        switch (c2->shape){
        case CAPSULE:break;
        case Collider::AABB:break;
        case SPHERE:result = SphereIntersect(size_1,size_2,offset,intersect);break;
        case ARC:break;
        default:break;
        }break;
    case ARC:
        switch (c2->shape){
        case CAPSULE:break;
        case Collider::AABB:break;
        case SPHERE:break;
        case ARC:break;
        default:break;
        }break;
    default:break;
    }
    if(intersect_point != nullptr){intersect_point->set(intersect.x,intersect.y,intersect.z);}
    return result;
}
