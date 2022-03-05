#ifndef COLLIDER_H
#define COLLIDER_H

#include <game_project_1/types/3d_types.hpp>


class CapsuleCollider{
	public:

    vec3	offset;
	float	height;
	float	radius;
	
	CapsuleCollider(float h, float r);
	
	CapsuleCollider(float x,float y,float z,float h,float r);
	
    bool Intersects(vec3 p,CapsuleCollider c2,vec3 p2);

};



#endif