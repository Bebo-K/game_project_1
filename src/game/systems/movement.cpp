#include "movement.h"
#include <math.h>



void Movement::Update(Scene* scene, float delta){
    for(Entity* e:scene->entities){
        if(e->movement != null){
            MovementData* move = e->movement;
            
            /* Turn speeds, I'm scrapping this.
            float current_angle = e->rotation.y;
            float move_angle = 180.0f*atan2f(move->move_goal.x, -move->move_goal.z)/PI +180.0f;
            vec3 movement_direction = move->move_goal;
            movement_direction.rotate_y(-(current_angle-move_angle));
            e->velocity.x = movement_direction.x*move->base_speed;
            e->velocity.z = movement_direction.z*move->base_speed;
            */
           
            //TODO: Ramp-up
            e->velocity.x = move->move_goal.x*move->base_speed;
            e->velocity.z = move->move_goal.z*move->base_speed;
            move->is_moving = (move->move_goal.length_sqr() > 0);

            if(move->can_jump && move->jump_goal){
                move->can_jump=false;
                move->is_jumping=true;
                e->velocity.y = move->jump_speed;
                if(e->phys_data!=null){e->phys_data->is_midair=true;}
            }
        }
    }
}