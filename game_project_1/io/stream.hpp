#ifndef STREAM_H
#define STREAM_H

#include <game_project_1/io/file.hpp>

class Stream{
    public:
	char* uri;
	long amount_read;
	long length;
	bool error;

	Stream(char* uri_path);
	virtual ~Stream();
	virtual int read(void* dest,int bytes) =0;
	virtual int peek(void* dest,int bytes) =0;
	virtual byte* readAll() = 0;

	char* parentURI();
};

class FileStream: public Stream{
    public:
    IFile backing_file;
    FileStream(char* uri_path);
    ~FileStream();


	int  read(void* dest,int bytes);
	int peek(void* dest,int bytes);
	byte* readAll();
};

/*
class StreamReader{
	public:
	Stream* source;
	int mark;
	byte* last_line;
	
	StreamReader(char* filename);
	byte* ReadLine();
	byte  ReadByte();
	~StreamReader();
};
*/



#endif