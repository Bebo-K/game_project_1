export module Memory;

import <cstdlib>;
import <cstring>;

export typedef unsigned char byte;
export typedef unsigned int uint;
export typedef unsigned long long  uint64;

#ifndef null
export const void* null = nullptr;
#endif

/* TODO: deprecate all these macros
#define SET_BIT(val,place)   ((val) |=  (1<<(place)))
#define CLEAR_BIT(val,place) ((val) &= ~(1<<(place)))
#define TOGGLE_BIT(val,place)  ((val) ^=  (1<<(place)))
#define GET_BIT(val,place) ((val & (1 << place)) != 0)

#define DEALLOCATE(val) if(val!=nullptr){free(val);val=nullptr;}
#define SAFE_DELETE(val) if(val!=nullptr){delete val;val=nullptr;}
*/

//Wrapping standard memory operations in case we want to add checks, logging, profiling, custom memory management etc.
//TODO: Replace usages.
//Eventually: Set up allocation pools for memory (GRAPHICS, STRINGS, NETWORK, etc)


//Allocates memory for a T* (no guarantees for initial memory state). Remember that {size} is measured in units of sizeof(T)
export template<typename T>
T* Alloc(int size) {
    return static_cast<T *>(malloc(size*sizeof(T)));
}

//Allocates exactly {size} bytes of memory for a T*, if for some reason you need a
//non-sizeof(T)-divisible block, or already have the size in bytes
export template<typename T>
T* ByteAlloc(int size) {
    return static_cast<T *>(malloc(size*sizeof(T)));
}

//Allocates a T* with its memory pre-zeroed. Remember that {size} is measured in units of sizeof(T)
export template<typename T>
T* ZeroAlloc(int size) {
    return static_cast<T *>(calloc(size, sizeof(T)));
}

//Simply frees {*ptr}
export template<typename T>
void Free(T* ptr) {
    free(ptr);
}


//Frees {*ptr} and nulls it's value for memory safety.
export template<typename T>
void Deallocate(T** ptr) {
    if(*ptr) {
        free(*ptr);
        *ptr = nullptr;
    }
}

//Sets all bytes to {val} in pointer block {ptr}. Remember that {size} is measured in units of sizeof(T)
export template<typename T>
void MemSet(T* ptr,int size,byte val) {
    memset(ptr,val,size*sizeof(T));
}

//Zeroes out pointer block {ptr}. Remember that {size} is measured in units of sizeof(T)
export template<typename T>
void MemClear(T* ptr,int size) {
    memset(ptr,0,size*sizeof(T));
}

//Copies {size} bytes of memory from {from} to {to}
export void MemCopy(void* to, const void* from, int size) {
    memcpy(to,from,size);
}

namespace Bit {
    export template<typename T>
    void Set(T* value,byte bit){*value |= (1<<bit);}

    export template<typename T>
    void Clear(T* value,byte bit){*value &= ~(1<<bit);}

    export template<typename T>
    void Toggle(T* value,byte bit){*value ^= ~(1<<bit);}

    export template<typename T>
    bool Get (T value,byte bit){return (value & (1<<bit)) != 0;}
}


export struct bitmask{
    static int bit(int place){return (1<<place);}
    static constexpr int all= -1;
    static constexpr int none = 0;
    int val;

    bitmask(int a):val(a){}
    void and_with(bitmask& b2){val &= b2.val;}
    void or_with(bitmask& b2){val |= b2.val;}
    void set(int place){val |= bit(place);}
    void clear(int place){val &= ~bit(place);}
    void clearAll(){val=0;}
    void toggle_bit(int place){val ^= bit(place);}
    bool get_bit(int place){return (val & bit(place)) != 0;}

    static bitmask invert(bitmask& b2){return {~b2.val};}
    //Builders for setting specific bits by placement.
    static bitmask of_bits(int a){return bitmask(1<<a);}
    static bitmask of_bits(int a, int b){return bitmask((1<<a)|(1<<b));}
    static bitmask of_bits(int a, int b, int c){return bitmask((1<<a)|(1<<b)|(1<<c));}
    static bitmask of_bits(int a[],int l){
        bitmask mask(0);
        for(int i=0;i<l;i++) {
            if (a[i]<0 || a[i] >= sizeof(int)*8) {logger::exception("bitmask.of_bits: invalid bit position %s",a[i]);}
            mask.set(a[i]);
        }
        return mask;
    }
};
