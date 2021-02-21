#include "input.h"
#include "config.h"
#include "io/file.h"
#include "log.h"
#include <math.h>
#include <string.h>
#include <stdlib.h>


vec2 Input::Axis::GetNormalized(){
    vec2 ret = {(float)(raw_x-min_x)/(float)(max_x-min_x)-0.5f,
                (float)(raw_y-min_y)/(float)(max_y-min_x)-0.5f};

    ret.x *= 2.0f;
    ret.y *= 2.0f;

    if(ret.length_sqr() < AXIS_CUTOFF){return {0,0};}
    if(ret.x * ret.x < AXIS_CUTOFF){ret.x=0;}
    if(ret.y * ret.y < AXIS_CUTOFF){ret.y=0;}
    if(ret.length_sqr() > 1.0){ret.normalize();}
    
    return ret;
}

//Don't do this if axis is not zeroed!
//TODO: make work on [0,1] and other odd-range axes.
void Input::Axis::SetBounds(int maxx,int minx,int maxy, int miny){
    max_x=maxx;
    min_x=minx;
    max_y=maxy;
    min_y=miny;
    raw_x = max_x+min_x/2;
    raw_y = max_y+min_y/2;
}

bool Input::Button::IsDown(){return (state&1) > 0;}
bool Input::Button::IsUp(){return (state&1) == 0;}
bool Input::Button::IsJustPressed(){return state == 3;}
bool Input::Button::IsJustReleased(){return state == 2;}

const int       axis_count = 4;
const int       button_count = 16;
const int       special_input_count = 3;
const int       input_count = axis_count+button_count+special_input_count;
const char*     input_names[1+axis_count+button_count] = {
    "none",
    "move","camera","move_cursor","scroll",
    "button_a", "button_b", "button_c", "button_d",
    "d_pad_up","d_pad_down","d_pad_left","d_pad_right",
    "l1","l2","r1","r2",
    "pause","menu",
    "cursor_select",
    "toggle_console",
};

const int       axis_direction_count=8;
const char*     axis_directions[8] = {
    "up",
    "down",
    "left",
    "right",
    "up-left",
    "up-right",
    "down-left",
    "down-right"
};

Input::Axis     axes[axis_count];
Input::Button   buttons[button_count];
Input::Button   any_button;

text_char*      input_text=null;            
text_char       character_code[2]  = {0,0};

int             state_changed_flags;
int             key_bind_count;
Input::KeyBind  *key_binds;

IDMap           physical_input_map= IDMap(8);

void Input::Init(){
    memset(&axes,0,sizeof(Input::Axis)*axis_count);
    for(int i=0;i<axis_count;i++){axes[i].max_x=1;axes[i].min_x=-1;axes[i].max_y=1;axes[i].min_y=-1;}
    memset(&buttons,0,sizeof(Input::Button)*button_count);
    any_button.state=0;
    any_button.event_id=Input::EventID::Any_Button;
    state_changed_flags=0;
    LoadKeyLayout("dat/misc/en_us_keyids.txt");
    LoadKeyBindings();
}

void Input::Destroy(){
    ClearInputText();
    free(key_binds);
}

void Input::RegisterPhysicalInput(char* name,int id){
    physical_input_map.Add(id,(byte*)name);
}

void Input::LoadKeyBindings(){
    key_bind_count = config::key_binding_count;
    key_binds = (KeyBind*)calloc(key_bind_count,sizeof(Input::KeyBind));;

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
}

void Input::SetAxisBounds(int axis_id,int xmax,int xmin,int ymax,int ymin){
    if(axis_id >= 0 && axis_id < axis_count){
        axes[axis_id].SetBounds(xmax,xmin,ymax,ymin);
    }
}

void Input::SetPhysicalAxisDirection(int input_id,bool flip_x,bool flip_y){
    for(int i=0;i<key_bind_count;i++){
        if(key_binds[i].physical_input==input_id){
            key_binds[i].flip_x=flip_x;
            key_binds[i].flip_y=flip_y;
        }
    }
}

void Input::PostUpdate(){
    state_changed_flags=0;
    for(int i=0;i< axis_count;i++){
        axes[i].dx=0;axes[i].dy=0;
    }
    for(int i=0;i< button_count;i++){
        if(buttons[i].state == 3){
            buttons[i].state=1;
            }
        if(buttons[i].state == 2){
            buttons[i].state=0;
            }
    }
    if(any_button.state == 3){any_button.state=1;}
    if(any_button.state == 2){any_button.state=0;}
    ClearInputText();
}

