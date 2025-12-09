#ifndef COLLIDER_H
#define COLLIDER_H

#include <game_project_1/types/transform.hpp>
#include <game_project_1/types/math_types.hpp>
#include <game_project_1/gfx/camera.hpp>


enum class ColliderShape{
	CYLINDER=0, //(Axis-aligned) size.x = radius, size.y = height, size.z is ignored (=size.x)
	CAPSULE=1, //(Unaligned, two connected spheres)  size.x = radius, size.y = height, size.z is ignored (=size.x)
	AABB=2,		//(Axis-aligned bounding box) size = full size in each dimension
	SPHERE=3, //(Alignment doesn't matter) size.x = radius
	ARC=4, //(Vertically aligned, only y rotation matters) size.x = radius, size.y = height, size.z= arc length 
};

class ShapeCollider{
	public:
	ColliderShape	shape;
	Transform		transform;
	vec3 			size;

	ShapeCollider();//Empty constructor is zero size sphere
	ShapeCollider(Transform* parent,vec3 center,float r);//assumes SPHERE
	ShapeCollider(Transform* parent,vec3 center,float h, float r);//assumes CAPSULE
	ShapeCollider(Transform* parent,vec3 center,vec3 size);//assumes AABB
	ShapeCollider(Transform* parent,vec3 center,float h,float r,float arc_angle,vec3 rotation);//assumes ARC

	ShapeCollider(Transform* parent,ShapeCollider* copy);//Copy constructor
	
    bool Intersects(ShapeCollider *c2);
    bool Intersects(ShapeCollider *c2,vec3* intersect_point);

	void Draw(Camera* cam);
};



#endif