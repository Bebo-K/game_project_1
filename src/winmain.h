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

void SetupOSInput();
void SetupJoypads();
void PollJoypads();
void SetupDirectInput(HWND window_handle);
void OnDirectInputJoyEvent(WPARAM wparam, bool down);








#endif