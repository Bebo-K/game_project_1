#ifndef FILE_H
#define FILE_H

#include <fstream>
#include <stdio.h>


class File{
	FILE* file_handle;
    public:
	int amount_read;
	void open(char* filename);
	void read(void* dest,int bytes);
	void close();
};



#endif
