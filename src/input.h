#ifndef INPUT_H
#define INPUT_H

#include "struct/data_types.h"
#include "struct/3d_types.h"

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

//Possible input events to generate (referred to as 'events'), negative event IDs should not be bindable.


namespace Input{
    const float AXIS_CUTOFF = 0.01;

    enum EventID{None=0,
    MoveAxis=1,CamAxis=2,CursorAxis=3,ScrollAxis=4,
    A=5,B=6,C=7,D=8,
    DPAD_Up=9,DPAD_Down=10,DPAD_Left=11,DPAD_Right=12,
    L1=13,L2=14,R1=15,R2=16,
    Pause=17, Menu=18, CursorSelect=19, ToggleConsole=20,
    Any_Button=21,Text=22};

    struct Axis{
        EventID event_id;
        int raw_x,raw_y;
        int dx,dy;
        int max_x,max_y;
        int min_x,min_y;
        vec2 GetNormalized();
        void SetBounds(int maxx,int minx,int maxy, int miny);
    };

    struct Button{
        EventID event_id;
        byte state;
        bool IsDown();
        bool IsUp();
        bool IsJustPressed();
        bool IsJustReleased();
    };

    struct KeyBind{
        int physical_input;
        EventID event;
        //only used for button-to-axis bindings.
        int axis_direction;
        bool flip_x,flip_y;
        //int axis_magnitude;
    };

    void Init();
    void Destroy();
    void LoadKeyLayout(char* layout_filename);
    void LoadKeyBindings();
    void RegisterPhysicalInput(char* name,int id);
    void SetAxisBounds(int axis_id,int xmax,int xmin,int ymax,int ymin);
    void SetPhysicalAxisDirection(int input_id,bool flip_x,bool flip_y);
    void ClearInputText();
    void PostUpdate();

    EventID GetEventID(char* event_name);
    const char* GetEventName(EventID event);

    int GetKeyID(char* key_name);
    const char* GetKeyName(int key_id);

    KeyBind GetKeyBindByInput(int input_id);

    EventID NextEvent();
    //bool   StateChanged(InputCode input_id);
    //Button GetButton(InputCode button_id);
    //Axis   GetAxis(InputCode axis_id);

    
    Axis Move_Axis();
    Axis Cam_Axis();
    Axis Cursor_Axis();
    Axis Scroll_Axis();

    Button AnyButton();
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
    Button Button_Cursor_Select();

    text_char* TextInput();

    void HandleBool(int key_id, bool down);
    void HandleIntAxis(int axis_id,int pos_x,int pos_y);
    //void HandleFloatAxis(float pos_x,int pos_y);
    void HandleCharacter(int code_point);
    void HandleText(text_char* text);
};

#endif