#ifndef WININPUT_H
#define WININPUT_H

#define MAX_CONTROLLERS 4
#define MAX_CONTROLLER_AXES 4

#include <winsock2.h>
#include <windows.h>
#include <dinput.h>
#include <winuser.h>
#include <mmsystem.h>

//#include <windows.h>
//#include <wingdi.h>
//#include <xinput.h>

//OS specific code

namespace OSInput{
    struct JoystickStateInfo{
        JOYCAPS     info;
        JOYINFOEX   state;
        DWORD       x_axis_max,x_axis_min;
        DWORD       y_axis_max,y_axis_min;
        double      x_axis_range;
        double      y_axis_range;
        bool        flip_x,flip_y;
        DWORD       prev_button_state;
        DWORD       prev_x_axis;//[MAX_CONTROLLER_AXES]
        DWORD       prev_y_axis;//[MAX_CONTROLLER_AXES]
    };


    void Init();
    void SetupOSInput();
    void SetupJoypads();
    void PollJoypads();
    void PollForJoyDevices();
    void SetupDirectInput(HWND window_handle);
    void OnDirectInputJoyEvent(WPARAM wparam, bool down);
    void OnDirectInputButtonEvent(WPARAM wparam, bool down);
}








#endif