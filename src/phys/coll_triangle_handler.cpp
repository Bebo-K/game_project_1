#include "coll_triangle_handler.h"
#include "../game/system/level_collision.h"

CollisionList* TriangleHandler::DoCollision(CollisionSurface* surface,Entity* e,vec3 step_position,Ellipse_t hitsphere,Triangle triangle){
    CollisionList* ret =null;
    float plane_y_component = triangle.face.normal.y;

    CheckIfOOB(e,step_position,triangle);

    if(plane_y_component > 0.1f){
        ret= HandleFloorCase(surface,e,step_position,hitsphere,triangle);
    }
    else if(plane_y_component > -0.1f && plane_y_component < 0.1f){
        ret= HandleWallCase(surface,e,step_position,hitsphere,triangle);
    }
    else if(plane_y_component < -0.1f){
        ret= HandleCeilingCase(surface,e,step_position,hitsphere,triangle);
    }
    return ret;
}

void TriangleHandler::CheckIfOOB(Entity* e,vec3 step_position,Triangle triangle){
    if(triangle.face.normal.y*triangle.face.normal.y <= 0){return;}
    float vertical_intersect = triangle.face.YIntersect(step_position.x, step_position.z);
    if(triangle.PointInTriangle({step_position.x,vertical_intersect,step_position.z}) ){
        e->phys_data->out_of_bounds =false;	
    }
}

CollisionList* TriangleHandler::HandleFloorCase(CollisionSurface* surface,Entity* e,vec3 step_position,Ellipse_t hitsphere,Triangle triangle){
    CollisionList* ret =null;
    
    float vertical_intersect = triangle.face.YIntersect(step_position.x, step_position.z);
    float vertical_shunt = vertical_intersect - step_position.y;
    float max_up_shunt = hitsphere.height*FLOOR_SHUNT_HEIGHT_RATIO;
    float max_down_shunt =  -hitsphere.height*FLOOR_SHUNT_HEIGHT_RATIO;
    
    vec3 intersect_point = {step_position.x,vertical_intersect,step_position.z};
    if(triangle.PointInTriangle(intersect_point)){
        e->phys_data->out_of_bounds =false;	
        if(vertical_shunt >= max_down_shunt && vertical_shunt <= max_up_shunt){
            //If we're embedded or very close to the ground, register a floor collision 
            //(We start with a shunt-less ground collision when airborne to prevent the entity from prematurely snapping to the ground)
            ret = LevelCollision::GrabCollisionSlot();
            if(ret == null)return null;
            ret->surface = surface;
            ret->shunt={0,0,0};
            ret->normal = triangle.face.normal;
            ret->floor_distance = vertical_shunt;
            ret->flags = LevelCollisionFlag::FLOOR;
            ret->next=null;
            //if we're close to, on, or under the floor, and not going up, it's safe to mark us as grounded.
            if(e->velocity.y <= 0){e->phys_data->is_midair=false;}
            //If we're underneath by less than our max up-shunt height, move us up and out and set us grounded
            if(vertical_shunt >= 0 && vertical_shunt <= max_up_shunt){
                ret->flags |= LevelCollisionFlag::CANCEL_VELOCITY;
                ret->velocity_cancel = {0,1,0};
                ret->shunt.y = vertical_shunt;
            }
            //If we're already grounded and floating over the floor by less than our max shunt height, move us down to ground.
            else if(!e->phys_data->is_midair && vertical_shunt < 0 && vertical_shunt >= max_down_shunt){
                ret->flags |= LevelCollisionFlag::CANCEL_VELOCITY;
                ret->velocity_cancel = {0,1,0};
                ret->shunt.y = vertical_shunt;
            }
        }
        else{
            ret=null;
        }
    }
    return ret;
}
		
