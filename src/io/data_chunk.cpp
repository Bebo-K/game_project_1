#include "data_chunk.h"
#include "../log.h"
#include <string>

#define CHUNK_ENVELOPE_SIZE 12
#define PAYLOAD_START 8 

unsigned long CRC_TABLE[256];
bool CRC_TABLE_AVAILABLE = false;

void SetupCRCTable(){
     unsigned long c;
     for (int n = 0; n < 256; n++) {
       c = (unsigned long) n;
       for (int k = 0; k < 8; k++) {
         c = (c & 1) ? (0xedb88320L ^ (c >> 1)) : (c >> 1);
       }
       CRC_TABLE[n] = c;
     }
     CRC_TABLE_AVAILABLE = true;
}

int DoCRC(byte* data, int len){
    if(!CRC_TABLE_AVAILABLE){SetupCRCTable();}
	unsigned int crc32 = 0xFFFFFFFFu;
	
	for (int i = 0; i < len; i++) {
		const unsigned int index = (crc32 ^ data[i]) & 0xff;
		crc32 = (crc32 >> 8) ^ CRC_TABLE[index]; 
	}
	crc32 ^= 0xFFFFFFFFu;
	return crc32;
}


DataChunk::DataChunk(byte* chunk_data, int chunk_len){
    header =*(int*)&chunk_data[0];
    size = *(int*)&chunk_data[4];
    crc = *(int*)&chunk_data[chunk_len-4];

    int actual_size = chunk_len - CHUNK_ENVELOPE_SIZE;
    if(size !=actual_size){
        logger::exception("DataChunk.ctor: data is not a valid chunk. Reported size: %d, actual:%d\n",size,actual_size);
        header = -1;
        payload = nullptr;
        size=0;
        crc =0;
    }

    int actual_crc = DoCRC(&chunk_data[PAYLOAD_START],size);
    if(crc != actual_crc){
        logger::warn("WARNING: CRCs did not match for ChunkID %d. Check for corrupted or tampered data!\n",header);
    }

    payload = (byte*)calloc(size,1);
    memcpy(payload,&chunk_data[PAYLOAD_START],size);
}

void DataChunk::SetType(int header_id){header = header_id;}
void DataChunk::SetPayload(byte* data,int len){
    size = len;
    payload = (byte*)calloc(size,1);
    memcpy(payload,data,size);
    crc = DoCRC(payload,size);
}
bool DataChunk::VerifyCRC(){
    int actual_crc = DoCRC(payload,size);
    return (crc == actual_crc);
}
byte* DataChunk::Bytes(){
    byte* ret =  (byte*)calloc(CHUNK_ENVELOPE_SIZE+size,1);
    
    int* head_ptr = (int*)&ret[0];
    *head_ptr =  header;
    int* size_ptr = (int*)&ret[4];
    *size_ptr =  header;
    memcpy(&ret[PAYLOAD_START],payload,size);
    int* crc_ptr = (int*)&ret[PAYLOAD_START+size];
    *crc_ptr = DoCRC(&ret[PAYLOAD_START],size);
    return ret;
}