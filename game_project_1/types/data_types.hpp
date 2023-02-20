#ifndef DATA_STRUCTS_H
#define DATA_STRUCTS_H

#ifndef null
#define null nullptr
#endif

#define SET_BIT(val,place)   ((val) |=  (1<<(place)))
#define CLEAR_BIT(val,place) ((val) &= ~(1<<(place)))
#define TOGGLE_BIT(val,place)  ((val) ^=  (1<<(place)))
inline bool GET_BIT(int val,char place){return (val & (1 << place)) != 0;}

namespace Random{
    //TODO: start here
    int Int(int min, int max);
    float Float();
    float Float(float min, float max);
};

typedef unsigned int uint32;
typedef unsigned char byte;

struct bitmask{
    static int bit(int place);
    int val;
    //TODO: start  here
    bitmask(int a);
    static bitmask of_bits(int a);
    static bitmask of_bits(int a, int b);
    static bitmask of_bits(int a, int b, int c);
    static bitmask of_bits(int[] a,int l);
    inline void set(int place);
    inline void clear(int place);
    inline void toggle_bit(int place);
    inline bool get_bit(int place);
};



union value
{
    int i;
    float f;
    wchar_t* wstr;
};

struct color{
    byte r,g,b,a;
    void rgba(byte R, byte G, byte B, byte A);
    void rgba(int color_code);
    int as_code();
};

struct color_f{
    float r,g,b,a;
    void rgba(float R,float G,float B,float A);
    void from_color(color c);
    color to_color();
}








#endif

