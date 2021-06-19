#include "movement.h"
#include <math.h>



void Movement::Update(Scene* scene, float delta){
    for(Entity* e:scene->entities){
        if(e->movement != null){
            MovementData* move = e->movement;
            
            bool airborne = false;
            if(e->phys_data!=null){airborne = e->phys_data->is_midair;}
            
            /* Turn speeds, I'm scrapping this.
            float current_angle = e->rotation.y;
            float move_angle = 180.0f*atan2f(move->move_goal.x, -move->move_goal.z)/PI +180.0f;
            vec3 movement_direction = move->move_goal;
            movement_direction.rotate_y(-(current_angle-move_angle));
            e->velocity.x = movement_direction.x*move->base_speed;
            e->velocity.z = movement_direction.z*move->base_speed;
            */
           
            vec2 horizontal_velocity = e->velocity.xz();
            float max_speed_sqr = move->MaxSpeed(); max_speed_sqr *= max_speed_sqr;
            float accel_speed = (airborne)?e->movement->jump_accel_speed:e->movement->accel_speed;

            horizontal_velocity.x += move->move_goal.x*accel_speed*delta;
            horizontal_velocity.y += move->move_goal.z*accel_speed*delta;

            if(horizontal_velocity.length_sqr() > max_speed_sqr){
                    horizontal_velocity.normalize();
                    horizontal_velocity.x *= move->MaxSpeed();
                    horizontal_velocity.y *= move->MaxSpeed();
            }

            e->velocity.x = horizontal_velocity.x;
            e->velocity.z = horizontal_velocity.y;

            if(horizontal_velocity.length_sqr() > 0){
                horizontal_velocity.normalize();
                e->rotation.y = atan2(-horizontal_velocity.x, horizontal_velocity.y) /(PI_OVER_180);
            }
            move->is_moving = (horizontal_velocity.length_sqr() > 0);

            if(airborne){
                //todo: coyote time
                move->can_jump = false;
            }
            //Jump hold floating
            if(move->jump_goal && move->is_jumping){
                if(e->velocity.y > 0){e->velocity.y += delta*e->movement->jump_hold_boost;}
            }
            //Jump impulse
            if(move->can_jump && move->jump_goal){
                move->can_jump=false;
                move->is_jumping=true;
                e->velocity.y = move->jump_speed;
                if(e->phys_data!=null){e->phys_data->is_midair=true;}
            }

        }
    }
}