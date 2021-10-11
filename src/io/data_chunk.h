#ifndef DATA_CHUNK_H
#define DATA_CHUNK_H

#include "../struct/data_types.h"


struct DataChunk{
    int header;
    int size;
    byte* payload;
    int crc;

    DataChunk(byte* chunk_data, int chunk_len);

    void SetType(int header);
    void SetPayload(byte* payload,int len);
    bool VerifyCRC();
    byte* Bytes();
};






#endif