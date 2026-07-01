export module Threads;

import Memory;
import OS;
import Primitives;
import Logger;

constexpr byte BLOCK_EMPTY=0,BLOCK_FULL=1,BLOCK_IN_USE=2;
constexpr int GET_NONE_AVAILABLE=-1,GET_BUFFER_CLOSED=-2;

//Pointer keyed lock across threads.
export struct Mutex{
    void* obj_ptr;
    void* os_mtx_ptr;
    Mutex(void* object){obj_ptr=object;os_mtx_ptr=nullptr;}
    bool GetSync(int timeout){
        os_mtx_ptr = OS::GetMutexLock(obj_ptr,timeout);
        return os_mtx_ptr != nullptr;
    }
    void ReleaseSync(){
        
        if(os_mtx_ptr != nullptr){
            OS::ReleaseMutexLock(os_mtx_ptr);
            os_mtx_ptr=nullptr;}
    }
    ~Mutex(){ReleaseSync();}//just to keep mutexes from dangling.
    
};

//Thread-safe buffer that allows concurrent write/read of different blocks, but will ensure data blocks are only in use by one thread at a time.
export class SynchronousBuffer{
private:
    int block_length;
    int blocks;
    byte* data;
    byte* occupancy;

    int Get(const byte state){
        if (blocks==0)return GET_BUFFER_CLOSED;
        int index=GET_NONE_AVAILABLE;
        Mutex buffer_mutex(this);
        buffer_mutex.GetSync(1);

        for(int i=0;i<blocks;i++){
            if(occupancy[i] == state){index=i;break;}
        }

        if(index >= 0){occupancy[index]=BLOCK_IN_USE;}
        else if(index < 0 && state == BLOCK_EMPTY){
            //Resize (double the buffer)
            //TODO: checks to keep memory footprint from getting away from us
            int new_block_count = blocks*2;
            byte* new_data = ZeroAlloc<byte>(new_block_count*block_length);
            byte* new_occupancy = ZeroAlloc<byte>(new_block_count);
            int blocks_to_copy = (blocks < new_block_count)?blocks:new_block_count;
            //Spin until other transfers have completed (We already have mutex so no new ones will start)
            for(int i=0;i<blocks;i++){if(occupancy[i]==BLOCK_IN_USE)i=0;}
            MemCopy(new_data,data,blocks_to_copy*block_length);
            MemCopy(new_occupancy,occupancy,blocks_to_copy);
            blocks = new_block_count;
            Free(data);
            Free(occupancy);
            data = new_data;
            occupancy = new_occupancy;
            //
            index = blocks;
            occupancy[index]=BLOCK_IN_USE;
        }
        buffer_mutex.ReleaseSync();
        return index;
    }

public:

    SynchronousBuffer(int block_size,int block_count){
        if(block_size <= 0 || block_count <= 0) {
            logger::exception("SynchronousBuffer.Init: Buffer size invalid: (%d x %d)",block_count,block_size);
        }
        occupancy = ZeroAlloc<byte>(block_count);
        data = ZeroAlloc<byte>(block_count*block_size);
        block_length = block_size;
        blocks = block_count;
    }

    ~SynchronousBuffer(){
        Mutex buffer_mutex(this);
        buffer_mutex.GetSync(1);
        blocks=0;
        Deallocate(&occupancy);
        Deallocate(&data);
        buffer_mutex.ReleaseSync();
    }

    bool Read(byte* dest){
        int block_index = Get(BLOCK_FULL);
        if(block_index < 0){return false;}
        MemCopy(dest,&data[block_index*block_length],block_length);
        occupancy[block_index]=BLOCK_EMPTY;
        return true;
    }

    void Write(byte* block){
        int block_index = Get(BLOCK_EMPTY);
        if(block_index == GET_NONE_AVAILABLE){logger::exception("SynchronousBuffer.Write: Buffer overflow");}
        if(block_index == GET_BUFFER_CLOSED){/*fail silently*/}
        MemCopy(&data[block_index*block_length],block,block_length);
        occupancy[block_index]=BLOCK_FULL;
    }

};

