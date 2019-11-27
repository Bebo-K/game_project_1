
#include "file.h"


void File::open(char* filename){
	file_handle = fopen(filename,"r");
	amount_read = 0;
}
void File::read(void* dest, int bytes){
	int read_amount = fread(dest,1,bytes,file_handle);
	if(read_amount != bytes){
		printf("Error @ File::read -> Failed to read %d bytes from file.",bytes);
		return;
	}
	amount_read += bytes;
}
void File::close(){
	fclose(file_handle);
	amount_read = -1;
}