#ifndef DATA_STRUCTS_H
#define DATA_STRUCTS_H

#ifndef null
#define null nullptr
#endif

struct BitArray{
    int bits;
    unsigned char* data;

    BitArray();
    BitArray(int bit_count);
    ~BitArray();
    void resize(int new_bit_count);

    void print();
    int set_count();
    int unset_count();
    bool get(int index);
    void set(int index);
    void unset(int index);
    bool toggle(int index);//returns new value;
};

struct DataBlock{
    int slots;
    int slot_size;
    BitArray slot_is_filled;
    void* data;

    DataBlock(int slot_count,int slot_size);

    int count();
    int add(void* object);
    void* get(int index);
    void remove(int index);
    void resize(int new_count);
};

template <typename T>
struct Array{
    DataBlock data;
    int running_count;
    
    Array();

    int count();
    int add(T* object);
    T* get(int index);
    T* remove(int index);
};


int integer_from_string(char* str,int index);
char* write_integer_string(int a, char *str, int index);

float float_from_string(char* str,int index);
char* write_float_string(int a, char *str, int index);

int bool_from_string(char* str,int index);
char* write_bool_string(int a, char *str, int index);

#endif