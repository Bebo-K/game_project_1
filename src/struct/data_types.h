#ifndef DATA_STRUCTS_H
#define DATA_STRUCTS_H

#ifndef null
#define null nullptr
#endif

#define SET_BIT(x,bit) x |= (1<<(bit))
#define CLEAR_BIT(x, bit) x &= ~(1<<(bit))

typedef unsigned int uint32;
typedef unsigned char byte;

inline void* i_to_p(int ptr){
    byte* ret = null;
    ret += ptr;
    return ret;
}

#endif

