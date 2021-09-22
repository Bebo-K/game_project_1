#ifndef WININPUT_H
#define WININPUT_H

#define MAX_CONTROLLERS 4
#define MAX_CONTROLLER_AXES 4
#include <windows.h>
#include <dinput.h>
#include <winuser.h>
#include <mmsystem.h>

//#include <windows.h>
//#include <wingdi.h>
//#include <xinput.h>

//OS specific code

struct JoystickStateInfo{
    JOYCAPS     info;
    JOYINFOEX   state;
    DWORD       x_axis_max,x_axis_min;
    DWORD       y_axis_max,y_axis_min;
    float       x_axis_multiplier;
    float       y_axis_multiplier;
    DWORD       prev_button_state;
    DWORD       prev_x_axis;//[MAX_CONTROLLER_AXES]
    DWORD       prev_y_axis;//[MAX_CONTROLLER_AXES]
};

void SetupOSInput();
void SetupJoypads();
void FlipJoyAxis(int joy_id, bool flip_horizontal,bool flip_vertical);
void PollJoypads();
void SetupDirectInput(HWND window_handle);
void OnDirectInputJoyEvent(WPARAM wparam, bool down);






#endif