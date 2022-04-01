#ifndef COLLISION_TYPES_H
#define COLLISION_TYPES_H

#include <game_project_1/types/math_types.hpp>
#include <game_project_1/types/data_types.hpp>


struct Ellipse_t{
    float height;
    float radius;

    Ellipse_t(float h, float w);
};

namespace SurfaceType{
    const int SOLID=0;
    const int NOCLIP=1;
    const int TRIGGER=2;
    const int WATER=3;
    const int DAMAGE=4;
    const int DEATH=5;
};

namespace SurfaceMaterial{
    const int NONE=0;
    const int STONE=1;
};

struct CollisionSurface{
    int     type;
    int     material;

    int     exit_id;
    int     exit_entr_id;

    int     damage;

    byte*   metadata;
    int     metadata_len;

    CollisionSurface();
    CollisionSurface(char* type_name,char* material_name);
};

//Level Collision linked list
namespace LevelCollisionFlag{
    const char NONE=0;
    const char WALL=1;
    const char FLOOR=2;
    const char CEILING=4;
    const char WALL_CASE=7;
    const char CANCEL_VELOCITY=8;
};
struct CollisionList {
    CollisionSurface*   surface;
    vec3                normal;//Tangent to collision
    vec3                velocity_cancel;//Direction to stop moving in
    vec3                shunt;//Any movement we need to make to un-clip
    char                flags;
    bool                solid;
    float               floor_distance;//TODO what is this?
    CollisionList*      next;
    CollisionList();
    CollisionList* last();

    static CollisionList* Append(CollisionList* first,CollisionList* new_coll);
};

struct Triangle{    
	vec3	verts[3];//vtx positions
	vec3	edges[3];//edge vectors
	Plane	face;

    Triangle();

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