CollisionList* TriangleHandler::HandleWallCase(CollisionSurface* surface,Entity* e,vec3 step_position,Ellipse_t hitsphere,Triangle triangle){
    CollisionList* ret =null;
    
    vec3 center = step_position;
    center.y += (hitsphere.height/2);
    
    float bottom_cutoff = step_position.y;
    float top_cutoff = step_position.y+hitsphere.height;

    //Keeps grounded entitites from colliding with "knee high" walls before floor
    //if(!e->phys_data->is_midair){
    bottom_cutoff += hitsphere.height*FLOOR_SHUNT_HEIGHT_RATIO;
    //}
    //if(e->phys_data->is_midair){
    //    top_cutoff -= hitsphere.height*0.25;
    //}
    

    FloatRange vertical_cutoff_range(bottom_cutoff,top_cutoff);

    //Find the clostest point on the triangle plane to our position.
    vec3 plane_intersect = triangle.ClosestPointToPlane(center);
    vec3 intersect_offset = plane_intersect-center;
    vec3 intersect_horizontal_offset = intersect_offset.horizontal();
    //First check if we're even near the plane. In both plane + edge cases it should be within our cylinder's radius+height.
     if(intersect_horizontal_offset.length_sqr() < hitsphere.radius*hitsphere.radius &&  vertical_cutoff_range.Contains(plane_intersect.y)){
         //Next, check if the plane point lies in the triangle
        if(triangle.PointInTriangle(plane_intersect)){
                float horizontal_offset = intersect_horizontal_offset.length();
                ret = LevelCollision::GrabCollisionSlot();
                if(ret == null)return null;
                ret->surface=surface;
                ret->velocity_cancel = triangle.face.normal.horizontal();
                ret->shunt = ret->velocity_cancel * (hitsphere.radius-horizontal_offset);
                ret->normal = triangle.face.normal;
                ret->flags = LevelCollisionFlag::WALL | LevelCollisionFlag::CANCEL_VELOCITY;
                ret->next=null;
        }
        //If not, then check if there's a point on the triangle's edges.
        else{
            vec3 edge_point = triangle.ClosestEdgePoint(center);
            if(vertical_cutoff_range.Contains(edge_point.y)){//All edges are on the plane, so no need to recheck if the point is in our radius.
                vec3 edge_offset = center - edge_point;
                vec3 edge_horizontal_offset = edge_offset.horizontal();
                vec3 edge_normal = edge_horizontal_offset.normalized();
                float edge_normal_cos = edge_normal.dot(triangle.face.normal);
                
                //We shrink our horizontal collision radius in edge cases. This mitigates 2 side effects somewhat:
                //1.) Colliding with interior edges when sliding along a wall.
                //2.) Getting pushed inward from corners when rounding them.
                float effective_radius = hitsphere.radius*EDGE_RAD_SHRINK;
                //This shrinks our effective radius further in the case we're falling off a ledge. It smoothes out the push away from the wall we experience when falling next to a ledge.
                //TODO: This case here would also be a good condition to proc "hanging off wall" animations
                if(e->phys_data->is_midair && edge_offset.y >= 0 && edge_offset.y < hitsphere.height/2 ){				
                    effective_radius *= 1.0f-(2*edge_offset.y/hitsphere.height);
                }
                
                if(edge_horizontal_offset.length_sqr() < effective_radius*effective_radius && edge_normal_cos > MIN_EDGE_NORMAL_COS){
                    
                    float shunt_amount = (effective_radius-edge_horizontal_offset.length())*edge_normal_cos;
                    ret = LevelCollision::GrabCollisionSlot();
                    if(ret == null)return null;
                    ret->surface=surface;
                    ret->velocity_cancel = edge_normal.horizontal();
                    ret->shunt = ret->velocity_cancel * shunt_amount;
                    ret->normal = triangle.face.normal;
                    ret->flags = LevelCollisionFlag::WALL| LevelCollisionFlag::CANCEL_VELOCITY;
                }
            }  
        }	    
    }
    return ret;
}
	
CollisionList* TriangleHandler::HandleCeilingCase(CollisionSurface* surface,Entity* e,vec3 step_position,Ellipse_t hitsphere,Triangle triangle){
    CollisionList* ret =null;
    
    vec3 top = step_position;
    top.y += hitsphere.height;
    vec3 bottom = step_position;
    bottom.y += hitsphere.height*FLOOR_SHUNT_HEIGHT_RATIO;

    vec3 collision_point = step_position;
    collision_point.y = triangle.face.YIntersect(collision_point.x, collision_point.z);

    //If we collide with a ceiling, our definition of a successful response is to shunt our player downward so that the top of their hitsphere is no longer above the triangle plane.
    if(triangle.PointInTriangle(collision_point)){
        float top_dist = triangle.face.DistanceTo(top);
        float bottom_dist = triangle.face.DistanceTo(bottom);
         
        //if hitsphere top and bottom are on different sides of the plane.
        if(top_dist*bottom_dist < 0){           
            ret = LevelCollision::GrabCollisionSlot();
            if(ret == null)return null;
            ret->surface=surface;
            //ret->velocity_cancel={0,-1,0};
            ret->shunt = {0,top.y-collision_point.y,0};
            ret->normal = triangle.face.normal;
            ret->flags = LevelCollisionFlag::CEILING;
            ret->next=null;
        }
    }	
    return ret;
}