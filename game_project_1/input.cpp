#include <game_project_1/input.hpp>
#include <math.h>
#include <game_project_1/types/primitives.hpp>
#include <game_project_1/types/pool.hpp>
#include <game_project_1/types/map.hpp>
#include <game_project_1/io/file.hpp>
#include <game_project_1/os.hpp>

const int BUTTON_COUNT=14;
const int AXIS_COUNT=2;
const float AXIS_CUTOFF = 0.01;

const int AXIS_EVENT_BASE=0;
const int BUTTON_EVENT_BASE=2;

const char* button_names[BUTTON_COUNT] = 
{   "a","b","c","d",
    "d_up","d_down","d_left","d_right",
    "l1","l2","r1","r2",
    "pause","menu"};

const char* axis_names[AXIS_COUNT] = {"move","camera"};
const char* axis_directions[4] = {"up","down","left","right"};

//bitflag representing whether or not the state of EventID (i-1) has changed
unsigned long int input_state = 0;

Controller::Axis controller_axes[AXIS_COUNT];
Controller::Button controller_buttons[BUTTON_COUNT];

point_i mouse_cursor;
vec2    mouse_scroll;
Controller::Button mouse_l,mouse_r,mouse_c;

Controller::Button toggle_console;
Controller::Button any_button;


wchar* input_text=null;            
wchar  character_code[2]  = {0,0};

Dictionary<int,char*> physical_input_map= Dictionary<int,char*>(8);

Pool<Input::Key_Button_Bind> button_binds;
Pool<Input::Key_Axis_Bind> key_axis_binds;
Pool<Input::Axis_Bind> axis_binds;

int toggle_console_key_id=0;

bool keyboard_type_only=false;
int enter_physical_key;//these two keys are still enabled as inputs in typing mode
int escape_physical_key;

const char* keylayout_filename = "dat/misc/en_us_keyids.txt";

//////////////////////
//    Engine events
////////////////////

void SetEvent(Input::Event event_id){
    SET_BIT(input_state,(event_id-1));
}

void Input::Init(){
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
    
    if(IFile::Exists(keylayout_filename)){LoadKeyLayout(keylayout_filename);}
    
    LoadDefaultKeyBindings();
}

void Input::Update(){
    //clear "changed" flag for buttons
    for(int i=0;i<BUTTON_COUNT;i++){
        CLEAR_BIT(controller_buttons[i].state,1);
    }
    
    CLEAR_BIT(mouse_l.state,1);
    CLEAR_BIT(mouse_r.state,1);
    CLEAR_BIT(mouse_c.state,1);
    CLEAR_BIT(toggle_console.state,1);
    CLEAR_BIT(any_button.state,1);
    //clear deltas for axes
    for(int i=0;i<AXIS_COUNT;i++){
        controller_axes[i].ClearTiltDelta();
    }
    //clear input text
    character_code[0]=0;
    if(input_text != nullptr){
        if(input_text != character_code){free(input_text);}
        input_text = nullptr;
    }
    ClearAllEvents();
}

void Input::Destroy(){
    for(Tuple<int,char*> t: physical_input_map){if(t.value)free(t.value);}
    physical_input_map.Clear();
    button_binds.Clear();
    key_axis_binds.Clear();
    axis_binds.Clear();
}


//////////////////////
//    Input event handlers
////////////////////

void Input::OnKeyboardKey(int key_id,bool down){
    if(keyboard_type_only && key_id != enter_physical_key && key_id != escape_physical_key){return;}

    if(key_id == enter_physical_key && down){SetEvent(Event::PC_Return);}
    if(key_id == escape_physical_key && down){SetEvent(Event::PC_Escape);}

    OnKey(key_id,down);
}

