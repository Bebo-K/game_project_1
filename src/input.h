#ifndef INPUT_H
#define INPUT_H

#include "struct/data_types.h"
#include "struct/str.h"
#include "struct/3d_types.h"
#include "struct/2d_types.h"
#include "config.h"

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

typedef int AxisID;
typedef int ButtonID;
namespace Controller{

    struct Axis{                                     
        float x,y;
        float dx,dy;
        int direction_down;
        void AddTilt(vec2 tilt);
        void SetTilt(vec2 tilt);
        void SetDirection(int direction_id,bool down);
        vec2 GetNormalized();
    };

    struct Button{
        byte state;
        inline bool IsDown(){return (state&1) > 0;}
        inline bool IsUp(){return (state&1) == 0;}
        inline bool IsJustPressed(){
            return state == 3;}
        inline bool IsJustReleased(){
            return state == 2;}
    };

    const AxisID Move=0;
    const AxisID Camera=1;
    const ButtonID A=0;
    const ButtonID B=1;
    const ButtonID C=2;
    const ButtonID D=3;
    const ButtonID D_Up=4;
    const ButtonID D_Down=5;
    const ButtonID D_Left=6;
    const ButtonID D_Right=7;
    const ButtonID L1=8;
    const ButtonID L2=9;
    const ButtonID R1=10;
    const ButtonID R2=11;
    const ButtonID Pause=12;
    const ButtonID Menu=13;

    //Input state accessors
    Button      GetButton(ButtonID id);
    Axis        GetAxis(AxisID axis);
    point_i     GetPCCursor();
    Button      GetPCLeftMouse();
    Button      GetPCRightMouse();
    Button      GetPCCenterMouse();
    Button      GetAnyButton();
    Button      GetToggleConsole();
    vec2        GetPCScroll();
    wchar*      GetTextInput();
};

namespace Input{
    enum Event{
        None=0,
        Move=1,
        Cam=2,
        A=3,
        B=4,
        C=5,
        D=6,
        D_Up=7,
        D_Down=8,
        D_Left=9,
        D_Right=10,
        L1=11,
        L2=12,
        R1=13,
        R2=14,
        Pause=15, 
        Menu=16,
        PC_Cursor=17,
        PC_Scroll=18,
        PC_RClick=18,
        PC_LClick=19,
        PC_Text=21,
        ToggleConsole=22,
        AnyButton=23
    };

    struct Key_Button_Bind{
        int physical_key_id;
        ButtonID button;
    };

    struct Key_Axis_Bind{
        int physical_key_id;
        AxisID axis;
        int direction_id;
    };

    struct Axis_Bind{
        int physical_axis_id;
        AxisID axis;
    };

    //Engine events
    void Init();
    void Update();
    void Destroy();

    //Input event handlers
    void OnKey(int key_id,bool down);
    void OnAxis(int axis_id,float x,float y);

    void OnPCCursor(int x,int y);
    void OnPCClick(bool down, bool left);
    void OnPCScroll(int dx, int dy);

    void OnText(wchar* text);
    void OnCharacter(int code_point);

    //Event flag management
    Event NextEvent();
    Event NextEvent(Event start);
    void    ClearEvent(Event event);
    void    ClearAllEvents();

    //Key binding management
    void LoadKeyLayout(char* layout_filename);
    void LoadDefaultKeyBindings();
    void LoadKeyBindings(char** lines,int line_count);
    void AddKeyButtonBind(ButtonID button, int key_id);
    void RemoveKeyButtonBind(ButtonID button, int key_id);
    void AddKeyAxisBind(AxisID axis, int key_id, int direction_id);
    void RemoveKeyAxisBind(AxisID axis, int key_id);
    void AddAxisBind(AxisID axis, int physical_axis_id);
    void ClearButtonBinds(int key_id);//clears all kinds of bindings from a key
    void ClearAxisBind(int physical_axis_id);

    void UnbindButton(ButtonID button);
    void UnbindAxis(AxisID axis);

    //Utility
    int GetKeyID(char* key_name);
    const char* GetKeyName(int key_id);

    //EventID GetEventID(char* event_name);
    //const char* GetEventName(EventID event);
};


#endif