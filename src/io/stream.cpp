#include "stream.h"

#include "../struct/data_types.h"

Stream::Stream(char* uri_path){uri = cstr::new_copy(uri_path);}
Stream::~Stream(){
    free(uri);
    error=true;
}
char* Stream::parentURI(){
    int last_instance_of_seperator=0;
	for(int i=0;uri[i]!= 0;i++){
		if(uri[i] == '/' || uri[i] == '\\' ){last_instance_of_seperator=i;}
	}
    char* parent_uri = (char*)calloc(last_instance_of_seperator,1);
    memcpy(parent_uri,uri,last_instance_of_seperator-1);
    return parent_uri;
}

FileStream::FileStream(char* uri_path):Stream(uri_path), backing_file(uri_path){
    error=backing_file.error;
}

int FileStream::read(void* dest,int bytes){
    return backing_file.read(dest,bytes);
}

int FileStream::peek(void* dest,int bytes){
    return backing_file.peek(dest,bytes);
}

FileStream::~FileStream(){
    backing_file.close();
}


byte* ReadStream(Stream* stream){
    byte* dat = (byte*)malloc(stream->length);
    if(stream->read(dat,stream->length) == 0){
        free(dat);
        return nullptr;
    }
    return dat;
}