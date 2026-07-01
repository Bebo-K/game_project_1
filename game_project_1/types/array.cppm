export module Array;

import Memory;
import Logger;
import Primitives;
import Iterator;
import <string>;
import <stdlib>;


export template <typename T>
class Array;

template <typename T>
struct ArrayIterator{
    Array<T>*    parent;
    int         index;
    T* operator*();
    ArrayIterator<T> operator++(){
        index++;
        return (*this);
    }
    bool operator==(ArrayIterator<T>& l2){
        return index ==l2.index;
    }
    bool operator!=(ArrayIterator<T>& l2){
        return !(index ==l2.index);
    }
};

//Wrapper class for {T* array;int arraySize;}. Safe for objects with no trivial copy-assignment.
export template <typename T>
class Array{
    friend struct ArrayIterator<T>;
    int length;
    T* data;

    Array(){
        data=nullptr;
        length=0;
    }

    Array(int size){
        data=nullptr;
        length=0;
        Init(size);
    }

    ~Array(){Destroy();}

    void Init(int size){
        if(length > 0){
            logger::exception("Array::Init -> Memory leak condition, array has already been allocated.");
        }
        if(size == 0){
            data=nullptr;
            length=0;
        }
        else{
            data= new T[size];
            length=size;
        }
    }

    void Destroy(){
        delete[] data;
        length=0;
        data=nullptr;
    };

    T* operator[](int index){
        if(index < 0 || index >= length){logger::exception("Array::[] -> Index %d is out of range.\n",index);return nullptr;}
        return &data[index];
    }

    int IndexOf(T* obj){
        int index = obj-data;
        if(index < 0 || index >= length){logger::exception("Array::Index of -> Index %d is out of range.",index);return 0;}
        return index;
    }

    ArrayIterator<T> begin(){ return {this,0};}
    ArrayIterator<T> end(){return {this,length};}
};


export template <typename T>
T* ArrayIterator<T>::operator*(){
    return (*parent)[index];
}


//A resizable array of individually settable bits.
export struct BitArray{
    int bits;
    int allocated;
    byte* data;

    BitArray():bits(0),allocated(0),data(nullptr){}
    BitArray(int bit_count){
        Initialize(bit_count);
    }
    ~BitArray(){
        free(data);
    }
    void Resize(int new_bit_count){
        if(new_bit_count ==0){
            free(data);data=null;
            bits=0;
            return;
        }
        int old_char_count = bits/8 + (bits % 8 > 0);
        int new_char_count = new_bit_count/8 + (new_bit_count % 8 > 0);
        int new_int_count = new_char_count/4 + ((new_char_count % 4) > 0);
        if(allocated < (int)(new_int_count*sizeof(int))){
            allocated = (int)(new_int_count*sizeof(int));
            unsigned char* new_data = (unsigned char*)calloc(new_char_count,1);
            memcpy(new_data,data,old_char_count);

            free(data);
            data = new_data;
        }
        bits = new_bit_count;
    }
    void Initialize(int bit_count){
        bits = bit_count;
        int char_count = bit_count/8 + ((bit_count % 8) > 0);
        int int_count = char_count/4 + ((char_count % 4) > 0);
        allocated = sizeof(int)*int_count;//try to fit on int boundary
        data = (unsigned char*)calloc(allocated,1);
    }

    void Print(){
        for(int bit=0;bit<bits;bit++){
            int byte = bit/8;
            int bit_indx = bit%8;
            logger::info("%s:%c ",bit, ((data[byte] & (1 << bit_indx)) != 0)? 'T':'F');
        }
    }
    int CountBitsSet(){
        int count = 0;
        for(int bit=0;bit<bits;bit++){
            int byte = bit/8;
            int bit_indx = bit%8;
            if((data[byte] & (1 << bit_indx)) != 0)count++;
        }
        return count;
    }
    int CountBitsUnset(){
        int count = 0;
        for(int bit=0;bit<bits;bit++){
            int byte = bit/8;
            int bit_indx = bit%8;
            if((data[byte] & (1 << bit_indx)) == 0)count++;
        }
        return count;
    }
    bool Get(int index){
        if(index < 0 || index >= bits){logger::exception("BitArray::is_set -> Index is out of range: %d",index);}
        return ((data[index/8] >> (index%8)) & 1) > 0;
    }
    void Set(int index){
        if(index < 0 || index >= bits){logger::exception("BitArray::set -> Index is out of range: %d",index);}
        data[index/8] = (data[index/8] | (1 << (index%8)));
    }
    void Unset(int index){
        if(index < 0 || index >= bits){logger::exception("BitArray::unset -> Index is out of range: %d",index);}
        byte dat = data[index/8];
        byte mask = ~(1 << (index%8));
        dat = dat & mask;
        data[index/8] = dat;
    }
    //returns the new value of the toggled bit
    bool Toggle(int index){
        if(index < 0 || index >= bits){logger::exception("BitArray::toggle -> Index is out of range: %d",index);}

        data[index/8] = (data[index/8] ^ (1 << (index%8)));
        return ((data[index/8] >> (index%8)) & 1) > 0;
    }
    void Clear(){
        int bytes = bits/8 + ((bits % 8) > 0);
        memset(data,0,bytes);
    }