void Input::OnKey(int key_id,bool down){
    for(Key_Button_Bind* keybind:button_binds){
        if(key_id == keybind->physical_key_id){
            controller_buttons[keybind->button].state = (down)?3:2;
            SET_BIT(input_state,BUTTON_EVENT_BASE+keybind->button);
        }
    }
    for(Key_Axis_Bind* keybind:key_axis_binds){
        if(key_id == keybind->physical_key_id){
            controller_axes[keybind->axis].SetDirection(keybind->direction_id,down);
            SET_BIT(input_state,AXIS_EVENT_BASE+keybind->axis);
        }
    }
    if(key_id == toggle_console_key_id){
        toggle_console.state=(down)?3:2; 
        SetEvent(Event::ToggleConsole);
    }
}
void Input::OnAxis(int axis_id,float x,float y){
    for(Axis_Bind* keybind:axis_binds){
        if(axis_id == keybind->physical_axis_id){
            controller_axes[keybind->axis].SetTilt({x,y});
            SET_BIT(input_state,AXIS_EVENT_BASE+keybind->axis);
        }
    }
}
void Input::OnPCCursor(int x,int y){
    mouse_cursor.x=x;
    mouse_cursor.y=Window::height-y;
    
    SetEvent(Input::PC_Cursor);
}
void Input::OnPCClick(bool down, bool left){
    if(left){
        mouse_l.state=(down)?3:2; 
        SetEvent(Input::PC_LClick);
    }
    else {
        mouse_r.state=(down)?3:2; 
        SetEvent(Input::PC_RClick);
    }
}
void Input::OnPCScroll(int dx, int dy){
    mouse_scroll.x = dx;
    mouse_scroll.y = dy;
    SetEvent(Input::PC_Scroll);
}
void Input::OnText(wchar* text){
    input_text = wstr::new_copy(text);
    SetEvent(Input::PC_Text);
}
void Input::OnCharacter(int code_point){
    character_code[0]=code_point;
    character_code[1]=0;
    input_text = character_code;
    SetEvent(Input::PC_Text);
}

//////////////////////
//    Event flag management
////////////////////
Input::Event Input::NextEvent(){
    for(int i=0; i<Input::Event::COUNT;i++){
        if(GET_BIT(input_state,i)){return (Input::Event)(i+1);}
    }
    return Input::Event::None;
}
Input::Event Input::NextEvent(Input::Event start){
    if(start < 0 || (start + 1) >= Input::Event::COUNT)return Input::Event::None;
    for(int i=start; i<Input::Event::COUNT;i++){
        if(GET_BIT(input_state,i)){return (Input::Event)(i+1);}
    }
    return Input::Event::None;
}
void Input::ClearEvent(Input::Event event){
    if(event <= 0 || event >= Input::Event::COUNT)return;
    CLEAR_BIT(input_state,(int)(event-1));
}
void Input::ClearAllEvents(){
    input_state=0;
}

//////////////////////
//    Input state accessors
////////////////////
Controller::Button  Controller::GetButton(ButtonID id){return controller_buttons[id];}
Controller::Axis    Controller::GetAxis(AxisID axis){return controller_axes[axis];}
point_i             Controller::GetPCCursor(){return mouse_cursor;}
Controller::Button  Controller::GetPCLeftMouse(){return mouse_l;}
Controller::Button  Controller::GetPCRightMouse(){return mouse_r;}
Controller::Button  Controller::GetPCCenterMouse(){return mouse_c;}
vec2                Controller::GetPCScroll(){return mouse_scroll;}
Controller::Button  Controller::GetAnyButton(){return any_button;}
Controller::Button  Controller::GetToggleConsole(){return toggle_console;}
wchar*              Controller::GetTextInput(){return input_text;}

