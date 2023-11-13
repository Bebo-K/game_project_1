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
    int Int(int min, int max);
    float Float();
    float Float(float min, float max);
};

typedef unsigned int uint32;
typedef unsigned char byte;

struct bitmask{
    static int bit(int place);
    const static int all= -1;
    const static int none = 0;
    int val;
    bitmask(int a);
    static bitmask of_bits(int a);
    static bitmask of_bits(int a, int b);
    static bitmask of_bits(int a, int b, int c);
    static bitmask of_bits(int a[],int l);
    void and_with(bitmask& b2);
    void or_with(bitmask& b2);
    static bitmask invert(bitmask& b2);
    void set(int place);
    void clear(int place);
    void clearAll();
    void toggle_bit(int place);
    bool get_bit(int place);

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
    color_f mult(color_f c2);
};








#endif

