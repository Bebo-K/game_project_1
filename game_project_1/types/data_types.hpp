#ifndef DATA_STRUCTS_H
#define DATA_STRUCTS_H

#ifndef null
#define null nullptr
#endif

#define SET_BIT(val,place)   ((val) |=  (1<<(place)))
#define CLEAR_BIT(val,place) ((val) &= ~(1<<(place)))
#define TOGGLE_BIT(val,place)  ((val) ^=  (1<<(place)))
inline bool GET_BIT(int val,char place){return (val & (1 << place)) != 0;}

typedef unsigned int uint32;
typedef unsigned char byte;

void* i_to_p(int ptr);//Convention exception to match "atof",etc

struct color{
    byte r,g,b,a;
    void from_bytes(byte R, byte G, byte B, byte A);
    void from_int(int color_code);
    int to_int();
};

#endif
