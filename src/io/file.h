#ifndef FILE_H
#define FILE_H

#include <fstream>
#include <stdio.h>

typedef unsigned int uint32;
typedef unsigned char byte;

class File{
	FILE* file_handle;
    public:
	const char* path;
	long amount_read;
	long length;
	bool error;
	File();
	File(const char* filename);
	void read(void* dest,int bytes);
	void peek(void* dest,int bytes);
	void close();
};

class FileBuffer{

	public:
	File  source;
	byte* contents;

	FileBuffer(const char* filename);
	int Length();
	~FileBuffer();
};


byte* GetFile(const char* filename);
byte* GetRelativeFile(const char* filename,const char* reference_file);



#endif
