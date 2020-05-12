
#include "file.h"
#include "../log.h"
#include <string.h>

File::File(){
	file_handle=nullptr;
	length=-1;
	error=true;
	amount_read=0;
}
File::File(const char* filename){
	path=filename;
	file_handle = fopen(filename,"rb");
	amount_read = 0;
	length = -1;
	error=false;
	if(file_handle == nullptr){
		logger::warn("File::open -> Cannot open file %s",filename);
		error=true;
	}
	else{
		fseek(file_handle, 0, SEEK_END);
		length = ftell(file_handle);
		fseek(file_handle, 0, SEEK_SET);
	}
}


char* File::GetPathOf(const char* filename){
	if(filename==nullptr)return nullptr;
	int last_seperator=0;
	int end = strlen(filename);
	for(int i=0;i<end;i++){
		if(filename[i]=='/'||
		filename[i]=='\\'
		){last_seperator=i;}
	}
	char* ret = (char*)calloc(last_seperator+1,sizeof(char));//sets trailing 0
	memcpy(ret,filename,last_seperator);
	return ret;
}


bool File::Exists(const char* filename){
	if(filename==nullptr)return false;
    FILE *file = fopen(filename, "r");
    if (file){
        fclose(file);
        return true;
    }
    return false;
}


void File::read(void* dest, int bytes){
	if(error){
		logger::warn("File::read -> File is closed or in an error state.");
		return;
	}
	int read_amount = fread(dest,1,bytes,file_handle);
	if(read_amount != bytes){
		logger::warn("File::read -> Failed to read %d bytes from file.",bytes);
		error=true;
		return;
	}
	amount_read += bytes;
}
void File::peek(void* dest,int bytes){
	if(error){
		logger::warn("File::peek -> File is closed or in an error state.");
		return;
	}
	int read_amount = fread(dest,1,bytes,file_handle);
	fseek(file_handle,-bytes,SEEK_CUR);
	if(read_amount != bytes){
		logger::warn("File::peek -> Failed to peek %d bytes from file.",bytes);
		error=true;
		return;
	}
}

void File::close(){
	fclose(file_handle);
	amount_read = -1;
	error=true;//to keep people from reading.
}

FileBuffer::FileBuffer(const char* filename){
	source = File(filename);
	contents=nullptr;
	if(source.length > 0){
		contents = (byte*)malloc(source.length+1);
		source.read(contents,source.length);
		contents[source.length] = 0;
	}
};

int FileBuffer::Length(){
	return (int)source.length;
}

FileBuffer::~FileBuffer(){
	free(contents);
}

byte* GetFile(const char* filename){
    FILE *f = fopen(filename, "rb");
	byte* data = nullptr;
	if(f != nullptr){
		fseek(f, 0, SEEK_END);
		long fsize = ftell(f);
		fseek(f, 0, SEEK_SET);
		data = (byte *)malloc(fsize);
		fread(data, fsize, 1, f);
		fclose(f);
	}
	else{
		logger::warn("getFile -> could not open filename %s",filename);
	}
	return data;
}


char* GetRelativeFilename(const char* filename,const char* reference_file){
	int last_instance_of_seperator=0;
	for(int i=0;reference_file[i]!= 0;i++){
		if(reference_file[i] == '/' ||
		reference_file[i] == '\\' ){last_instance_of_seperator=i;}
	}
	int filename_len = 0;
	for(filename_len=0;filename[filename_len] != 0;filename_len++);
	filename_len += last_instance_of_seperator;
	char* relativename = (char*)malloc(sizeof(char)*(filename_len+1));
	for(int c=0;c<=last_instance_of_seperator;c++){
		relativename[c]=reference_file[c];
	}
	for(int d=0;d < (filename_len-last_instance_of_seperator);d++){
		relativename[last_instance_of_seperator+d]=filename[d];
	}
	relativename[filename_len] =0;
	return relativename;
}

//gets a file starting relatively from the path of reference_file
byte* GetRelativeFile(const char* filename,const char* reference_file){
	char* relative_filename = GetRelativeFilename(filename,reference_file);
	byte* file= GetFile(relative_filename);
	free(relative_filename);
	return file;
}