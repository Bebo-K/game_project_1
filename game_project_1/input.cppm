export module Input;

import <math>;
import Memory;
import Primitives;
import Pool;
import Map;
import File;
import OS;
import String;
import Config;


//July 29th, 2021 rendition of Input Handling

//Possible physical inputs to handle (referred to as 'keys')
namespace PhysicalInput{
    export constexpr int KEY_COUNT              = 0x100;
    export constexpr  int MOUSE_CURSOR_AXIS     = 0x101;
    export constexpr  int MOUSE_SCROLL_AXIS     = 0x102;
    export constexpr  int MOUSE_LEFT_BUTTON     = 0x103;
    export constexpr  int MOUSE_RIGHT_BUTTON    = 0x104;
    export constexpr  int MOUSE_CENTER_BUTTON   = 0x105;
    export constexpr  int MOUSE_BUTTON_1        = 0x106;
    export constexpr  int MOUSE_BUTTON_2        = 0x107;
    export constexpr  int JOY1_AXIS_1           = 0x110;
    export constexpr  int JOY1_AXIS_2           = 0x111;
    export constexpr  int JOY1_AXIS_3           = 0x112;
    export constexpr  int JOY1_AXIS_4           = 0x113;
    export constexpr  int JOY1_BUTTON_0         = 0x114;
    //JOY1_BUTTON_1 - 31            = 0x115 - 0x134;

    export constexpr  int JOY2_AXIS_1           = 0x136;
    export constexpr  int JOY2_AXIS_2           = 0x137;
    export constexpr  int JOY2_AXIS_3           = 0x138;
    export constexpr  int JOY2_AXIS_4           = 0x139;
    export constexpr  int JOY2_BUTTON_0         = 0x140;
    //JOY2_BUTTON_1 - 31            = 0x141 - 0x15F;
};

export enum class AxisID {
    Move=0,
    Camera=1,
};
byte operator+(int lhs, AxisID rhs){return lhs+static_cast<byte>(rhs);}

export enum class ButtonID {
    A=0,B=1,C=2,D=3,
    D_Up=4,D_Down=5,D_Left=6,D_Right=7,
    L1=8,L2=9,R1=10,R2=11,
    Pause=12,Menu=13
};
byte operator+(int lhs, ButtonID rhs){return lhs+static_cast<byte>(rhs);}

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

namespace Controller{

    export struct Axis{
        constexpr static int UP=0, DOWN=1, LEFT=2, RIGHT=3;
        //TODO: Rework into per-axis config, hi(100%)+low(0%,buttonpress) mapping
        constexpr float AXIS_CUTOFF = 0.01;
        constexpr static float tilt_cutoff = 0.9f;
        float x,y;
        float dx,dy;
        short key_flags;//4 bits are used, one per direction.
        //tilt_flags
        //Hi 8 bits aren't used. Low 8 bits are used in pairs, one per direction starting with lowest = UP lo bit
        // Lo bit is set if direction is 90% tilted, Hi bit is set if lo bit just changed
        short tilt_flags;
        //////////////////////
        //    Axis
        ////////////////////
        void AddTilt(vec2 tilt){
            float new_x = x+tilt.x;
            float new_y = y+tilt.y;
            if(x*x + y*y < AXIS_CUTOFF){x=0;y=0;}//deadzone
            if(x > 1.0f){x=1.0f;}
            if(x < -1.0f){x=-1.0f;}
            if(y > 1.0f){y=1.0f;}
            if(y < -1.0f){y=-1.0f;}
            dx = new_x-x;
            dy = new_y-y;
            x = tilt.x;
            y = tilt.y;
            UpdateTilt();
        }
        void SetTilt(vec2 tilt){
            dx = tilt.x-x;
            dy = tilt.y-y;
            x = tilt.x;
            y = tilt.y;
            if(x*x + y*y < AXIS_CUTOFF){x=0;y=0;}//deadzone
            UpdateTilt();
        }
        void SetDirection(short direction_id,bool down){
            if (down) {
                Bit::Set(&key_flags,direction_id);
            }
            else {
                Bit::Clear(&key_flags,direction_id);
            }
            x=0;y=0;
            if(Bit::Get(key_flags,UP)){y += 1.0f;}
            if(Bit::Get(key_flags,DOWN)){y -= 1.0f;}
            if(Bit::Get(key_flags,LEFT)){x -= 1.0f;}
            if(Bit::Get(key_flags,RIGHT)){x += 1.0f;}

            vec2 old_pos = {x,y};
            vec2 new_pos = GetNormalized();
            x=new_pos.x;
            y=new_pos.y;
            dx=x-old_pos.x;
            dy=x-old_pos.y;
            UpdateTilt();
        }

