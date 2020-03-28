#ifndef COLLIDER_H
#define COLLIDER_H

#include "../structs/3d_types.h"


class Collider{
	public:

    vec3	offset;
	float	height;
	float	radius;
	
	 Collider(float h, float r);
	
	 Collider(float x,float y,float z,float h,float r);
	
    bool Intersects(vec3 p,Collider c2,vec3 p2);

};



#endif