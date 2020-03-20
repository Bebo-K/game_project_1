#ifndef INPUT_H
#define INPUT_H


#include "gfx/types.h"

namespace Input{
    enum MOUSE_IDS{
        LEFT_CLICK=0x101,RIGHT_CLICK=0x102,SCROLL_CLICK=0x103
    };
    enum JOY_IDS{
        A=0x1A0,B=0x1A1,X=0x1A2,Y=0x1A3,START=0x1A4,
        L1=0x1A5,L2=0x1A6,R1=0x1A7,R2=0x1A8,
        DPAD_UP=0x1A9,DPAD_DOWN=0x1AA,DPAD_LEFT=0x1AB,DPAD_RIGHT=0x1AC,
        LEFT_STICK=0x1AD,RIGHT_STICK=0x1AE
    };
    int GetKeyID(char* key_name);
    const char* GetKeyName(int key_id);

    void Init();
    void HandleKey(int key_id, bool down);
    void HandleCursor(int pos_x,int pos_y);
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