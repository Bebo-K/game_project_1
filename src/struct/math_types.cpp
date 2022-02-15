#include "math_types.h"
#include "../log.h"
#include <math.h>

FloatRange FloatRange::Union(FloatRange r2){
    return FloatRange( (min < r2.min)?min:r2.min , (max > r2.max)?max:r2.max );
}

FloatRange::FloatRange(){
    min = max= 0;
}
FloatRange::FloatRange(float a,float b){
    min = (a < b)? a:b;
    max = (a < b)? b:a;
    if(a==b){
        logger::exception("Invalid float range: [ %f , %f ] - Range cannot be empty.\n",a,b);
        a=0.0f;b=1.0f;
    }
}
float FloatRange::Clamp(float f){
    return (f<min)?min:((f>max)?max:f);
}
float FloatRange::Average(){
    return (min+max)/2.0f;
}
FloatRange FloatRange::Clamp(FloatRange range){
    return {(range.min < min)?range.min:min,(range.max > max)?range.max:max};
}

bool FloatRange::Overlaps(FloatRange range){
    if(min >= range.min && min <= range.max)return true;
    if(max >= range.min && max <= range.max)return true;
    if(range.min >= min && range.min <= max)return true;
    if(range.max >= min && range.min <= max)return true;
    return false;
}
bool FloatRange::Contains(float f){return(f >= min && f <= max);}
float FloatRange::ScaleTo(float f1, FloatRange r1){//scales float f1 from range r1 to this range.
    float f0 = (f1-r1.min)/(r1.max-r1.min);
    return f0*(max-min) + min;
}


void AABB::Union(AABB box2){
    if(box2.East() > East()){hi_corner.x = box2.hi_corner.x;}
    if(box2.Top() > Top()){hi_corner.y = box2.hi_corner.y;}
    if(box2.North() > North()){hi_corner.z = box2.hi_corner.z;}
    if(box2.West() < West()){lo_corner.x = box2.lo_corner.x;}
    if(box2.Bottom() < Bottom()){lo_corner.y = box2.lo_corner.y;}
    if(box2.South() < South()){lo_corner.z = box2.lo_corner.z;}
}

void AABB::Move(vec3 offset){
    lo_corner = lo_corner+offset;
    hi_corner = hi_corner+offset;
}
bool AABB::PointInside(vec3 point){
    float T = Top();
    float B = Bottom();
    float N = North();
    float S = South();
    float E = East();
    float W = West();
    
    if(	point.x > E &&  point.x < W &&
        point.y > B &&	point.y < T &&
        point.z > S &&	point.z < N) {
        return true;
    }
    
    return false;
}

bool AABB::EllipseInside(vec3 point,vec3 ellipse){
    float T = Top();
    float B = Bottom();
    float N = North();
    float S = South();
    float E = East();
    float W = West();
    
    if(	point.x+ellipse.x > W &&	point.x-ellipse.x < E &&
        point.y+ellipse.y > B &&	point.y-ellipse.y < T &&
        point.z+ellipse.z > S &&	point.z-ellipse.z < N) {
        return true;
    }
    return false;
}

bool AABB::ContainsCircle_XZ(vec3 position, float width) {
    float N = North();
    float S = South();
    float E = East();
    float W = West();
    
    if(	position.x+width >= W &&	position.x-width <= E &&
        position.z+width >= S &&	position.z-width <= N) {
        return true;
    }
    return false;
}

vec3 AABB::Center(){
    return {(hi_corner.x+lo_corner.x / 2.0f),(hi_corner.y+lo_corner.y / 2.0f),(hi_corner.z+lo_corner.z / 2.0f)};
}

void AABB::EncompassPoint(vec3 point){
    if(!PointInside(point)){
        if(point.x > East()){SetEast(point.x);}
        if(point.x < West()){SetWest(point.x);}
        if(point.y > Top()){SetTop(point.y);}
        if(point.y < Bottom()){SetBottom(point.y);}
        if(point.z > North()){SetNorth(point.z);}
        if(point.z < South()){SetSouth(point.z);}
    }
}

Plane::Plane(){
    normal = {0,1,0};
    distance = 0;
}

Plane::Plane(float a, float b, float c, float d){
    normal = {a,b,c};
    normal = normal.normalized();//just to double-check
    distance = d;
}

/*
Plane::Plane(vec3 tangent_point,vec3 normal_vec){
    normal = normal_vec.normalized();
    distance = -(normal.x*tangent_point.x + normal.y*tangent_point.y + normal.z*tangent_point.z);
}
*/

void Plane::SetFromPointNormal(vec3 tangent_point,vec3 normal_vec){
    normal = normal_vec.normalized();
    distance = -(normal.x*tangent_point.x + normal.y*tangent_point.y + normal.z*tangent_point.z);
}
	
float Plane::DistanceTo(vec3 pos){
    return normal.x*pos.x + normal.y*pos.y + normal.z*pos.z + distance;
}

void Plane::Invert() {
    distance *= -1;
    normal.x *= -1;
    normal.y *= -1;
    normal.z *= -1;
}

float Plane::YIntersect(float X, float Z){
    if(normal.y*normal.y == 0){
        return NAN;
    }
    return (normal.x*X + normal.z*Z + distance)/-normal.y;
}