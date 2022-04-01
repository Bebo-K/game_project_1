#include <game_project_1/game/unit.hpp>
#include <string>


void color::from_bytes(byte R, byte G, byte B, byte A){
    r=R;b=B;g=G;a=A;
}
void color::from_int(int color_code){
    r = (color_code>>24) | 255;
    g = (color_code>>16) | 255;
    b = (color_code>>8) | 255;
    a = (color_code) | 255;
}

int color::to_int(){
    int color_code=0;
    color_code |= r;
    color_code = color_code << 8;
    color_code |= g;
    color_code = color_code << 8;
    color_code |= b;
    color_code = color_code << 8;
    color_code |= a;

    return color_code;
}

GameUnit::GameUnit(){
    entity_id=0;
    name=nullptr;
    race_id=0;
    class_id=0;
}
GameUnit::~GameUnit(){
    if(name != nullptr){free(name);name=nullptr;}
}
UnitAppearance::UnitAppearance(){
    colors=nullptr;color_count=0;
    styles=nullptr;style_options=0;
}
UnitAppearance::~UnitAppearance(){
    if(colors != nullptr){free(colors);colors=nullptr;}
    if(styles != nullptr){free(styles);styles=nullptr;}
}
void UnitAppearance::operator=(UnitAppearance& b){
    color_count = b.color_count;
    style_options = b.style_options;
    colors =(color*)malloc(sizeof(color)*color_count);
    styles =(int*)malloc(sizeof(int)*style_options);
}