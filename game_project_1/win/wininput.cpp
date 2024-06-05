#include <game_project_1/win/wininput.hpp>
#include <game_project_1/io/file.hpp>
#include <game_project_1/input.hpp>
#include <game_project_1/io/log.hpp>
#include <game_project_1/config.hpp>

#ifdef _WINDOWS_
//Windows input hooks

using namespace OSInput;
//DIRECTINPUT (For generic USB controllers)
JoystickStateInfo   joy_states[MAX_CONTROLLERS];
UINT                device_count;
bool                plugged_in[MAX_CONTROLLERS];
int                 controller_ids[MAX_CONTROLLERS];




void OSInput::Init(){
    SetupJoypads();
}

void OSInput::SetupJoypads(){
    for(int i=0;i<MAX_CONTROLLERS;i++){
        memset(&joy_states[i].info,0,sizeof(JOYCAPS));
        memset(&joy_states[i].state,0,sizeof(JOYINFOEX));
        joy_states[i].state.dwSize = sizeof(JOYINFOEX);
        joy_states[i].state.dwFlags = JOY_RETURNALL;
        memset(&joy_states[i].prev_button_state,0,sizeof(DWORD));
    }
    for(int i=0;i<MAX_CONTROLLERS;i++){plugged_in[i]=false;controller_ids[i]=-1;}
    device_count=0;
    PollForJoyDevices();
}

void OSInput::PollForJoyDevices(){
    UINT current_device_count = joyGetNumDevs();
    if(device_count == current_device_count)return;
    device_count = current_device_count;
    int ccn =0;//current controller number, but that would be a tad long
    for(int i=0; i<(int)current_device_count && ccn < MAX_CONTROLLERS;i++){
        JOYINFOEX temp_device_state;
        temp_device_state.dwSize = sizeof(JOYINFOEX);
        temp_device_state.dwFlags = JOY_RETURNALL;
        MMRESULT device_plugged_in = joyGetPosEx(JOYSTICKID1+i,&temp_device_state);
        if(device_plugged_in == JOYERR_NOERROR && ccn < MAX_CONTROLLERS){
            MMRESULT getCapsResult = joyGetDevCaps(JOYSTICKID1+i,&joy_states[ccn].info,sizeof(JOYCAPS));
            if(getCapsResult != JOYERR_NOERROR ){
                logger::exception("Failed to retrieve joystick capabilities for Joy ID %d: error code %d",JOYSTICKID1+i,getCapsResult);
            }


            plugged_in[ccn]=true;
            controller_ids[ccn]=i;
            joy_states[ccn].prev_x_axis=temp_device_state.dwXpos;
            joy_states[ccn].prev_y_axis=temp_device_state.dwYpos;

            joy_states[ccn].x_axis_range = (double)(joy_states[ccn].info.wXmax -  joy_states[ccn].info.wXmin);
            joy_states[ccn].y_axis_range = (double)(joy_states[ccn].info.wYmax -  joy_states[ccn].info.wYmin);
            joy_states[ccn].flip_x=false;
            joy_states[ccn].flip_y=true;
            ccn++;
        }
    }
}

void OSInput::SetupDirectInput(HWND window_handle){
    for(int i=0;i<MAX_CONTROLLERS;i++){
        if(joySetCapture(window_handle, JOYSTICKID1+i, 0, false) != JOYERR_NOERROR ) { /*We'll deal with this if I make couch co-op*/} 
    }
}

void OSInput::PollJoypads(){
    PollForJoyDevices();
    for(int i=0;i < MAX_CONTROLLERS;i++){
        DWORD err=joyGetPosEx(controller_ids[i],&joy_states[i].state);
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

                //Set joystick to range {-1,1};
                double axis_x = (joy_states[i].state.dwXpos - joy_states[i].info.wXmin)/joy_states[i].x_axis_range;
                double axis_y = (joy_states[i].state.dwYpos - joy_states[i].info.wYmin)/joy_states[i].y_axis_range;
                
                axis_x -= 0.5f;axis_x *= 2.0f;if(joy_states[i].flip_x){axis_x*=-1;}
                axis_y -= 0.5f;axis_y *= 2.0f;if(joy_states[i].flip_y){axis_y*=-1;}

                Input::OnAxis(PhysicalInput::JOY1_AXIS_1,axis_x,axis_y);
            }

            joy_states[i].prev_button_state = joy_states[i].state.dwButtons;
            joy_states[i].prev_x_axis = joy_states[i].state.dwXpos;
            joy_states[i].prev_y_axis = joy_states[i].state.dwYpos;
        }
    }
}

void OSInput::OnDirectInputJoyEvent(WPARAM wparam, bool down){}

void OSInput::OnDirectInputButtonEvent(WPARAM wparam, bool down){
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

