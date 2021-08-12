#include "input.h"
#include <math.h>
#include "struct/list.h"
#include "io/file.h"

const int BUTTON_COUNT=14;
const int AXIS_COUNT=2;
const float AXIS_CUTOFF = 0.01;

const int AXIS_EVENT_BASE=0;
const int BUTTON_EVENT_BASE=2;

const int PC_EVENT_CURSOR =16;
const int PC_EVENT_SCROLL =17;
const int PC_EVENT_RCLICK =18;
const int PC_EVENT_LCLICK =19;
const int PC_EVENT_CCLICK =20;
const int PC_EVENT_TEXT = 21;

const int EVENT_CONSOLE = 22;
const int EVENT_ANYBUTTON = 23;
const int EVENT_COUNT=24;

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


text_char* input_text=null;            
text_char  character_code[2]  = {0,0};

IDMap physical_input_map= IDMap(8);

List<Input::Key_Button_Bind> button_binds;
List<Input::Key_Axis_Bind> key_axis_binds;
List<Input::Axis_Bind> axis_binds;


//////////////////////
//    Engine events
////////////////////

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
    }
    char* en_us_keylayout_filename="dat/misc/en_us_keyids.txt";
    if(File::Exists(en_us_keylayout_filename)){
        LoadKeyLayout(en_us_keylayout_filename);
    }
    
    LoadDefaultKeyBindings();
}
void Input::Update(){
    //Clear previous frame's state

    //clear "changed" flag for buttons
        for(int i=0;i<BUTTON_COUNT;i++){
            controller_buttons[i].state ^= 2; 
        }
        mouse_l.state ^= 2; mouse_r.state ^= 2; mouse_c.state ^= 2;
        toggle_console.state ^= 2;
        any_button.state ^= 2;
    //clear deltas for axes
        for(int i=0;i<AXIS_COUNT;i++){
            controller_axes[i].dx=0;controller_axes[i].dy=0;
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
    button_binds.Clear();
    key_axis_binds.Clear();
    axis_binds.Clear();
}


//////////////////////
//    Input event handlers
////////////////////
void Input::OnKey(int key_id,bool down){
    for(Key_Button_Bind* keybind:button_binds){
        if(key_id == keybind->physical_key_id){
            controller_buttons[keybind->button].state = (down)?3:2;
            SET_BIT(input_state,BUTTON_EVENT_BASE+keybind->button);
        }
    }
    for(Key_Axis_Bind* keybind:key_axis_binds){
        if(key_id == keybind->physical_key_id){
            controller_axes[keybind->axis].AddTilt(keybind->direction);
            SET_BIT(input_state,AXIS_EVENT_BASE+keybind->axis);
        }
    }
}
void Input::OnAxis(int axis_id,float x,float y){
    for(Axis_Bind* keybind:axis_binds){
        if(axis_id == keybind->physical_axis_id){
            controller_axes[keybind->axis].AddTilt({x,y});
            SET_BIT(input_state,AXIS_EVENT_BASE+keybind->axis);
        }
    }
}
void Input::OnPCCursor(int x,int y){
    mouse_cursor.x=x;
    mouse_cursor.y=y;
    SET_BIT(input_state,PC_EVENT_CURSOR);
}
void Input::OnPCClick(bool down, bool left){
    if(left){
        mouse_l.state=(down)?3:2; 
        SET_BIT(input_state,PC_EVENT_LCLICK);
    }
    else {
        mouse_r.state=(down)?3:2; 
        SET_BIT(input_state,PC_EVENT_RCLICK);
    }
}
void Input::OnPCScroll(int dx, int dy){
    mouse_scroll.x = dx;
    mouse_scroll.y = dy;
    SET_BIT(input_state,PC_EVENT_SCROLL);
}
void Input::OnText(text_char* text){
    input_text = TextString::copy(text);
    SET_BIT(input_state,PC_EVENT_TEXT);
}
void Input::OnCharacter(int code_point){
    character_code[0]=code_point;
    input_text = character_code;
    SET_BIT(input_state,PC_EVENT_TEXT);
}

//////////////////////
//    Event flag management
////////////////////
Input::Event Input::NextEvent(){
    for(int i=0; i<EVENT_COUNT;i++){
        if(GET_BIT(input_state,i)){return (Event)(i+1);}
    }
    return Event::None;
}
Input::Event Input::NextEvent(Input::Event start){
    if(start < 0 || (start + 1) >= EVENT_COUNT)return Event::None;
    for(int i=start; i<EVENT_COUNT;i++){
        if(GET_BIT(input_state,(i-1))){return (Event)(i+1);}
    }
    return Event::None;
}
void Input::ClearEvent(Input::Event event){
    if(event <= 0 || event >= EVENT_COUNT)return;
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
text_char*          Controller::GetTextInput(){return input_text;}

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
void Input::AddKeyAxisBind(AxisID axis, int key_id, vec2 direction){
    Key_Axis_Bind* newbind = key_axis_binds.Add();
    newbind->axis=axis;
    newbind->direction=direction;
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

//////////////////////
//    Utility
////////////////////
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
    x += tilt.x;
    y += tilt.y;
    dx = tilt.x;
    dy = tilt.y;
    if(x*x + y*y < AXIS_CUTOFF){x=0;y=0;}//deadzone
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


//////////////////////
//    Loading
////////////////////
void Input::LoadKeyLayout(char* layout_filename){
    FileReader layout_file(layout_filename);

    for(byte* line=layout_file.ReadLine();line != null;line=layout_file.ReadLine()){
        if(line[0]=='[')continue;
        char* key = cstr::substr_before((char*)line,'=');
        char* value = cstr::substr_after((char*)line,'=');
        if(key!=nullptr&&value!=nullptr){
            int key_id = cstr::integer_from_string(value);
            if(key_id > 0){
                physical_input_map.Add(key_id,(byte*)cstr::new_copy(key));
            }
        }
        if(key)free(key);
        if(value)free(value);
    }
}

void Input::LoadDefaultKeyBindings(){
    char* keybinds[] = { 
        "w=move.up", "a=move.left", "s=move.down", "d=move.right",
        "n=button_a", "m=button_b", "j=button_c", "k=button_d",
        "q=l1", "e=r1",
        "enter=pause",
        "tab=menu",
        "joy_lstick=move",
        "joy_lstick=down",
        "joy_10=move.up", "joy_11=move.down", "joy_12=move.left", "joy_13=move.right",
        "joy_2=button_a", "joy_3=button_b", "joy_1=button_c", "joy_4=button_d",
        "joy_5=l1", "joy_6=r1",
        "joy_9=menu",
        "joy_10=pause"
    };
    ConfigMap* map = new ConfigMap();
    map->LoadFromTextArray(keybinds,26);
    LoadKeyBindings(map);
}

void Input::LoadKeyBindings(ConfigMap* bindings){
    
    for(StringPair pair: *bindings){
        int physical_input_id = GetKeyID(pair.key);
        
        for(int i=0;i<BUTTON_COUNT;i++){
            if(cstr::compare(button_names[i],pair.value)){
                AddKeyButtonBind((ButtonID)i,physical_input_id);
            }
        }
        for(int i=0;i<AXIS_COUNT;i++){
            if(cstr::compare(axis_names[i],pair.value)){
                AddAxisBind((AxisID)i,physical_input_id);
            }
            else if(cstr::starts_with(pair.value,axis_names[i])){
                char* direction_name = cstr::substr_after(pair.value,'.');
                int direction_id=-1;
                for(int j=0;j<4;j++){
                    if(cstr::compare(direction_name,axis_directions[j])){
                        direction_id = j;
                    }
                }

                vec2 dir = {0,0};
                switch (direction_id)
                {
                    case 0:dir ={0,1};break;
                    case 1:dir ={0,-1};break;
                    case 2:dir ={-1,0};break;
                    case 3:dir ={1,0};break;
                    default:break;
                }
                AddKeyAxisBind((AxisID)i,physical_input_id,dir);
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