        void UpdateTilt(){
            if(y > tilt_cutoff && !Bit::Get(tilt_flags,UP*2)){ Bit::Set(&tilt_flags,UP*2); Bit::Set(&tilt_flags,(UP*2)+1);}
            else if(y < tilt_cutoff && Bit::Get(tilt_flags,UP*2)){ Bit::Clear(&tilt_flags,UP*2); Bit::Set(&tilt_flags,(UP*2)+1);}
            
            if(y < -tilt_cutoff && !Bit::Get(tilt_flags,DOWN*2)){ Bit::Set(&tilt_flags,DOWN*2); Bit::Set(&tilt_flags,(DOWN*2)+1);}
            else if(y > -tilt_cutoff && Bit::Get(tilt_flags,DOWN*2)){ Bit::Clear(&tilt_flags,DOWN*2); Bit::Set(&tilt_flags,(DOWN*2)+1);}
            
            if(x < -tilt_cutoff && !Bit::Get(tilt_flags,LEFT*2)){ Bit::Set(&tilt_flags,LEFT*2); Bit::Set(&tilt_flags,(LEFT*2)+1);}
            else if(x > -tilt_cutoff && Bit::Get(tilt_flags,LEFT*2)){ Bit::Clear(&tilt_flags,LEFT*2); Bit::Set(&tilt_flags,(LEFT*2)+1);}

            if(x > tilt_cutoff && !Bit::Get(tilt_flags,RIGHT*2)){ Bit::Set(&tilt_flags,RIGHT*2); Bit::Set(&tilt_flags,(RIGHT*2)+1);}
            else if(x < tilt_cutoff && Bit::Get(tilt_flags,RIGHT*2)){ Bit::Clear(&tilt_flags,RIGHT*2); Bit::Set(&tilt_flags,(RIGHT*2)+1);}
        }

        void ClearTiltDelta(){
            dx=0;
            dy=0;
            Bit::Clear(&tilt_flags,(UP*2)+1);
            Bit::Clear(&tilt_flags,(DOWN*2)+1);
            Bit::Clear(&tilt_flags,(LEFT*2)+1);
            Bit::Clear(&tilt_flags,(RIGHT*2)+1);
        }

        vec2 GetNormalized(){
            //Clip to unit length
            vec2 ret = {x,y};
            float len_sqr = x*x + y*y;
            if(len_sqr > 1){
                len_sqr = sqrtf(len_sqr);
                ret.x /= len_sqr;
                ret.y /= len_sqr;
            }
            return ret;
        }

        bool IsUp(){
            return Bit::Get(tilt_flags,(UP*2));
        }
        bool IsDown(){
            return Bit::Get(tilt_flags,(DOWN*2));
        }
        bool IsLeft(){
            return Bit::Get(tilt_flags,(LEFT*2));
        }
        bool IsRight(){
            return Bit::Get(tilt_flags,(RIGHT*2));
        }

        bool IsJustUp(){
            return Bit::Get(tilt_flags,(UP*2)) && Bit::Get(tilt_flags,(UP*2)+1);
        }
        bool IsJustDown(){
            return Bit::Get(tilt_flags,(DOWN*2)) && Bit::Get(tilt_flags,(DOWN*2)+1);
        }
        bool IsJustLeft(){
            return Bit::Get(tilt_flags,(LEFT*2)) && Bit::Get(tilt_flags,(LEFT*2)+1);
        }
        bool IsJustRight(){
            return Bit::Get(tilt_flags,(RIGHT*2)) && Bit::Get(tilt_flags,(RIGHT*2)+1);
        }
    };

    export struct Button{
        byte state;
        inline bool IsDown(){return (state&1) > 0;}
        inline bool IsUp(){return (state&1) == 0;}
        inline bool IsJustPressed(){return state == 3;}
        inline bool IsJustReleased(){return state == 2;}
    };