//////////////////////
//    Key binding management
////////////////////
void Input::AddKeyButtonBind(ButtonID button, int key_id){
    Key_Button_Bind* newbind = button_binds.Add();
    newbind->button=button;
    newbind->physical_key_id=key_id;
}
void Input::RemoveKeyButtonBind(ButtonID button, int key_id){
    for(int i=0;i< button_binds.Count();i++){
        if(button_binds[i]->button==button && button_binds[i]->physical_key_id==key_id){
            button_binds.Delete(i);
            i=0;
        }
    }
}
void Input::AddKeyAxisBind(AxisID axis, int key_id, int direction_id){
    Key_Axis_Bind* newbind = key_axis_binds.Add();
    newbind->axis=axis;
    newbind->direction_id=direction_id;
    newbind->physical_key_id=key_id;
}
void Input::RemoveKeyAxisBind(AxisID axis, int key_id){
    for(int i=0;i< key_axis_binds.Count();i++){
        if(key_axis_binds[i]->physical_key_id ==key_id && key_axis_binds[i]->axis==axis){
            key_axis_binds.Delete(i);
            i=0;
        }
    }
}
void Input::AddAxisBind(AxisID axis, int physical_axis_id){
    Axis_Bind* newbind = axis_binds.Add();
    newbind->axis=axis;
    newbind->physical_axis_id=physical_axis_id;
}
void Input::ClearButtonBinds(int key_id){
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
void Input::ClearAxisBind(int physical_axis_id){
    for(int i=0;i< button_binds.Count();i++){
        if(axis_binds[i]->physical_axis_id == physical_axis_id){
            axis_binds.Delete(i);
            i=0;
        }
    }
}
void Input::UnbindButton(ButtonID button){
    for(int i=0;i< button_binds.Count();i++){
        if(button_binds[i]->button == button){
            key_axis_binds.Delete(i);
            i=0;
        }
    }
}
void Input::UnbindAxis(AxisID axis){
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

void Input::SetKeyboardTypingOnly(bool enabled){
    keyboard_type_only=enabled;
}

//////////////////////
//    Utility
////////////////////
int Input::GetKeyID(char* key_name){
    for(Tuple<int,char*> t: physical_input_map){
        if(cstr::compare(t.value,key_name))return t.key;
    }
    return -1;
}
const char* Input::GetKeyName(int key_id){
    if(physical_input_map.Has(key_id)){
        return physical_input_map.Get(key_id);
    }
    return nullptr;
}

/*
Input::EventID Input::GetEventID(char* event_name){
    for(int i=1;i<=AXE=;i++){if(cstr::starts_with(event_name,input_names[i])){return (Input::EventID)(i);}}
    for(int i=1;i<=button_count;i++){
        if(cstr::compare(event_name,input_names[i+axis_count])){
            return (Input::EventID)(axis_count+i);
            }
        }
    return Input::EventID::None;
}
const char* Input::GetEventName(EventID event){

}
*/

//////////////////////
//    Axis
////////////////////
void Controller::Axis::AddTilt(vec2 tilt){
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

void Controller::Axis::SetTilt(vec2 tilt){
    dx = tilt.x-x;
    dy = tilt.y-y;
    x = tilt.x;
    y = tilt.y;
    if(x*x + y*y < AXIS_CUTOFF){x=0;y=0;}//deadzone
    UpdateTilt();
}

void Controller::Axis::SetDirection(short direction_id,bool down){
    if(down){SET_BIT(key_flags,direction_id);}
    else{CLEAR_BIT(key_flags,direction_id);}

    vec2 old_pos = {x,y};
    x=0;y=0;
    if(GET_BIT(key_flags,UP)){y += 1.0f;}
    if(GET_BIT(key_flags,DOWN)){y -= 1.0f;}
    if(GET_BIT(key_flags,LEFT)){x -= 1.0f;}
    if(GET_BIT(key_flags,RIGHT)){x += 1.0f;}
        
    vec2 new_pos = GetNormalized();
    x=new_pos.x;
    y=new_pos.y;
    dx=x-old_pos.x;
    dy=x-old_pos.y;
    UpdateTilt();
}

void Controller::Axis::UpdateTilt(){
    if(y > tilt_cutoff && !GET_BIT(tilt_flags,UP*2)){ SET_BIT(tilt_flags,UP*2); SET_BIT(tilt_flags,(UP*2)+1);}
    else if(y < tilt_cutoff && GET_BIT(tilt_flags,UP*2)){ CLEAR_BIT(tilt_flags,UP*2); SET_BIT(tilt_flags,(UP*2)+1);}
    
    if(y < -tilt_cutoff && !GET_BIT(tilt_flags,DOWN*2)){ SET_BIT(tilt_flags,DOWN*2); SET_BIT(tilt_flags,(DOWN*2)+1);}
    else if(y > -tilt_cutoff && GET_BIT(tilt_flags,DOWN*2)){ CLEAR_BIT(tilt_flags,DOWN*2); SET_BIT(tilt_flags,(DOWN*2)+1);}
    
    if(x < -tilt_cutoff && !GET_BIT(tilt_flags,LEFT*2)){ SET_BIT(tilt_flags,LEFT*2); SET_BIT(tilt_flags,(LEFT*2)+1);}
    else if(x > -tilt_cutoff && GET_BIT(tilt_flags,LEFT*2)){ CLEAR_BIT(tilt_flags,LEFT*2); SET_BIT(tilt_flags,(LEFT*2)+1);}

    if(x > tilt_cutoff && !GET_BIT(tilt_flags,RIGHT*2)){ SET_BIT(tilt_flags,RIGHT*2); SET_BIT(tilt_flags,(RIGHT*2)+1);}
    else if(x < tilt_cutoff && GET_BIT(tilt_flags,RIGHT*2)){ CLEAR_BIT(tilt_flags,RIGHT*2); SET_BIT(tilt_flags,(RIGHT*2)+1);}
}

void Controller::Axis::ClearTiltDelta(){
    dx=0;
    dy=0;
    CLEAR_BIT(tilt_flags,(UP*2)+1);
    CLEAR_BIT(tilt_flags,(DOWN*2)+1);
    CLEAR_BIT(tilt_flags,(LEFT*2)+1);
    CLEAR_BIT(tilt_flags,(RIGHT*2)+1);
}

vec2 Controller::Axis::GetNormalized(){
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

bool Controller::Axis::IsUp(){
    return GET_BIT(this->tilt_flags,(UP*2));
}
bool Controller::Axis::IsDown(){
    return GET_BIT(this->tilt_flags,(DOWN*2));
}
bool Controller::Axis::IsLeft(){
    return GET_BIT(this->tilt_flags,(LEFT*2));
}
bool Controller::Axis::IsRight(){
    return GET_BIT(this->tilt_flags,(RIGHT*2));
}

bool Controller::Axis::IsJustUp(){
    return GET_BIT(this->tilt_flags,(UP*2)) && GET_BIT(this->tilt_flags,(UP*2)+1);
}
bool Controller::Axis::IsJustDown(){
    return GET_BIT(this->tilt_flags,(DOWN*2)) && GET_BIT(this->tilt_flags,(DOWN*2)+1);
}
bool Controller::Axis::IsJustLeft(){
    return GET_BIT(this->tilt_flags,(LEFT*2)) && GET_BIT(this->tilt_flags,(LEFT*2)+1);
}
bool Controller::Axis::IsJustRight(){
    return GET_BIT(this->tilt_flags,(RIGHT*2)) && GET_BIT(this->tilt_flags,(RIGHT*2)+1);
}



//////////////////////
//    Loading
////////////////////
void Input::LoadKeyLayout(const char* layout_filename){
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
        if(key)free(key);
        if(value)free(value);
    }
}

void Input::LoadDefaultKeyBindings(){
    char* keybinds[] = { 
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

void Input::LoadKeyBindings(char** lines,int line_count){
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
                AddKeyAxisBind((AxisID)j,physical_input_id,direction_id);
            }
        }
    }
/*
    for(int i=0;i< config::key_binding_count;i++){
        StringPair binding = config::key_bindings[i];

        key_binds[i].physical_input = GetKeyID(binding.value);
        key_binds[i].event = GetEventID(binding.key);
        //logger::info("Assigning input %d to event %d\n",key_binds[i].physical_input,key_binds[i].event);
        if(key_binds[i].event >0 &&  key_binds[i].event<= axis_count){
            for(int j=0;j<axis_direction_count;j++){
                if(cstr::contains(binding.key,axis_directions[j])){
                    key_binds[i].axis_direction=j;
                }
            }
        }
    }
    */
}       
