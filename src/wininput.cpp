#include "winmain.h"
#include "io/file.h"
#include "input.h"
#include "log.h"

#include "config.h"

#ifdef _WINDOWS_
//Windows input hooks

//DIRECTINPUT (For generic USB controllers)
JoystickStateInfo joy_states[MAX_CONTROLLERS];
UINT        current_device_count;

void SetupOSInput(){
    SetupJoypads();
}

void SetupJoypads(){
    for(int i=0;i<MAX_CONTROLLERS;i++){
        memset(&joy_states[i].info,0,sizeof(JOYCAPS));
        memset(&joy_states[i].state,0,sizeof(JOYINFOEX));
        joy_states[i].state.dwSize = sizeof(JOYINFOEX);
        joy_states[i].state.dwFlags = JOY_RETURNALL;
        memset(&joy_states[i].prev_button_state,0,sizeof(DWORD));
    }
    current_device_count = joyGetNumDevs();
    bool plugged_in[MAX_CONTROLLERS];
    int  controller_ids[MAX_CONTROLLERS];
    int current_controller_num =0;
    for(int i=0;i<MAX_CONTROLLERS;i++){plugged_in[i]=false;controller_ids[i]=0;}
    for(int i=0;i<(int)current_device_count;i++){
        JOYINFOEX temp_device_state;
        temp_device_state.dwSize = sizeof(JOYINFOEX);
        temp_device_state.dwFlags = JOY_RETURNALL;
        MMRESULT device_plugged_in = joyGetPosEx(JOYSTICKID1+i,&temp_device_state);
        if(device_plugged_in == JOYERR_NOERROR && current_controller_num < MAX_CONTROLLERS){
            plugged_in[current_controller_num]=true;
            controller_ids[current_controller_num]=i;
            joy_states[current_controller_num].prev_x_axis=temp_device_state.dwXpos;
            joy_states[current_controller_num].prev_y_axis=temp_device_state.dwYpos;

            MMRESULT getCapsResult = joyGetDevCaps(JOYSTICKID1+i,&joy_states[i].info,sizeof(JOYCAPS));
            if(getCapsResult != JOYERR_NOERROR ){
                logger::exception("Failed to retrieve joystick capabilities for Joy ID %d: error code %d",JOYSTICKID1+i,getCapsResult);
            }
            current_controller_num++;
        }

    }

    for(int i=0;i<current_controller_num;i++){
        if(!plugged_in[i])continue;
        int id = controller_ids[i];//controller device ID;
        //Set joystick to range {-1,1};
        UINT joy_x_range = joy_states[id].info.wXmax -  joy_states[id].info.wXmin;
        UINT joy_y_range = joy_states[id].info.wYmax -  joy_states[id].info.wYmin;
        joy_states[i].x_axis_multiplier = (1.0f/joy_x_range);
        joy_states[i].y_axis_multiplier = (1.0f/joy_y_range);
        FlipJoyAxis(i,false,true);
    }
}


void FlipJoyAxis(int joy_id, bool flip_horizontal,bool flip_vertical){
    joy_states[joy_id].x_axis_multiplier *= (flip_horizontal)?-1:1;
    joy_states[joy_id].y_axis_multiplier *= (flip_vertical)?-1:1;
}

void SetupDirectInput(HWND window_handle){
    for(int i=0;i<MAX_CONTROLLERS;i++){
        if(joySetCapture(window_handle, JOYSTICKID1+i, 0, false) != JOYERR_NOERROR ) { /*We'll deal with this if I make couch co-op*/} 
    }
}

void PollJoypads(){
    for(int i=0;i < MAX_CONTROLLERS;i++){
        DWORD err=joyGetPosEx(JOYSTICKID1+i,&joy_states[i].state);
        if(err == JOYERR_NOERROR){
            DWORD changed_buttons = joy_states[i].state.dwButtons ^ joy_states[i].prev_button_state;
            if(changed_buttons!= 0){
                int button_id=1;
                for(int j=0; j<32;j++){
                    if((changed_buttons & button_id) > 0){
                        Input::OnKey(PhysicalInput::JOY1_BUTTON_0+j,(joy_states[i].state.dwButtons & button_id) > 0);
                    } 
                    button_id = button_id << 1;
                }
            }

            if(joy_states[i].state.dwXpos != joy_states[i].prev_x_axis || 
                joy_states[i].state.dwYpos != joy_states[i].prev_y_axis){
                    
                float axis_x = (joy_states[i].state.dwXpos - joy_states[i].info.wXmin)*joy_states[i].x_axis_multiplier;
                float axis_y = (joy_states[i].state.dwYpos - joy_states[i].info.wYmin)*joy_states[i].y_axis_multiplier;
                Input::OnAxis(PhysicalInput::JOY1_AXIS_1,axis_x,axis_y);
            }

            joy_states[i].prev_button_state = joy_states[i].state.dwButtons;
            joy_states[i].prev_x_axis = joy_states[i].state.dwXpos;
            joy_states[i].prev_y_axis = joy_states[i].state.dwYpos;
        }
    }
}

void OnDirectInputJoyEvent(WPARAM wparam, bool down){}

void OnDirectInputButtonEvent(WPARAM wparam, bool down){
    DWORD changed_buttons = joy_states[0].state.dwButtons ^ joy_states[0].prev_button_state;
    if(changed_buttons!= 0){
        int button_id=1;
        for(int i=0; i<32;i++){
            if((changed_buttons & button_id) > 0){
                Input::OnKey(PhysicalInput::JOY1_BUTTON_0+i,(joy_states[0].state.dwButtons & button_id) > 0);
            }
        }
        button_id = button_id << 1;
    }
}

//XINPUT (For XBox controllers, I guess.)



#endif

