#ifndef INPUT_H
#define INPUT_H

#include "structs/data_types.h"

typedef long InputCode;

namespace InputEvent{//Don't rename these to game specific events until gameplay is finalized.
    const static InputCode None                 = 0x00000000;
    const static InputCode AnyButton            = 0x00000001;
    const static InputCode Axis_1               = 0x00000002;
    const static InputCode Axis_2               = 0x00000004;
    const static InputCode Button_A             = 0x00000008;
    const static InputCode Button_B             = 0x00000010;
    const static InputCode Button_C             = 0x00000020;
    const static InputCode Button_D             = 0x00000040;
    const static InputCode Button_Up            = 0x00000080;
    const static InputCode Button_Down          = 0x00000100;
    const static InputCode Button_Left          = 0x00000200;
    const static InputCode Button_Right         = 0x00000400;
    const static InputCode Button_L1            = 0x00000800;
    const static InputCode Button_L2            = 0x00001000;
    const static InputCode Button_R1            = 0x00002000;
    const static InputCode Button_R2            = 0x00004000;
    const static InputCode Button_Pause         = 0x00008000;
    const static InputCode Button_Menu          = 0x00010000;
    const static InputCode Button_Cursor        = 0x00020000;
    const static InputCode Button_ToggleConsole = 0x00040000;
    const static InputCode Cursor_Move          = 0x00080000;
    const static InputCode Cursor_Scroll        = 0x00100000;
    const static InputCode Text                 = 0x00200000;
};

namespace Input{
    struct Axis{
        InputCode event_id;
        short x,y;
        short dx,dy;
    };

    struct Button{
        InputCode event_id;
        byte state;
        bool IsDown();
        bool IsUp();
        bool IsJustPressed();
        bool IsJustReleased();
    };

    void Init();
    void ClearInputText();
    void PostUpdate();

    int GetKeyID(char* key_name);
    const char* GetKeyName(int key_id);

    InputCode NextInput();
    //bool   StateChanged(InputCode input_id);
    Button GetButton(InputCode button_id);
    Axis   GetAxis(InputCode axis_id);

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
    Button Button_ToggleConsole();
    text_char* TextInput();
    Axis Cursor();
    Axis Scroll();
    Button Cursor_Select();

    void HandleKey(int key_id, bool down);
    void HandleCursor(int pos_x,int pos_y);
    void HandleCharacter(int code_point);

};

#endif