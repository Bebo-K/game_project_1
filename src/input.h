#ifndef INPUT_H
#define INPUT_H

#include "structs/data_types.h"

enum InputEvent{//Don't rename these to game specific events until gameplay is finalized.
    AnyButton,
    Axis_1,
    Axis_2,
    Button_A,
    Button_B,
    Button_C,
    Button_D,
    Button_Up,  
    Button_Down,
    Button_Left,
    Button_Right,
    Button_L1,
    Button_L2,
    Button_R1,
    Button_R2,
    Button_Pause,
    Button_Menu,
    Text,
    Cursor_Move,
    Cursor_Select,
    Scroll
};

namespace Input{

    struct Axis{
        short x,y;
        short dx,dy;
    };

    struct Button{
        byte state;
        
        bool IsDown();
        bool IsUp();
        bool IsJustPressed();
        bool IsJustReleased();
    };

    void Init();
    void PostUpdate();

    int GetKeyID(char* key_name);
    const char* GetKeyName(int key_id);

    bool StateChanged(InputEvent event_type);

    Button AnyButton();
    Axis Axis_1();
    Axis Axis_2();
    Button Button_A();
    Button Button_B();
    Button Button_C();
    Button Button_D();
    Button Button_Up(); 
    Button Button_Down();
    Button Button_Left();
    Button Button_Right();
    Button Button_L1();
    Button Button_L2();
    Button Button_R1();
    Button Button_R2();
    Button Button_Pause();
    Button Button_Menu();
    text_char* TextInput();
    Axis Cursor();
    Axis Scroll();
    Button Cursor_Select();

    void HandleKey(int key_id, bool down);
    void HandleCursor(int pos_x,int pos_y);
    void HandleCharacter(int code_point);

};

#endif