    //////////////////////
    //    Input state accessors
    ////////////////////
    export Button  GetButton(ButtonID id);
    export Axis    GetAxis(AxisID axis);
    export point_i GetPCCursor();
    export Button  GetPCLeftMouse();
    export Button  GetPCRightMouse();
    export Button  GetPCCenterMouse();
    export vec2    GetPCScroll();
    export Button  GetAnyButton();
    export Button  GetToggleConsole();
    export wchar*  GetTextInput();

}

namespace Input{
    export enum class Event{
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
        PC_Cursor=18,
        PC_Scroll=19,
        PC_RClick=20,
        PC_LClick=21,
        PC_Text=22,
        PC_Escape=23,
        PC_Return=24,
        ToggleConsole=25,
        AnyButton=26,
        COUNT=27
    };

    //Engine events
    export void Init();
    export void Update();
    export void Destroy();

    //    Input event handlers
    export void OnKeyboardKey(int key_id,bool down);
    export void OnKey(int key_id,bool down);
    export void OnAxis(int axis_id,float x,float y);
    export void OnPCCursor(int x,int y);
    export void OnPCClick(bool down, bool left);
    export void OnPCScroll(int dx, int dy);
    export void OnText(wchar* text);
    export void OnCharacter(int code_point);

    //    Event flag management
    export Event NextEvent();
    export Event NextEvent(Event start);
    export void ClearEvent(Event event);
    export void SetEvent(Event event);
    export void ClearAllEvents();

    //    Key binding management
    export void AddKeyButtonBind(ButtonID button, int key_id);
    export void RemoveKeyButtonBind(ButtonID button, int key_id);
    export void AddKeyAxisBind(AxisID axis, int key_id, int direction_id);
    export void RemoveKeyAxisBind(AxisID axis, int key_id);
    export void AddAxisBind(AxisID axis, int physical_axis_id);
    export void ClearButtonBinds(int key_id);
    export void ClearAxisBind(int physical_axis_id);
    export void UnbindButton(ButtonID button);
    export void UnbindAxis(AxisID axis);
    //Typing mode
    export void SetKeyboardTypingOnly(bool enabled);

    //    Utility
    export int GetKeyID(const char* key_name);
    export const char* GetKeyName(int key_id);

    //    Loading
    export void LoadKeyLayout(const char* layout_filename);
    export void LoadKeyBindings(const char** lines,int line_count);
    export void LoadDefaultKeyBindings();

    ///*** Internal State ***///
    point_i mouse_cursor;
    vec2    mouse_scroll;
    Controller::Button mouse_l,mouse_r,mouse_c;
    Controller::Button toggle_console;
    Controller::Button any_button;

    wchar* input_text=nullptr;
    wchar  character_code[2]  = {0,0};
    Dictionary<int,char*> physical_input_map= Dictionary<int,char*>(8);
    Pool<Key_Button_Bind> button_binds;
    Pool<Key_Axis_Bind> key_axis_binds;
    Pool<Axis_Bind> axis_binds;

    constexpr int AXIS_COUNT=2;
    Controller::Axis controller_axes[AXIS_COUNT];
    constexpr int BUTTON_COUNT=14;
    Controller::Button controller_buttons[BUTTON_COUNT];

    //bitflag representing whether the state of EventID (i-1) has changed
    unsigned long int input_state = 0;

    bool keyboard_type_only=false;
    //these keys are still enabled as inputs in typing mode
    int enter_physical_key;
    int escape_physical_key;
    int toggle_console_key_id=0;

    constexpr int AXIS_EVENT_BASE=0;
    constexpr int BUTTON_EVENT_BASE=2;

    const char* button_names[BUTTON_COUNT] =
    {   "a","b","c","d",
        "d_up","d_down","d_left","d_right",
        "l1","l2","r1","r2",
        "pause","menu"};

    const char* axis_names[AXIS_COUNT] = {"move","camera"};
    const char* axis_directions[4] = {"up","down","left","right"};
    ///*** End Internal State ***///



