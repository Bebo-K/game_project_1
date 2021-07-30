#include "winmain.h"
#include "io/file.h"
#include "input.h"
#include "log.h"

#include "config.h"

#ifdef _WINDOWS_
//Windows input hooks

//DIRECTINPUT (For generic USB controllers)
JOYCAPS     joystick_info[MAX_CONTROLLERS];
JOYINFOEX   joystick_state[MAX_CONTROLLERS]; 
DWORD       joystick_prev_button_state[MAX_CONTROLLERS];
DWORD       joystick_prev_x_axis[MAX_CONTROLLERS];//[MAX_CONTROLLER_AXES]
DWORD       joystick_prev_y_axis[MAX_CONTROLLERS];//[MAX_CONTROLLER_AXES]
UINT        current_device_count;

void SetupOSInput(){
    SetupJoypads();
}

void SetupJoypads(){
    for(int i=0;i<MAX_CONTROLLERS;i++){
        memset(&joystick_info[i],0,sizeof(JOYCAPS));
        memset(&joystick_state[i],0,sizeof(JOYINFOEX));
        joystick_state[i].dwSize = sizeof(JOYINFOEX);
        joystick_state[i].dwFlags = JOY_RETURNALL;
        memset(&joystick_prev_button_state[i],0,sizeof(DWORD));
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
            joystick_prev_x_axis[current_controller_num]=temp_device_state.dwXpos;
            joystick_prev_y_axis[current_controller_num]=temp_device_state.dwYpos;

            if(joyGetDevCaps(JOYSTICKID1+i,&joystick_info[i],sizeof(JOYCAPS))== JOYERR_NOERROR ){
                JOYCAPS stick_info;
                memcpy(&stick_info,&joystick_info[i],sizeof(JOYCAPS));
                /*
                printf("Controller #%d\n",i);
                printf("ControllerName: %s\n",stick_info.szPname);
                printf("ControllerName: %s\n",stick_info.szPname);
                printf("Conteroller info: %s\n",stick_info.szRegKey);
                printf("Manufacturerer ID: %d\n",stick_info.wMid);
                printf("Product ID: %d\n",stick_info.wPid);
                */
               
            }
            current_controller_num++;
        }

    }

    for(int i=0;i<current_controller_num;i++){
        if(!plugged_in[i])continue;
        int id = controller_ids[i];//controller device ID;
        //Where to put this?    
        Input::SetAxisBounds(0,joystick_info[id].wXmax,joystick_info[id].wXmin,joystick_info[id].wYmax,joystick_info[id].wYmin);
        Input::SetPhysicalAxisDirection(PhysicalInput::JOY1_AXIS_1+(i*26),false,true);
    }

}

void SetupDirectInput(HWND window_handle){
    for(int i=0;i<MAX_CONTROLLERS;i++){
        if(joySetCapture(window_handle, JOYSTICKID1+i, 0, false) != JOYERR_NOERROR ) { /*We'll deal with this if I make couch co-op*/} 
    }
}

//PhysicalInput::JOY1_BUTTON_0
void PollJoypads(){
    for(int i=0;i < MAX_CONTROLLERS;i++){
        DWORD err=joyGetPosEx(JOYSTICKID1+i,&joystick_state[i]);
        if(err == JOYERR_NOERROR){
            DWORD changed_buttons = joystick_state[i].dwButtons ^ joystick_prev_button_state[i];
            if(changed_buttons!= 0){
                int button_id=1;
                for(int j=0; j<32;j++){
                    if((changed_buttons & button_id) > 0){
                        Input::HandleBool(PhysicalInput::JOY1_BUTTON_0+j,(joystick_state[i].dwButtons & button_id) > 0);
                    } 
                    button_id = button_id << 1;
                }
            }

            if(joystick_state[i].dwXpos != joystick_prev_x_axis[i] || 
                joystick_state[i].dwYpos != joystick_prev_y_axis[i]){
                JOYINFOEX stick_state;
                memcpy(&stick_state,&joystick_state[i],sizeof(JOYINFOEX));
                Input::HandleIntAxis(PhysicalInput::JOY1_AXIS_1,joystick_state[i].dwXpos,joystick_state[i].dwYpos);
            }

            joystick_prev_button_state[i] = joystick_state[i].dwButtons;
            joystick_prev_x_axis[i] = joystick_state[i].dwXpos;
            joystick_prev_y_axis[i] = joystick_state[i].dwYpos;
        }
    }
}

void OnDirectInputJoyEvent(WPARAM wparam, bool down){}

void OnDirectInputButtonEvent(WPARAM wparam, bool down){
    DWORD changed_buttons = joystick_state[0].dwButtons ^ joystick_prev_button_state[0];
    if(changed_buttons!= 0){
        int button_id=1;
        for(int i=0; i<32;i++){
            if((changed_buttons & button_id) > 0){
                Input::HandleBool(PhysicalInput::JOY1_BUTTON_0+i,(joystick_state[0].dwButtons & button_id) > 0);
            }
        }
        button_id = button_id << 1;
    }
}

//XINPUT (For XBox controllers, I guess.)



#endif