    int GetSizeInBytes(){
        int bitflag_size_bytes = (bits/8);
        bitflag_size_bytes += (bitflag_size_bytes%8 > 0)?1:0;
        return bitflag_size_bytes;
    }
    //TODO- int can be different sizes, use a more specific name like GetSizeInDWords if this method is needed
    int GetSizeInInts(){
        int bitflag_size_ints = (bits/32);
        bitflag_size_ints += (bitflag_size_ints%32 > 0)?1:0;
        return bitflag_size_ints;
    }
};

//A resizeable array of unique integers. Order is not guaranteed as things may shift
export class IntegerSet{
    private:
    int     allocated;
    public:
    int     length;
    int*    data;

    IntegerSet():allocated(2),length(0){
        data=(int*)malloc(sizeof(int)*allocated);
    }
    IntegerSet(int size):allocated(size),length(size){
        data= (size==0)? null:(int*)malloc(sizeof(int)*allocated);
    }
    ~IntegerSet(){Clear();}

    void Add(int entry){
        if(Has(entry)){return;}
        if(length+1 > allocated){
            allocated= (allocated==0)? 2:allocated*2;
            int* new_data = (int*)malloc(sizeof(int)*allocated);
            for(int i=0;i<length;i++){new_data[i] = data[i];}
            free(data);
            data=new_data;
        }
        data[length]=entry;
        length++;
    }
    bool Has(int entry){
        for(int i=0;i<length;i++){if(data[i]==entry){return true;}}
        return false;
    }
    void Remove(int entry){
        if(!Has(entry)){return;}
        int index =0;
        for(int i=0;i<length;i++){if(data[i]==entry){index=i;}}
        if(index != length-1){data[index]=data[length-1];}
        length--;
    }
    void Clear(){
        Deallocate(&data);
        length=0;
        allocated=0;
    }

    int operator[](int index) const {
        return data[index];
    }
    //int* Harden();
};

//A resizeable array of data blocks and corresponding BitArray of block occupancy.
//Memory blocks can have any data.
//Allocated memory is doubled by default once the array runs out of slots.
//Add(),Allocate() and Remove() must be used to add/remove elements to maintain the vacancy bit array.
//Though Get(),GetArray() and the subscript operator([]) all are ways to access data, they handle vacant object slots differently
export class DynamicArray;
typedef Iterator<DynamicArray,byte*> DynamicArrayIterator;

class DynamicArray{
    private:
    BitArray occupancy;
    byte* data;

    friend DynamicArrayIterator;
    public:
    int slots;
    int slot_size;

    //DynamicArray();
    DynamicArray(int block_size):occupancy(1){
        slots=1;
        slot_size=block_size;
        data = (byte*)calloc(slots,slot_size);
    }
    DynamicArray(int count,int block_size):occupancy(count){
        slots=count;
        slot_size=block_size;
        data = (byte*)calloc(slots,slot_size);
    }
    ~DynamicArray(){
        Free(data);
        //occupancy array is auto-deallocated
    }

    //returns empty space for one object. Example usage: 'Object* o = new (DynamicArray.Allocate()) Object()'
    void* Allocate(){
        int slot_to_add=0;
        while(slot_to_add < slots && occupancy.Get(slot_to_add)){slot_to_add++;}
        if(slot_to_add == slots){Resize(slots*2);}
        occupancy.Set(slot_to_add);
        return data + (slot_to_add*slot_size);
    }
    //copies existing data into a new slot.
    int Add(void* object){
        int slot_to_add=0;
        while(slot_to_add < slots && occupancy.Get(slot_to_add)){slot_to_add++;}
        if(slot_to_add == slots){Resize(slots*2);}
        occupancy.Set(slot_to_add);
        memcpy(data + (slot_to_add*slot_size),object,slot_size);
        return slot_to_add;
    }
    //deallocates memory for one slot. This does not free any objects allocated with the data.
    void Remove(int index){
        if(index < 0 || index >= slots){logger::exception("DynamicArray::Remove -> Index %d is out of range.",index);}
        memset(&data[index*slot_size],0,slot_size);
        occupancy.Unset(index);
    }
    //If data is a pointer inside this memory block, returns it's index in the array.
    int  Index(byte* data){
        if(obj < data || obj >= data+(slot_size*slots)){logger::exception("DynamicArray::Index -> Memory address 0x%08x is out of range.",obj);}
        return (obj-data)/slot_size;
    }
    void* Get(int index){
        if(occupancy.Get(index)){return &data[index*slot_size];}else return null;
    }
    int   NextNonEmpty(int index){
        int next=start_index;
        while( ++next < slots && !occupancy.Get(next));
        return next;
    }

    int Count(){return occupancy.CountBitsSet();}
    void Clear(){
        memset(data,0,slot_size*slots);
        occupancy.Clear();
    }
    void Resize(int new_count){
        occupancy.Resize(new_count);
        byte* new_data = (byte*)calloc(new_count,slot_size);
        int slots_to_copy = (slots < new_count)?slots:new_count;
        memcpy(new_data,data,slots_to_copy*slot_size);
        slots = new_count;
        free(data);
        data = new_data;
    }
    DynamicArrayIterator begin(){return {this,NextNonEmpty(-1)};}
    DynamicArrayIterator end(){return {this,slots};}
};

template<>
byte* DynamicArrayIterator::GetResult(){
    return (byte*)parent->Get(index);
}

template<>
int DynamicArrayIterator::Next(){
    return parent->NextNonEmpty(index);
}