#include <game_project_1/types/data_types.hpp>
#include <cstdlib>

int Random::Int(int min, int max){return (rand() % (max-min)) + min;}
float Random::Float(){
    int seed = rand();
    double fseed = (double)seed;
    fseed /= (double)RAND_MAX;
    return (float)fseed;
}
float Random::Float(float min, float max){return (Random::Float() * (max-min)) + min;}



int bitmask::bit(int place){return (1<<place);}
bitmask::bitmask(int a){val =a;}
bitmask bitmask::of_bits(int a){return bitmask(1<<a);}
bitmask bitmask::of_bits(int a, int b){return bitmask((1<<a)|(1<<b));}
bitmask bitmask::of_bits(int a, int b, int c){return bitmask((1<<a)|(1<<b)|(1<<c));}
//...
bitmask bitmask::of_bits(int a[],int l){
    bitmask mask(0);
    for(int i=0;i<l;i++){mask.set(a[i]);}
    return mask;
}
void bitmask::set(int place){val |= bit(place);}
void bitmask::clear(int place){val &= ~bit(place);}
void bitmask::clearAll(){val=0;}
void bitmask::toggle_bit(int place){val ^= bit(place);}
bool bitmask::get_bit(int place){return (val & bit(place)) != 0;}
void bitmask::and_with(bitmask& b2){val &= b2.val;}
void bitmask::or_with(bitmask& b2){val |= b2.val;}
bitmask bitmask::invert(bitmask& b2){return {~b2.val};}

void color::rgba(byte R, byte G, byte B, byte A){
    r=R;b=B;g=G;a=A;
}
void color::rgba(int color_code){
    r = (color_code>>24) | 255;
    g = (color_code>>16) | 255;
    b = (color_code>>8) | 255;
    a = (color_code) | 255;
}
int color::as_code(){
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

color_f color_f::rgba(float R,float G,float B,float A){return {R,G,B,A};}
void color_f::from_color(color c){
    r = c.r/255.0f;
    g = c.g/255.0f;
    b = c.b/255.0f;
    a = c.a/255.0f;
}
color color_f::to_color(){
    return {(byte)(r*255.0f),(byte)(g*255.0f),(byte)(b*255.0f),(byte)(a*255.0f)};
}
