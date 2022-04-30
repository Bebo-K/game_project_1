#include <game_project_1/types/data_types.hpp>


void* i_to_p(int ptr){
    byte* ret = null;
    ret += ptr;
    return ret;
}

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