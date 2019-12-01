#ifndef FILE_H
#define FILE_H

#include <fstream>
#include <stdio.h>

typedef unsigned int uint32;
typedef unsigned char byte;

class File{
	FILE* file_handle;
    public:
	long amount_read;
	long length;
	bool error;
	File(const char* filename);
	void read(void* dest,int bytes);
	void close();
};

byte* getFile(const char* filename);



#endif
