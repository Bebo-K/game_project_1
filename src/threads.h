#ifndef THREADS_H
#define THREADS_H

#include "os.h"
#include "struct/data_types.h"


struct Mutex{
    void* obj_ptr;
    void* os_mtx_ptr;
    Mutex(void* object){obj_ptr=obj_ptr;os_mtx_ptr=nullptr;}
    inline bool getSync(int timeout){
        os_mtx_ptr = get_mutex_lock(obj_ptr,timeout);
        return os_mtx_ptr != nullptr;
    }
    inline void releaseSync(){
        
        if(os_mtx_ptr != nullptr){
            release_mutex_lock(os_mtx_ptr);
            os_mtx_ptr=nullptr;}
        }
    ~Mutex(){releaseSync();}//just to keep mutexes from dangling.
    
};

//Thread-safe buffer that allows concurrent write/read, but will guarantee data blocks can only be in use by one thread at a time.


class SynchronousBuffer{
    const static int BLOCK_EMPTY=0,BLOCK_FULL=1,BLOCK_IN_USE=2;

    int block_length;
    int blocks;
    byte* data;
    byte* occupancy;

    int Get(bool empty);
    void Resize(int block_count);

    public:
    SynchronousBuffer(int block_size,int initial_block_count);
    ~SynchronousBuffer();
    
    void Write(byte* block);
    bool Read(byte* dest);
};


#endif