Input::EventID Input::GetEventID(char* event_name){
    for(int i=1;i<=axis_count;i++){if(cstr::starts_with(event_name,input_names[i])){return (Input::EventID)(i);}}
    for(int i=1;i<=button_count;i++){
        if(cstr::compare(event_name,input_names[i+axis_count])){
            return (Input::EventID)(axis_count+i);
            }
        }
    return Input::EventID::None;
}

const char* Input::GetEventName(Input::EventID event){
    if(event >= 0 && event <= button_count+axis_count)return input_names[event];
    else return "error";
}


Input::KeyBind Input::GetKeyBindByInput(int input_id){
    for(int i=0;i<key_bind_count;i++){if(key_binds[i].physical_input==input_id)return key_binds[i];}
    return {input_id,EventID::None,0};
}

Input::EventID Input::NextEvent(){
    int input_id_mask=1;
    for(int i=1; i<input_count;i++){
        if((state_changed_flags& input_id_mask) > 0){
            state_changed_flags = state_changed_flags ^ input_id_mask;
            return (EventID)i;
        }
        input_id_mask = input_id_mask << 1;
    }
    return EventID::None;
}

Input::Axis Input::Move_Axis(){
    return axes[0];
}
Input::Axis Input::Cam_Axis(){return axes[1];}
Input::Axis Input::Cursor_Axis(){return axes[2];}
Input::Axis Input::Scroll_Axis(){return axes[3];}

Input::Button Input::AnyButton(){return any_button;}
Input::Button Input::Button_A(){return buttons[0];}
Input::Button Input::Button_B(){return buttons[1];}
Input::Button Input::Button_C(){return buttons[2];}
Input::Button Input::Button_D(){return buttons[3];}
Input::Button Input::Button_Up(){return buttons[4];} 
Input::Button Input::Button_Down(){return buttons[5];} 
Input::Button Input::Button_Left(){return buttons[6];} 
Input::Button Input::Button_Right(){return buttons[7];} 
Input::Button Input::Button_L1(){return buttons[8];} 
Input::Button Input::Button_L2(){return buttons[9];} 
Input::Button Input::Button_R1(){return buttons[10];} 
Input::Button Input::Button_R2(){return buttons[11];} 
Input::Button Input::Button_Pause(){return buttons[12];} 
Input::Button Input::Button_Menu(){return buttons[13];} 
Input::Button Input::Button_ToggleConsole(){return buttons[14];} 
Input::Button Input::Button_Cursor_Select(){return buttons[15];} 

text_char* Input::TextInput(){return input_text;}


void HandleBoolAsButtonEvent(Input::KeyBind keybind,bool down){
    int event_flag = 1 << (keybind.event - 1);
    state_changed_flags |= event_flag;
    int button_id = (keybind.event-1)-axis_count;
    buttons[button_id].state=down?3:2;
}
void HandleBoolAsAxisEvent(Input::KeyBind keybind,bool down){
    int event_flag = 1 << (keybind.event - 1);
    state_changed_flags |= event_flag;
    int axis_id = (keybind.event-1);

    int prev_axis[2] {axes[axis_id].raw_x,axes[axis_id].raw_y};

    int dir[2] = {0,0};
    switch(keybind.axis_direction){
        case 0:dir[1]=1;break;
        case 1:dir[1]=-1;break;
        case 2:dir[0]=-1;break;
        case 3:dir[0]=1;break;
        case 4:dir[0]=-1;dir[1]=1;break;
        case 5:dir[0]=1; dir[1]=1;break;
        case 6:dir[0]=-1;dir[1]=-1;break;
        case 7:dir[0]=1; dir[1]=-1;break;
        default:break;
    }

    int x_axis_avg = (axes[axis_id].max_x+axes[axis_id].min_x)/2;
    int y_axis_avg = (axes[axis_id].max_y+axes[axis_id].min_y)/2;
    int x_axis_travel = axes[axis_id].max_x-x_axis_avg;
    int y_axis_travel = axes[axis_id].max_y-y_axis_avg;

    axes[axis_id].raw_x += (down ^ keybind.flip_x) ? x_axis_travel*dir[0] : -x_axis_travel*dir[0];
    axes[axis_id].raw_y += (down ^ keybind.flip_y) ? y_axis_travel*dir[1] : -y_axis_travel*dir[1];
    if(axes[axis_id].raw_x > axes[axis_id].max_x){axes[axis_id].raw_x = axes[axis_id].max_x;}
    if(axes[axis_id].raw_x < axes[axis_id].min_x){axes[axis_id].raw_x = axes[axis_id].min_x;}
    if(axes[axis_id].raw_y > axes[axis_id].max_y){axes[axis_id].raw_y = axes[axis_id].max_y;}
    if(axes[axis_id].raw_y < axes[axis_id].min_y){axes[axis_id].raw_y = axes[axis_id].min_y;}

    axes[axis_id].dx = axes[axis_id].raw_x - prev_axis[0];
    axes[axis_id].dy = axes[axis_id].raw_y - prev_axis[1];
}
void HandleAxisAsButtonEvent(Input::KeyBind keybind,int pos_x,int pos_y){
    //Moving at all = down. 
    int button_id = (keybind.event-1)-axis_count;
    int event_flag = 1 << (keybind.event - 1);
    if(buttons[button_id].IsDown()&& pos_x==pos_y && pos_y==0){
        state_changed_flags |= event_flag;
        buttons[button_id].state=2;
    }
    else if(buttons[button_id].IsUp() && (pos_x !=pos_y || pos_y!=0)){
        state_changed_flags |= event_flag;
        buttons[button_id].state=3;
    }
}
void HandleAxisAsAxisEvent(Input::KeyBind keybind,int pos_x,int pos_y){
    int event_flag = 1 << (keybind.event - 1);
    state_changed_flags |= event_flag;
    int axis_id = (keybind.event-1);

    int prev_axis[2] {axes[axis_id].raw_x,axes[axis_id].raw_y};

    axes[axis_id].raw_x= keybind.flip_x ? axes[axis_id].max_x-pos_x : pos_x;
    axes[axis_id].raw_y= keybind.flip_y ? axes[axis_id].max_y-pos_y : pos_y;
    if(pos_x > axes[axis_id].max_x){axes[axis_id].max_x = pos_x;}   if(-pos_x > axes[axis_id].max_x){axes[axis_id].max_x = -pos_x;}
    if(pos_y > axes[axis_id].max_y){axes[axis_id].max_y = pos_y;}   if(-pos_y > axes[axis_id].max_y){axes[axis_id].max_y = -pos_y;}
    //Let's try to have axis' figure out their own max values.

    axes[axis_id].dx = axes[axis_id].raw_x - prev_axis[0];
    axes[axis_id].dy = axes[axis_id].raw_y - prev_axis[1];
}

