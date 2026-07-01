export module File;

import <string>;
import Log;



const char OS_PATH_SEPERATOR =
#ifdef _WIN32
		'\\';
#else
			'/';
#endif

typedef unsigned int uint32;
typedef unsigned char byte;

//Simple input file for reading. Only uses char* filenames, so it should only be used for small relative paths.
export class IFile{
	FILE* file_handle;
public:
	const char* path;
	long amount_read;
	long length;
	bool error;
	IFile(){
		file_handle=nullptr;
		length=-1;
		error=true;
		amount_read=0;
	}
	IFile(const char* filename){
		path=filename;
		file_handle = fopen(filename,"rb");
		amount_read = 0;
		length = -1;
		error=false;
		if(file_handle == nullptr){
			logger::warn("File::open -> Cannot open file %s\n",filename);
			error=true;
		}
		else{
			fseek(file_handle, 0, SEEK_END);
			length = ftell(file_handle);
			fseek(file_handle, 0, SEEK_SET);
		}
	}
	int read(void* dest, int bytes){
		if(error){
			logger::warn("File::read -> File is closed or in an error state.\n");
			return 0;
		}
		int read_amount = fread(dest,1,bytes,file_handle);
		if(read_amount != bytes){
			logger::warn("File::read -> Failed to read %d bytes from file.\n",bytes);
			error=true;
			return 0;
		}
		amount_read += bytes;
		return amount_read;
	}
	int peek(void* dest,int bytes){
		int read_amount = fread(dest,1,bytes,file_handle);
		fseek(file_handle,-bytes,SEEK_CUR);
		if(read_amount != bytes){
			logger::warn("File::peek -> Failed to peek %d bytes from file.\n",bytes);
			error=true;
			return 0;
		}
		return read_amount;
	}
	void close(){
		fclose(file_handle);
		amount_read = -1;
		error=true;//to keep people from reading.
	}
	static char* GetPathOf(const char* filename){
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
	static bool Exists(const char* filename){
		if(filename==nullptr)return false;
		FILE *file = fopen(filename, "r");
		if (file){
			fclose(file);
			return true;
		}
		return false;
	}
};



//File type for reading/writing files outside of game directory.
//Uses wide char filepaths to function properly with user's FS structure.
export class UserFile{
	FILE* file_handle;
public:
	const wchar_t* path;
	long amount_read;
	long amount_written;
	long length;
	bool error;
	bool read_mode;

	UserFile(const wchar_t* filename,char mode){
		path=filename;
		amount_written = 0;
		amount_read = 0;

		if(mode =='r'){
			file_handle = _wfopen(filename,L"rb");
			read_mode=true;
			if(file_handle){
				fseek(file_handle, 0, SEEK_END);
				length = ftell(file_handle);
				fseek(file_handle, 0, SEEK_SET);
			}
		}
		else if(mode == 'w'){
			file_handle = _wfopen(filename,L"wb");
			read_mode=false;
		}
		else{
			logger::exception("UserFile::open -> Invalid mode %c\n",mode);
		}
		error=false;
		if(file_handle == nullptr){
			logger::warnW(L"UserFile::open -> Cannot open file %S\n",filename);
			error=true;
		}
	}
	static bool Exists(const wchar_t* filename){
		FILE* file_handle = _wfopen(filename,L"rb");
		bool exists = (file_handle != nullptr);
		if(exists){fclose(file_handle);}
		return exists;
	}

	void read(void* dest,int bytes){
		if(!read_mode){
			logger::exception("UserFile::read -> File is open for write only.\n");
			return;
		}
		if(error){
			logger::warn("UserFile::read -> File is closed or in an error state.\n");
			return;
		}
		int read_amount = fread(dest,1,bytes,file_handle);
		if(read_amount != bytes){
			logger::warn("UserFile::read -> Failed to read %d bytes from file.\n",bytes);
			error=true;
			return;
		}
		amount_read += bytes;
	}
	void peek(void* dest,int bytes){
		if(!read_mode){
			logger::exception("UserFile::peek -> File is open for write only.\n");
			return;
		}
		int read_amount = fread(dest,1,bytes,file_handle);
		fseek(file_handle,-bytes,SEEK_CUR);
		if(read_amount != bytes){
			logger::warn("UserFile::peek -> Failed to peek %d bytes from file.\n",bytes);
			error=true;
			return;
		}
	}
	void write(byte* data,int bytes){
		if(read_mode){
			logger::exception("UserFile::write -> File is open for read only.\n");
			return;
		}
		if(error){
			logger::warn("UserFile::write -> File is closed or in an error state.\n");
			return;
		}
		int write_amount = fwrite(data,1,bytes,file_handle);
		if(write_amount != bytes){
			logger::warn("UserFile::write -> Failed to write %d bytes to file.\n",bytes);
			error=true;
			return;
		}
		amount_written += bytes;
	}
	void close(){
		fclose(file_handle);
		error=true;
	}
};


//Quick and dirty file line reader for parsing config files
export class FileReader{
public:
	int mark;
	int length;
	byte* contents;
	byte* last_line;

	FileReader(const char* filename){
		mark=0;
		last_line=nullptr;
		IFile temp_file(filename);
		length = temp_file.length;
		contents = (byte*)calloc(length,1);
		temp_file.read(contents,length);
		temp_file.close();
	}
	byte* ReadLine(){
		if(last_line != nullptr){free(last_line);}
		if(mark >= length){return nullptr;}
		int newmark = mark+1;
		byte next_byte = contents[newmark];
		while(next_byte != 0x0D && next_byte != 0x0A && newmark < length){
			newmark++;
			next_byte = contents[newmark];
		}
		int strlen = newmark-mark;
		byte* ret = (byte*)malloc(strlen+1);
		memcpy(ret,&contents[mark],strlen);
		ret[strlen]=0;

		mark= newmark+1;
		if(mark == 0x0D){mark++;}//Windows line endings are 0x0D,0x0A
		mark++;//Unix are just 0x0A
		last_line=ret;

		return ret;
	}
	byte  ReadByte(){
		return contents[mark++];
	}
	~FileReader(){
		free(contents);
	}
};



/*
FileBuffer::FileBuffer(char* filename){
	source = File(filename);
	contents=nullptr;
	if(source.length > 0){
		contents = (byte*)malloc(source.length+1);
		source.read(contents,source.length);
		contents[source.length] = 0;
	}
};
int FileBuffer::Length(){return (int)source.length;}
FileBuffer::~FileBuffer(){free(contents);}
*/


/*
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
*/