#ifndef THREADS_H
#define THREADS_H

#include "os.h"
#include "struct/data_types.h"


struct Mutex{
    void* obj_ptr;
    void* os_mtx_ptr;
    Mutex(void* object){obj_ptr=object;os_mtx_ptr=nullptr;}
    inline bool GetSync(int timeout){
        os_mtx_ptr = OS::GetMutexLock(obj_ptr,timeout);
        return os_mtx_ptr != nullptr;
    }
    inline void ReleaseSync(){
        
        if(os_mtx_ptr != nullptr){
            OS::ReleaseMutexLock(os_mtx_ptr);
            os_mtx_ptr=nullptr;}
        }
    ~Mutex(){ReleaseSync();}//just to keep mutexes from dangling.
    
};

//Thread-safe buffer that allows concurrent write/read, but will guarantee data blocks can only be in use by one thread at a time.


class SynchronousBuffer{

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