    //////////////////////
    //    Engine events
    ////////////////////
    void Init(){
        for(int i=0;i<BUTTON_COUNT;i++){controller_buttons[i].state=0;}
        mouse_l.state = 0;
        mouse_r.state = 0;
        mouse_c.state = 0;
        toggle_console.state = 0;
        any_button.state =0;
        for(int i=0;i<AXIS_COUNT;i++){
            controller_axes[i].x=0;
            controller_axes[i].y=0;
            controller_axes[i].dx=0;
            controller_axes[i].dy=0;
            controller_axes[i].key_flags=0;
            controller_axes[i].tilt_flags=0;
        }

        if(IFile::Exists(config::key_layout_filename)){LoadKeyLayout(config::key_layout_filename);}

        LoadDefaultKeyBindings();
    }

    void Update(){
        //clear "changed" flag for buttons
        for(int i=0;i<BUTTON_COUNT;i++){
            Bit::Clear(&controller_buttons[i].state,1);
        }

        Bit::Clear(&mouse_l.state,1);
        Bit::Clear(&mouse_l.state,1);
        Bit::Clear(&mouse_r.state,1);
        Bit::Clear(&mouse_c.state,1);
        Bit::Clear(&toggle_console.state,1);
        Bit::Clear(&any_button.state,1);
        //clear deltas for axes
        for(int i=0;i<AXIS_COUNT;i++){
            controller_axes[i].ClearTiltDelta();
        }
        //clear input text
        character_code[0]=0;
        if(input_text != nullptr){
            if(input_text != character_code){Free(input_text);}
            input_text = nullptr;
        }
        ClearAllEvents();
    }

    void Destroy(){
        for(Tuple<int,char*> t: physical_input_map){if(t.value)Free(t.value);}
        physical_input_map.Clear();
        button_binds.Clear();
        key_axis_binds.Clear();
        axis_binds.Clear();
    }


    //////////////////////
    //    Input event handlers
    ////////////////////

    void OnKeyboardKey(int key_id,bool down){
        if(keyboard_type_only && key_id != enter_physical_key && key_id != escape_physical_key){return;}

        if(key_id == enter_physical_key && down){SetEvent(Event::PC_Return);}
        if(key_id == escape_physical_key && down){SetEvent(Event::PC_Escape);}

        OnKey(key_id,down);
    }
    void OnKey(int key_id,bool down){
        for(Key_Button_Bind* keybind:button_binds){
            if(key_id == keybind->physical_key_id){
                controller_buttons[0+keybind->button].state = (down)?3:2;
                Bit::Set(&input_state,BUTTON_EVENT_BASE+keybind->button);
            }
        }
        for(Key_Axis_Bind* keybind:key_axis_binds){
            if(key_id == keybind->physical_key_id){
                controller_axes[0+keybind->axis].SetDirection(keybind->direction_id,down);
                Bit::Set(&input_state,AXIS_EVENT_BASE+keybind->axis);
            }
        }
        if(key_id == toggle_console_key_id){
            toggle_console.state=(down)?3:2;
            SetEvent(Event::ToggleConsole);
        }
    }
    void OnAxis(int axis_id,float x,float y){
        for(Axis_Bind* keybind:axis_binds){
            if(axis_id == keybind->physical_axis_id){
                controller_axes[0+keybind->axis].SetTilt({x,y});
                Bit::Set(&input_state,AXIS_EVENT_BASE+keybind->axis);
            }
        }
    }
    void OnPCCursor(int x,int y){
        mouse_cursor.x=x;
        mouse_cursor.y=Window::height-y;

        SetEvent(Event::PC_Cursor);
    }
    void OnPCClick(bool down, bool left){
        if(left){
            mouse_l.state=(down)?3:2;
            SetEvent(Event::PC_LClick);
        }
        else {
            mouse_r.state=(down)?3:2;
            SetEvent(Event::PC_RClick);
        }
    }
    void OnPCScroll(int dx, int dy){
        mouse_scroll.x = dx;
        mouse_scroll.y = dy;
        SetEvent(Event::PC_Scroll);
    }
    void OnText(wchar* text){
        input_text = wstr::new_copy(text);
        SetEvent(Event::PC_Text);
    }
    void OnCharacter(int code_point){
        character_code[0]=code_point;
        character_code[1]=0;
        input_text = character_code;
        SetEvent(Event::PC_Text);
    }

