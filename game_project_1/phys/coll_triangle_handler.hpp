#ifndef TRIANGLE_HANDLER_H
#define TRIANGLE_HANDLER_H

#include <game_project_1/phys/collision_types.hpp>
#include <game_project_1/component/phys_components.hpp>

//Handles collision against level triangles.
namespace TriangleHandler{

	const float FLOOR_SHUNT_HEIGHT_RATIO = 0.1f;
    //FLOOR_SHUNT_HEIGHT_RATIO is the ratio of an entity's hight that is "knee high", allowing them to climb small ledges without jumping
	const float EDGE_RAD_SHRINK = 0.8f;
    //EDGE_RAD_SHRINK is how much we shrink our collision bubble, horizontally, when looking for edge collisions. It prevents some odd edge case collisions.
	const float MIN_EDGE_NORMAL_COS  = 0.70710678118f;
    //MIN_EDGE_NORMAL_COS prevents the edge of a single wall from pushing us more than X degrees away from the way the wall actually faces.

    bool           CheckTriangleBounds(vec3 step_position,Triangle triangle);
    CollisionList* DoCollision(PhysBody* body,CollisionSurface* surface,vec3 step_position,Ellipse_t hitsphere,Triangle triangle);
    CollisionList* HandleFloorCase(PhysBody* body,CollisionSurface* surface,vec3 step_position,Ellipse_t hitsphere,Triangle triangle);
    CollisionList* HandleWallCase(PhysBody* body,CollisionSurface* surface,vec3 step_position,Ellipse_t hitsphere,Triangle triangle);
    CollisionList* HandleCeilingCase(PhysBody* body,CollisionSurface* surface,vec3 step_position,Ellipse_t hitsphere,Triangle triangle);
}


#endif