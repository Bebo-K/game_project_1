#ifndef COLLIDER_H
#define COLLIDER_H

#include <game_project_1/types/3d_types.hpp>

namespace Collider{
	enum Shape{
		CAPSULE=0, //scale.z is ignored (=scale.x)
		AABB=1,
		SPHERE=2,
		ARC=3 //scale.x = radius, scale.y = height, scale.z= arc length 
	};
};

class ShapeCollider{
	public:
	Collider::Shape	shape;
    vec3			center_offset;
	vec3			scale;
	vec3			rotation;

	ShapeCollider();//Empty constructor is zero size sphere
	ShapeCollider(ShapeCollider* c2);//copy constructor
	ShapeCollider(vec3 center,float r);//assumes SPHERE
	ShapeCollider(vec3 center,float h, float r);//assumes CAPSULE
	ShapeCollider(vec3 center,vec3 size);//assumes AABB
	ShapeCollider(vec3 center,float h,float r,float arc_angle,vec3 rotation);//assumes ARC
	
    bool Intersects(Location base, vec3 base_scale, ShapeCollider *c2, Location base2, vec3 base2_scale);
    bool Intersects(Location base, vec3 base_scale, ShapeCollider *c2, Location base2, vec3 base2_scale,vec3* intersect_point);
};



#endif