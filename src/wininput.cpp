#include "input.h"
#include <windows.h>
#include "config.h"

///Hahaha, why yes, this IS a bad idea!
const int VK_COUNT=0xFF;
const char* MS_VK_NAMES [] ={
/*0x00*/ "ERROR",
/*0x01*/ "[?]",
/*0x02*/ "[?]",
/*0x03*/ "[?]",	
/*0x04*/ "[?]",
/*0x05*/ "[?]",
/*0x06*/ "[?]",
/*0x07*/ "[?]",
/*0x08*/ "backspace",
/*0x09*/ "tab",
/*0x0A*/ "[?]",
/*0x0B*/ "[?]",
/*0x0C*/ "[?]",
/*0x0D*/ "enter",
/*0x0E*/ "scroll",	
/*0x0F*/ "[?]",
/*0x10*/ "shift",
/*0x11*/ "ctrl",
/*0x12*/ "alt",
/*0x13*/ "pause",
/*0x14*/ "capslock",
/*0x15*/ "[?]",
/*0x16*/ "[?]",
/*0x17*/ "[?]",
/*0x18*/ "[?]",
/*0x19*/ "[?]",
/*0x1A*/ "[?]",
/*0x1B*/ "escape",
/*0x1C*/ "[?]",
/*0x1D*/ "[?]",
/*0x1E*/ "[?]",
/*0x1F*/ "[?]",
/*0x20*/ "space",
/*0x21*/ "page up",
/*0x22*/ "page down",
/*0x23*/ "end",
/*0x24*/ "home",
/*0x25*/ "left",
/*0x26*/ "up",
/*0x27*/ "right",
/*0x28*/ "down",
/*0x29*/ "[?]",
/*0x2A*/ "[?]",
/*0x2B*/ "[?]",
/*0x2C*/ "print screen",
/*0x2D*/ "insert",
/*0x2E*/ "delete",
/*0x2F*/ "",
/*0x30*/ "0",
/*0x31*/ "1",
/*0x32*/ "2",
/*0x33*/ "3",
/*0x34*/ "4",
/*0x35*/ "5",
/*0x36*/ "6",
/*0x37*/ "7",
/*0x38*/ "8",
/*0x39*/ "9",
/*0x3A*/ "[?]",
/*0x3B*/ "[?]",
/*0x3C*/ "[?]",
/*0x3D*/ "[?]",
/*0x3E*/ "[?]",
/*0x3F*/ "[?]",
/*0x40*/ "[?]",
/*0x41*/ "a",
/*0x42*/ "b",
/*0x43*/ "c",
/*0x44*/ "d",
/*0x45*/ "e",
/*0x46*/ "f",
/*0x47*/ "g",
/*0x48*/ "h",
/*0x49*/ "i",
/*0x4A*/ "j",
/*0x4B*/ "k",
/*0x4C*/ "l",
/*0x4D*/ "m",
/*0x4E*/ "n",
/*0x4F*/ "o",
/*0x50*/ "p",
/*0x51*/ "q",
/*0x52*/ "r",
/*0x53*/ "s",
/*0x54*/ "t",
/*0x55*/ "u",
/*0x56*/ "v",
/*0x57*/ "w",
/*0x58*/ "x",
/*0x59*/ "y",
/*0x5A*/ "z",
/*0x5B*/ "[?]",
/*0x5C*/ "[?]",
/*0x5D*/ "[?]",
/*0x5E*/ "[?]",
/*0x5F*/ "[?]",
/*0x60*/ "keypad0",
/*0x61*/ "keypad1",
/*0x62*/ "keypad2",
/*0x63*/ "keypad3",
/*0x64*/ "keypad4",
/*0x65*/ "keypad5",
/*0x66*/ "keypad6",
/*0x67*/ "keypad7",
/*0x68*/ "keypad8",
/*0x69*/ "keypad9",
/*0x6A*/ "keypad*",
/*0x6B*/ "keypad+",
/*0x6C*/ "separator",
/*0x6D*/ "keypad-",
/*0x6E*/ "keypad.",
/*0x6F*/ "keypad/",
/*0x70*/ "f1",
/*0x71*/ "f2",
/*0x72*/ "f3",
/*0x73*/ "f4",
/*0x74*/ "f5",
/*0x75*/ "f6",
/*0x76*/ "f7",
/*0x77*/ "f8",
/*0x78*/ "f9",
/*0x79*/ "f10",
/*0x7A*/ "f11",
/*0x7B*/ "f12",
/*0x7C*/ "[?]",
/*0x7D*/ "[?]",
/*0x7E*/ "[?]",
/*0x7F*/ "[?]",
/*0x80*/ "[?]",
/*0x81*/ "[?]",
/*0x82*/ "[?]",
/*0x83*/ "[?]",
/*0x84*/ "[?]",
/*0x85*/ "[?]",
/*0x86*/ "[?]",
/*0x87*/ "[?]",
/*0x88*/ "[?]",
/*0x89*/ "[?]",
/*0x8A*/ "[?]",
/*0x8B*/ "[?]",
/*0x8C*/ "[?]",
/*0x8D*/ "[?]",
/*0x8E*/ "[?]",
/*0x8F*/ "[?]",
/*0x90*/ "num lock",
/*0x91*/ "scroll lock",
/*0x92*/ "[?]",
/*0x93*/ "[?]",
/*0x94*/ "[?]",
/*0x95*/ "[?]",
/*0x96*/ "[?]",
/*0x97*/ "[?]",
/*0x98*/ "[?]",
/*0x99*/ "[?]",
/*0x9A*/ "[?]",
/*0x9B*/ "[?]",
/*0x9C*/ "[?]",
/*0x9D*/ "[?]",
/*0x9E*/ "[?]",
/*0x9F*/ "[?]",
/*0xA0*/ "left shift",
/*0xA1*/ "right shift",
/*0xA2*/ "ctrl",
/*0xA3*/ "right ctrl",
/*0xA4*/ "[?]",
/*0xA5*/ "[?]",
/*0xA6*/ "[?]",
/*0xA7*/ "[?]",
/*0xA8*/ "[?]",
/*0xA9*/ "[?]",
/*0xAA*/ "[?]",
/*0xAB*/ "[?]",
/*0xAC*/ "[?]",
/*0xAD*/ "[?]",
/*0xAE*/ "[?]",
/*0xAF*/ "[?]",
/*0xB0*/ "[?]",
/*0xB1*/ "[?]",
/*0xB2*/ "[?]",
/*0xB3*/ "[?]",
/*0xB4*/ "[?]",
/*0xB5*/ "[?]",
/*0xB6*/ "[?]",
/*0xB7*/ "[?]",
/*0xB8*/ "[?]",
/*0xB9*/ "[?]",
/*0xBA*/ "[?]",
/*0xBB*/ "=",
/*0xBC*/ ",",
/*0xBD*/ "-",
/*0xBE*/ ".",
/*0xBF*/ "/",
/*0xC0*/ "tilde",
/*0xC1*/ "[?]",
/*0xC2*/ "[?]",
/*0xC3*/ "[?]",
/*0xC4*/ "[?]",
/*0xC5*/ "[?]",
/*0xC6*/ "[?]",
/*0xC7*/ "[?]",
/*0xC8*/ "[?]",
/*0xC9*/ "[?]",
/*0xCA*/ "[?]",
/*0xCB*/ "[?]",
/*0xCC*/ "[?]",
/*0xCD*/ "[?]",
/*0xCE*/ "[?]",
/*0xCF*/ "[?]",
/*0xD0*/ "[?]",
/*0xD1*/ "[?]",
/*0xD2*/ "[?]",
/*0xD3*/ "[?]",
/*0xD4*/ "[?]",
/*0xD5*/ "[?]",
/*0xD6*/ "[?]",
/*0xD7*/ "[?]",
/*0xD8*/ "[?]",
/*0xD9*/ "[?]",
/*0xDA*/ "[?]",
/*0xDB*/ "[",
/*0xDC*/ "\\",
/*0xDD*/ "]",
/*0xDE*/ "'",
/*0xDF*/ ";",
/*0xE0*/ "[?]",
/*0xE1*/ "[?]",
/*0xE2*/ "[?]",
/*0xE3*/ "[?]",
/*0xE4*/ "[?]",
/*0xE5*/ "[?]",
/*0xE6*/ "[?]",
/*0xE7*/ "[?]",
/*0xE8*/ "[?]",
/*0xE9*/ "[?]",
/*0xEA*/ "[?]",
/*0xEB*/ "[?]",
/*0xEC*/ "[?]",
/*0xED*/ "[?]",
/*0xEE*/ "[?]",
/*0xEF*/ "[?]",
/*0xF0*/ "[?]",
/*0xF1*/ "[?]",
/*0xF2*/ "[?]",
/*0xF3*/ "[?]",
/*0xF4*/ "[?]",
/*0xF5*/ "[?]",
/*0xF6*/ "[?]",
/*0xF7*/ "[?]",
/*0xF8*/ "[?]",
/*0xF9*/ "[?]",
/*0xFA*/ "[?]",
/*0xFB*/ "[?]",
/*0xFC*/ "[?]",
/*0xFD*/ "[?]",
/*0xFE*/ "[?]",
/*0xFF*/ "[?]"
};