    //////////////////////
    //    Event flag management
    ////////////////////
    Event NextEvent(){
        int event_count=static_cast<int>(Event::COUNT);
        for(int i=0; i<event_count;i++){
            if(Bit::Get(input_state,i)){return static_cast<Event>(i + 1);}
        }
        return Event::None;
    }
    Event NextEvent(Event start){
        int start_index=static_cast<int>(start);
        int event_count=static_cast<int>(Event::COUNT);
        if(start_index < 0 || (start_index + 1) >= event_count) return Event::None;
        for(int i=start_index; i<event_count;i++){
            if(Bit::Get(input_state,i)){return static_cast<Event>(i + 1);}
        }
        return Event::None;
    }
    void SetEvent(Event event){
        Bit::Set(&input_state,static_cast<int>(event)-1);
    }
    void ClearEvent(Event event){
        if(event <= Event::None || event >= Event::COUNT)return;
        Bit::Clear(&input_state,(static_cast<byte>(event))-1);
    }
    void ClearAllEvents(){
        input_state=0;
    }


    //////////////////////
    //    Key binding management
    ////////////////////
    void AddKeyButtonBind(ButtonID button, int key_id){
        Key_Button_Bind* newbind = button_binds.Add();
        newbind->button=button;
        newbind->physical_key_id=key_id;
    }
    void RemoveKeyButtonBind(ButtonID button, int key_id){
        for(int i=0;i< button_binds.Count();i++){
            if(button_binds[i]->button==button && button_binds[i]->physical_key_id==key_id){
                button_binds.Delete(i);
                i=0;
            }
        }
    }
    void AddKeyAxisBind(AxisID axis, int key_id, int direction_id){
        Key_Axis_Bind* newbind = key_axis_binds.Add();
        newbind->axis=axis;
        newbind->direction_id=direction_id;
        newbind->physical_key_id=key_id;
    }
    void RemoveKeyAxisBind(AxisID axis, int key_id){
        for(int i=0;i< key_axis_binds.Count();i++){
            if(key_axis_binds[i]->physical_key_id ==key_id && key_axis_binds[i]->axis==axis){
                key_axis_binds.Delete(i);
                i=0;
            }
        }
    }
    void AddAxisBind(AxisID axis, int physical_axis_id){
        Axis_Bind* newbind = axis_binds.Add();
        newbind->axis=axis;
        newbind->physical_axis_id=physical_axis_id;
    }
    void ClearButtonBinds(int key_id){
        for(int i=0;i< button_binds.Count();i++){
            if(button_binds[i]->physical_key_id==key_id){
                button_binds.Delete(i);
                i=0;
            }
        }
        for(int i=0;i< key_axis_binds.Count();i++){
            if(key_axis_binds[i]->physical_key_id==key_id){
                key_axis_binds.Delete(i);
                i=0;
            }
        }
    }
    void ClearAxisBind(int physical_axis_id){
        for(int i=0;i< button_binds.Count();i++){
            if(axis_binds[i]->physical_axis_id == physical_axis_id){
                axis_binds.Delete(i);
                i=0;
            }
        }
    }
    void UnbindButton(ButtonID button){
        for(int i=0;i< button_binds.Count();i++){
            if(button_binds[i]->button == button){
                key_axis_binds.Delete(i);
                i=0;
            }
        }
    }
    void UnbindAxis(AxisID axis){
        for(int i=0;i< button_binds.Count();i++){
            if(axis_binds[i]->axis == axis){
                axis_binds.Delete(i);
                i=0;
            }
        }
        for(int i=0;i< key_axis_binds.Count();i++){
            if(key_axis_binds[i]->axis == axis){
                key_axis_binds.Delete(i);
                i=0;
            }
        }
    }

    //////////////////////
    //    Typing mode
    ////////////////////
    void SetKeyboardTypingOnly(bool enabled){
        keyboard_type_only=enabled;
    }


    //////////////////////
    //    Utility
    ////////////////////
    int GetKeyID(const char* key_name){
        for(Tuple<int,char*> t: physical_input_map){
            if(cstr::compare(t.value,key_name))return t.key;
        }
        return -1;
    }
    const char* GetKeyName(int key_id){
        if(physical_input_map.Has(key_id)){
            return physical_input_map.Get(key_id);
        }
        return nullptr;
    }

