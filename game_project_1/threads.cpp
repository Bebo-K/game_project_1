#include <game_project_1/threads.hpp>
#include <stdlib.h>
#include <string.h>

const int BLOCK_EMPTY=0,BLOCK_FULL=1,BLOCK_IN_USE=2;

SynchronousBuffer::SynchronousBuffer(int block_size,int block_count){
    if(block_size <= 0 || block_count <= 0)return;
    occupancy = (byte*)calloc(block_count,1);
    data = (byte*)calloc(block_count,block_size);
    block_length = block_size;
    blocks = block_count;
}

SynchronousBuffer::~SynchronousBuffer(){
    blocks=0;
    free(occupancy);occupancy=nullptr;
    free(data);data=nullptr;
}

bool SynchronousBuffer::Read(byte* dest){
    int block_index = Get(false);
    if(block_index < 0){return false;}
    memcpy(dest,&data[block_index*block_length],block_length);
    occupancy[block_index]=BLOCK_EMPTY;
    return true;
}

void SynchronousBuffer::Write(byte* block){
    int block_index = Get(true);
    memcpy(&data[block_index*block_length],block,block_length);
    occupancy[block_index]=BLOCK_FULL;
}

int SynchronousBuffer::Get(bool empty){
    int index=-1;
    Mutex buffer_mutex(this);
    buffer_mutex.GetSync(1);

    for(int i=0;i<blocks;i++){
        if(occupancy[i] == ((empty)?BLOCK_EMPTY:BLOCK_FULL)){index=i;break;}
    }

    if(index >= 0){occupancy[index]=BLOCK_IN_USE;}
    else if(index < 0 && empty){
        index = blocks;
        Resize(blocks*2);
        occupancy[index]=BLOCK_IN_USE;
    }
    buffer_mutex.ReleaseSync();
    return index;
}

void SynchronousBuffer::Resize(int block_count){
    byte* new_data = (byte*)calloc(block_count,block_length);
    byte* new_occupancy = (byte*)calloc(block_count,1);
    int blocks_to_copy = (blocks < block_count)?blocks:block_count;

    for(int i=0;i<blocks;i++){if(occupancy[i]==BLOCK_IN_USE)i=0;}//spin lock until existing transfers are complete (No new ones will be started as long as parent Get() has mutex)
    memcpy(new_data,data,blocks_to_copy*block_length);
    memcpy(new_occupancy,occupancy,blocks_to_copy);
    blocks = block_count;
    free(data);
    free(occupancy);
    data = new_data;
    occupancy = new_occupancy;
}