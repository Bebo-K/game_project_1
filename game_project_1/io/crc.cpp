#include <game_project_1/io/crc.hpp>


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

int CRC(byte* data, int len){
    if(!CRC_TABLE_AVAILABLE){SetupCRCTable();}
	unsigned int crc32 = 0xFFFFFFFFu;
	
	for (int i = 0; i < len; i++) {
		const unsigned int index = (crc32 ^ data[i]) & 0xff;
		crc32 = (crc32 >> 8) ^ CRC_TABLE[index]; 
	}
	crc32 ^= 0xFFFFFFFFu;
	return crc32;
}