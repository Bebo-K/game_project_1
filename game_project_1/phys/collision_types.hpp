#ifndef COLLISION_TYPES_H
#define COLLISION_TYPES_H

#include <game_project_1/types/math_types.hpp>
#include <game_project_1/types/data_types.hpp>


struct Ellipse_t{
    float height;
    float radius;

    Ellipse_t(float h, float w);
};

struct Triangle{    
	vec3	verts[3];//vtx positions
	vec3	edges[3];//edge vectors
	Plane	face;

    Triangle();
	Triangle(Triangle& t2);
	void operator=(Triangle* t2);

    void SetFromVertices(float vertices[9]);
	
	//vec3 ClosestPoint(vec3 position);
	//vec3 ClosestPoint(vec3 position,vec3* out_contact_normal);
    bool Intersects(vec3 position,vec3* out_intersection_point);
	vec3 ClosestPointToPlane(vec3 center);
	vec3 ClosestEdgePoint(vec3 position);

	bool PointInTriangle(vec3 point);
    bool IsZeroArea();
	
	static bool SameSide(vec3 point,vec3 reference,vec3 origin,vec3 axis);
	static vec3 ClosestPointOnEdge(vec3 point,vec3 v1,vec3 v2);
};


#endif