#include <game_project_1/io/stream.hpp>
#include <game_project_1/types/str.hpp>

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
    length=backing_file.length;
    amount_read=0;
}

int FileStream::read(void* dest,int bytes){
    int read_amount = backing_file.read(dest,bytes);
    amount_read += read_amount;
    return read_amount;
}

int FileStream::peek(void* dest,int bytes){
    return backing_file.peek(dest,bytes);
}

byte* FileStream::readAll(){
    byte* dat = (byte*)malloc(length+1);
    int bytes_read = backing_file.read(dat,length);
    if(bytes_read != length){
        free(dat);
        return nullptr;
    }
    amount_read = bytes_read;
    dat[length] = 0;
    return dat;
}

FileStream::~FileStream(){
    backing_file.close();
}