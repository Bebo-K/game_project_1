
#include "file.h"
#include "log.h"


File::File(const char* filename){
	file_handle = fopen(filename,"rb");
	amount_read = 0;
	length = -1;
	error=false;
	if(file_handle == nullptr){
		log("Error @ File::open -> Cannot open file %s",filename);
		error=true;
	}
	else{
		fseek(file_handle, 0, SEEK_END);
		length = ftell(file_handle);
		fseek(file_handle, 0, SEEK_SET);
	}
}
void File::read(void* dest, int bytes){
	if(error){
		log("Error @ File::read -> File is closed or in an error state.");
		return;
	}
	int read_amount = fread(dest,1,bytes,file_handle);
	if(read_amount != bytes){
		log("Error @ File::read -> Failed to read %d bytes from file.",bytes);
		error=true;
		return;
	}
	amount_read += bytes;
}
void File::close(){
	fclose(file_handle);
	amount_read = -1;
	error=true;//to keep people from reading.
}


byte* getFile(const char* filename){
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
		log("Error @ getFile: could not open filename %s",filename);
	}
	return data;
}

