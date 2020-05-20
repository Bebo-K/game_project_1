#include "player_input.h"
#include "../../input.h"


Entity* tracked_entity= null;

void PlayerInput::Update(Scene* scene, int ms){
    if(tracked_entity != null){
        if(Controller::AnyButtonDown()){

            tracked_entity->models[0].pose->StartAnimation("run");

        }



    }
}

void PlayerInput::Track(Entity* e){
    tracked_entity = e;
}
void PlayerInput::Untrack(Entity* e){
    if(tracked_entity==e){tracked_entity = null;}
}