int MOUSE_BUTTON_OFFSET=0x101;
int MOUSE_BUTTON_COUNT=3;
const char* MOUSE_BUTTON_NAMES[] ={
    "mouse_left",
    "mouse_right",
    "mouse_center"
};

int JOYSTICK_OFFSET=0x1A0;
int JOYSTICK_BUTTON_COUNT=15;
const char* JOYSTICK_BUTTON_NAMES[] ={
    "joy_a",
    "joy_b",
    "joy_x",
    "joy_y",
    "joy_l1",
    "joy_l2",
    "joy_r1",
    "joy_r2",
    "joy_start",
    "joy_lstick",
    "joy_rstick",
    "joy_dpad_up",
    "joy_dpad_down",
    "joy_dpad_left",
    "joy_dpad_right",
};

int Input::GetKeyID(char* key_name){
    if(cstr::starts_with(key_name,"mouse")){
        for(int i=0;i<MOUSE_BUTTON_COUNT;i++){
            if(cstr::compare(key_name,MOUSE_BUTTON_NAMES[i])){
                return MOUSE_BUTTON_OFFSET + i;
            }
        }
    }
    if(cstr::starts_with(key_name,"joy")){
        for(int i=0;i<JOYSTICK_BUTTON_COUNT;i++){
            if(cstr::compare(key_name,JOYSTICK_BUTTON_NAMES[i])){
                return JOYSTICK_OFFSET + i;
            }
        }
    }
    for(int i=0;i<VK_COUNT;i++){
        if(cstr::compare(key_name,MS_VK_NAMES[i])){
            return i;
        }
    }
    return -1;
}

const char* Input::GetKeyName(int key_id){
    if(key_id >= 0 && key_id <= VK_COUNT){return MS_VK_NAMES[key_id];}
    if(key_id >= MOUSE_BUTTON_OFFSET && 
    key_id <= MOUSE_BUTTON_OFFSET+MOUSE_BUTTON_COUNT){
        return MOUSE_BUTTON_NAMES[key_id-MOUSE_BUTTON_OFFSET];
    }
    if(key_id >= JOYSTICK_OFFSET && 
    key_id <= JOYSTICK_OFFSET+JOYSTICK_BUTTON_COUNT){
        return MOUSE_BUTTON_NAMES[key_id-MOUSE_BUTTON_OFFSET];
    }
    return nullptr;
}
    


