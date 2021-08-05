#ifndef INPUT_H
#define INPUT_H

#include "struct/data_types.h"
#include "struct/3d_types.h"

//July 29th, 2021 rendition of Input Handling

//Possible physical inputs to handle (referred to as 'keys')
namespace PhysicalInput{
    const int KEY_COUNT             = 0x100;
    const int MOUSE_CURSOR_AXIS     = 0x101;
    const int MOUSE_SCROLL_AXIS     = 0x102;
    const int MOUSE_LEFT_BUTTON     = 0x103;
    const int MOUSE_RIGHT_BUTTON    = 0x104;
    const int MOUSE_CENTER_BUTTON   = 0x105;
    const int MOUSE_BUTTON_1        = 0x106;
    const int MOUSE_BUTTON_2        = 0x107;
    const int JOY1_AXIS_1           = 0x110;
    const int JOY1_AXIS_2           = 0x111;
    const int JOY1_AXIS_3           = 0x112;
    const int JOY1_AXIS_4           = 0x113;
    const int JOY1_BUTTON_0         = 0x114;
    //JOY1_BUTTON_1 - 31            = 0x115 - 0x134;

    const int JOY2_AXIS_1           = 0x136;
    const int JOY2_AXIS_2           = 0x137;
    const int JOY2_AXIS_3           = 0x138;
    const int JOY2_AXIS_4           = 0x139;
    const int JOY2_BUTTON_0         = 0x140;
    //JOY2_BUTTON_1 - 31            = 0x141 - 0x15F;
};

namespace Input{
    const float AXIS_CUTOFF = 0.01;

    struct Axis{
        float x,y;
        float dx,dy;
    };

    struct Button{
        byte state;
        inline bool IsDown(){return (state&1) > 0;}
        inline bool IsUp(){return (state&1) == 0;}
        inline bool IsJustPressed(){return state == 3;}
        inline bool IsJustReleased(){return state == 2;}
    };

    //Virtual controller 
    class Controller{
        const static char* button_names[];
        const static char* axis_names[];

        int  changed_inputs;
        Axis MoveAxis;
        Axis CameraAxis;

        Button A,B,C,D;
        Button D_Up,D_Down,D_Left,D_Right;
        Button L1,L2,R1,R2;

        Button Pause;
        Button Menu;

        Controller();
        Button* GetButtonByName(char* button_name);
        Axis* GetAxisByName(int axis_id);
    };

    struct Key_Button_Bind{
        int physical_key_id;
        Button* button;
    };

    struct Key_Axis_Bind{
        int physical_key_id;
        Axis* axis;
        vec2 direction;
    };

    struct Axis_Bind{
        int physical_axis_id;
        Axis* axis;
    };

    void Init();
    void Destroy();

    void OnKey(int key_id,bool down);
    void OnAxis(int axis_id,float x,float y);
    Controller GetController(/*int controller_id*/);
};


#endif