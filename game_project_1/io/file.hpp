#ifndef FILE_H
#define FILE_H

//Workaround for debug compile, hopefully it doesn't break things
//#define RC_INVOKED

#include <fstream>
#include <stdio.h>

const char OS_PATH_SEPERATOR =
#ifdef _WIN32
		'\\';
#else
		'/';
#endif

typedef unsigned int uint32;
typedef unsigned char byte;

//Simple input file for reading. Only uses char* filenames, so it should only be used for small relative paths.
class IFile{
	FILE* file_handle;
    public:
	const char* path;
	long amount_read;
	long length;
	bool error;
	IFile();
	IFile(const char* filename);
	int read(void* dest,int bytes);
	int peek(void* dest,int bytes);
	void close();
	static char* GetPathOf(const char* filename);
	static bool Exists(const char* filename);
};



//File type for reading/writing files outside of game directory.
//Uses wide char filepaths to function properly with user's FS structure.
class UserFile{
	FILE* file_handle;
	public:
	const wchar_t* path;
	long amount_read;
	long amount_written;
	long length;
	bool error;
	bool read_mode;

	UserFile(const wchar_t* filename,char mode);
	static bool Exists(const wchar_t* filename);
	void write(byte* data,int bytes);
	void read(void* dest,int bytes);
	void peek(void* dest,int bytes);
	void close();
};


//Quick and dirty file line reader for parsing config files
class FileReader{
	public:
	int mark;
	int length;
	byte* contents;
	byte* last_line;
	
	FileReader(const char* filename);
	byte* ReadLine();
	byte  ReadByte();
	~FileReader();
};

//In-memory data from file.
/*
class FileBuffer{
	public:
	FILE* file_handle;
	byte* contents;

	FileBuffer(const char* filename);
	int Length();
	~FileBuffer();
};
*/

//Line-by-line reader for a file.



//byte* GetFile(const char* filename);
//byte* GetRelativeFile(const char* filename,const char* reference_file);



#endif
