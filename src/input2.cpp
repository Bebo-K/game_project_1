#include "input2.h"
#include "struct/list.h"


List<Input::Key_Button_Bind> button_binds;
List<Input::Key_Axis_Bind> button_axis_binds;
List<Input::Axis_Bind> axis_binds;

Input::Controller controller;
Input::Controller Input::GetController(){return controller;}

text_char* input_text=null;            
text_char  character_code[2]  = {0,0};

const char* Input::Controller::button_names[] = {
    "A","B","C","D",
    "D_Up","D_Down","D_Left","D_Right",
    "L1","L2","R1","R2",
    "Pause","Menu"};

const char* Input::Controller::axis_names[] = {"Move","Camera"};
const char* axis_directions[8] = {"up","down","left","right"};


void Input::Init(){

}

void Input::Destroy(){
    button_binds.Clear();
    button_axis_binds.Clear();
    axis_binds.Clear();
}

void Input::OnKey(int key_id,bool down){

    for(Key_Button_Bind* keybind:button_binds){
        if(key_id == keybind->physical_key_id){

        }
    }

    for(Key_Axis_Bind* keybind:button_axis_binds){
        if(key_id == keybind->physical_key_id){
            
        }
    }

}

void Input::OnAxis(int axis_id,float x,float y){

    for(Axis_Bind* keybind:axis_binds){
        if(axis_id == keybind->physical_axis_id){
            
        }
    }

}

Input::Controller::Controller(){
    changed_inputs=0;
    MoveAxis.x=0;   MoveAxis.y=0;   MoveAxis.dx=0;  MoveAxis.dy=0;
    CameraAxis.x=0; CameraAxis.y=0; CameraAxis.dx=0;CameraAxis.dy=0;
    A.state=0;      B.state=0;      C.state=0;      D.state=0;
    D_Up.state=0;   D_Down.state=0; D_Left.state=0; D_Right.state=0;
    L1.state=0;     L2.state=0;     R1.state=0;     R2.state=0;
    Pause.state=0;  Menu.state=0;
}

Input::Button* Input::Controller::GetButtonByName(char* button_name){

}

Input::Axis* Input::Controller::GetAxisByName(int axis_id){

}