    /*
    EventID GetEventID(char* event_name){
        for(int i=1;i<=AXE=;i++){if(cstr::starts_with(event_name,input_names[i])){return (EventID)(i);}}
        for(int i=1;i<=button_count;i++){
            if(cstr::compare(event_name,input_names[i+axis_count])){
                return (EventID)(axis_count+i);
            }
        }
        return EventID::None;
    }
    const char* GetEventName(EventID event){

    }
    */

    //////////////////////
    //    Loading
    ////////////////////
    void LoadKeyLayout(const char* layout_filename){
        FileReader layout_file(layout_filename);

        for(byte* line=layout_file.ReadLine();line != null;line=layout_file.ReadLine()){
            if(line[0]=='[')continue;
            char* key = cstr::substr_before((char*)line,'=');
            char* value = cstr::substr_after((char*)line,'=');
            if(key!=nullptr&&value!=nullptr){
                int key_id = cstr::integer_from_string(value);
                if(key_id > 0){
                    physical_input_map.Add(key_id,cstr::new_copy(key));
                }
                if(cstr::compare("enter",key)){enter_physical_key=key_id;}
                if(cstr::compare("escape",key)){escape_physical_key=key_id;}
            }
            if(key)Free(key);
            if(value)Free(value);
        }
    }

    void LoadKeyBindings(const char** lines,int line_count) {
        for(int i=0;i<line_count;i++){
            char* key = cstr::substr_before(lines[i],'=');
            char* value = cstr::substr_after(lines[i],'=');

            int physical_input_id = GetKeyID(key);

            for(int j=0;j<BUTTON_COUNT;j++){
                if(cstr::compare(button_names[j],value)){
                    AddKeyButtonBind((ButtonID)j,physical_input_id);
                }
            }
            for(int j=0;j<AXIS_COUNT;j++){
                if(cstr::compare(axis_names[j],value)){
                    AddAxisBind((AxisID)j,physical_input_id);
                }
                else if(cstr::starts_with(value,axis_names[j])){
                    char* direction_name = cstr::substr_after(value,'.');
                    int direction_id=-1;
                    for(int k=0;k<4;k++){
                        if(cstr::compare(direction_name,axis_directions[k])){
                            direction_id = k;
                        }
                    }
                    AddKeyAxisBind(static_cast<AxisID>(j),physical_input_id,direction_id);
                }
            }
        }
    }

    void LoadDefaultKeyBindings(){
        const char* keybinds[] = {
            "w=move.up", "a=move.left", "s=move.down", "d=move.right",
            "n=a", "m=b", "j=c", "k=d",
            "q=l1", "e=r1",
            "enter=pause",
            "tab=menu",
            "joy_lstick=move",
            "joy_lstick=down",
            "joy_10=move.up", "joy_11=move.down", "joy_12=move.left", "joy_13=move.right",
            "joy_2=a", "joy_3=b", "joy_1=c", "joy_4=d",
            "joy_5=l1", "joy_6=r1",
            "joy_9=menu",
            "joy_10=pause"
        };
        LoadKeyBindings(keybinds,26);
        toggle_console_key_id = GetKeyID("tilde");
    }
};




//////////////////////
//    Input state accessors
////////////////////
Controller::Button  Controller::GetButton(ButtonID id){return Input::controller_buttons[static_cast<int>(id)];}
Controller::Axis    Controller::GetAxis(AxisID axis){return Input::controller_axes[static_cast<int>(axis)];}
point_i             Controller::GetPCCursor(){return Input::mouse_cursor;}
Controller::Button  Controller::GetPCLeftMouse(){return Input::mouse_l;}
Controller::Button  Controller::GetPCRightMouse(){return Input::mouse_r;}
Controller::Button  Controller::GetPCCenterMouse(){return Input::mouse_c;}
vec2                Controller::GetPCScroll(){return Input::mouse_scroll;}
Controller::Button  Controller::GetAnyButton(){return Input::any_button;}
Controller::Button  Controller::GetToggleConsole(){return Input::toggle_console;}
wchar*              Controller::GetTextInput(){return Input::input_text;}


















