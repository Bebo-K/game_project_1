#ifndef INPUT_H
#define INPUT_H


#include "gfx/types.h"

namespace Input{
    enum MOUSE_IDS{
        LEFT_CLICK=0x101,RIGHT_CLICK=0x102,SCROLL_CLICK=0x103
    };
    enum JOY_IDS{
        A=0x1A0,B=0x1A1,X=0x1A2,Y=0x1A3,
        L1,L2,R1,R2,
        DPAD_UP,DPAD_DOWN,DPAD_LEFT,DPAD_RIGHT,
        LEFT_STICK,RIGHT_STICK
    };
    int GetKeyID(char* key_name);
    const char* GetKeyName(int key_id);

    void Init();
    void HandleKey(int key_id, bool down);
    void HandleCursor(int pos_x,int pos_y);
    void HandleJoystick(int stick_id,float tilt_x,float tilt_y);
    void HandleCharacter(int code_point);

    void Update();
};

namespace Controller{

    struct Button{
        char state;
        
        bool IsDown();
        bool IsUp();
        bool IsJustPressed();
        bool IsJustReleased();
    };

    bool   AnyButtonDown();
    vec2   MoveAxis();
    vec2   CameraAxis();
    Button Confirm();
    Button Cancel();
    Button Jump();
    Button Crouch();
    Button CenterCamera();
    Button Pause();
    Button Inventory();
    //vec2 Cursor();
    //char* TextInput();
}

#endif