void Input::HandleBool(int key_id, bool down){
    Input::KeyBind bind = GetKeyBindByInput(key_id);
    if(bind.event != Input::EventID::None){
        if(bind.event <= axis_count){HandleBoolAsAxisEvent(bind,down);}
        else{HandleBoolAsButtonEvent(bind,down);}
    }
}
void Input::HandleIntAxis(int axis_id,int pos_x,int pos_y){
    Input::KeyBind bind = GetKeyBindByInput(axis_id);
    if(bind.event != Input::EventID::None){
        if(bind.event <= axis_count){HandleAxisAsAxisEvent(bind,pos_x,pos_y);}
        else{HandleAxisAsButtonEvent(bind,pos_x,pos_y);}
    }
}
//void HandleFloatAxis(float pos_x,int pos_y);
void Input::HandleCharacter(int code_point){
    character_code[0]=code_point;
    input_text = character_code;
    state_changed_flags |= Input::EventID::Text;
}

void Input::HandleText(text_char* text){
    input_text = TextString::copy(text);
    state_changed_flags |= Input::EventID::Text;
}

void Input::ClearInputText(){
    state_changed_flags ^= (1 << (Input::EventID::Text-1));//clear text event;
    character_code[0]=0;
    if(input_text != nullptr){
        if(input_text != character_code){free(input_text);}
        input_text = nullptr;
    }
}


int Input::GetKeyID(char* key_name){
    char* phys_input_id=null;
    for(int i=0;i< physical_input_map.slots;i++){
        if(!physical_input_map.slot_is_filled.Get(i))continue;
        phys_input_id=(char*)physical_input_map.values[i];
        if(cstr::compare(key_name,phys_input_id)){
            return physical_input_map.keys[i];
        }
    }
    return -1;
}

const char* Input::GetKeyName(int key_id){
    if(physical_input_map.Has(key_id)){
        return (char*)physical_input_map.Get(key_id);
    }
    return nullptr;
}

void Input::LoadKeyLayout(char* layout_filename){
    FileReader layout_file(layout_filename);

    for(byte* line=layout_file.ReadLine();line != null;line=layout_file.ReadLine()){
        if(line[0]=='[')continue;
        char* key = cstr::substr_before((char*)line,'=');
        char* value = cstr::substr_after((char*)line,'=');
        if(key==nullptr||value==nullptr){
            if(key)free(key);
            if(value)free(value);
        }
        int key_id = cstr::integer_from_string(key);
        if(key_id > 0){
            physical_input_map.Add(key_id,(byte*)cstr::new_copy(value));
        }
        
        free(key);
        free(value);
    }
}