#include "image.h"
#include "log.h"
#include <string.h>
#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <zlib.h>

/*
    Hello, traveler. This code before you is ancient, an artifact of games 100 times abandoned.
    I know not the date this code was forged, however, I have kept the
    comment block that follows to serve as a clue to it's age.
    Fear not, I have undone the ANSI C madness described below.
*/

/*Dang, I wrote this a long time ago. 
 Me and this code have been at an agreement:
 I don't touch it, and it will keep working....probably.*/
 //2/24/2015-Touched it. It's now ANSI C and it still works. (SuccessBaby.png)

//Thanks for the specs from http://www.libpng.org/pub/png/book/chapter08.html#png.ch08.div.5.2


int ENDIAN_SWAP(unsigned int i) {
	unsigned int ret = i;
	unsigned int val = 0;
	val |= (ret&0xFF000000)>>24;
	val |= (ret&0x00FF0000)>>8;
	val |= (ret&0x0000FF00)<<8;
	val |= (ret&0x000000FF)<<24;
	return val;}

int PAETH(int a,int b,int c){
	int p = a + b - c;
	if ((abs(p - a) <= abs(p - b))&&(abs(p - a) <=  abs(p - c)))return a;
	else if (abs(p - b)<= abs(p - c))return b;
	else return c;}

uint32 read_int(byte* base,int* p_offset){
	uint32 ret = 0;
	if(p_offset != nullptr){
		ret = *(uint32*)(&base[*p_offset]);
		*p_offset += 4;
	}
	else{
		ret = *(uint32*)base;
	}
	return (PNG_LITTLE_ENDIAN)?ENDIAN_SWAP(ret):ret;
}
byte read_byte(byte* base,int *p_offset){
	byte ret = 0;
	if(p_offset != nullptr){
		ret = (byte)base[*p_offset];
		*p_offset += 1;
	}
	else{
		ret = (byte)&base;
	}
	return ret;
}

void FormatImage(unsigned char *in,unsigned char *out,unsigned char *Palette,unsigned char *tPalette,int Width,int Height,int BPP);



Image::Image(){
    width=0;height=0;
    image_data=nullptr;
}
Image::Image(const char* filename){
    File f = File(filename);
    if(!f.error){
        byte* filedata = (byte*)malloc(f.length);
        f.read(filedata,f.length);
        if(!load(filedata)){
            log("Error @ Image::Image -> Failed to parse file as PNG: %s\n",filename);
        }
        free(filedata);
    }else{
        log("Error @ Image::Image -> Failed to read file %s\n",filename);
    }
}
Image::Image(byte* raw_png){
    if(!load(raw_png)){
        log("Error @ Image::Image -> Failed to parse PNG data.");
    }
}
void Image::unload(){
    free(image_data);
}

bool Image::load(byte* data){
    int offset = 0;

	int BytesPerPixel =3; //will be 4 by the end of this
	uint32 Sig=0;

	int ret=0;
	char Done=0;
	char Paletted=0;

	uint32 Length=0;
	uint32 Type=0;
	uint32 CRC=0;

    uint32 DecompresseDataSz =0;
	byte* DecompressedData= NULL;
	byte* Palette= NULL;
	byte* TPalette = NULL;

	z_stream z;
	memset(&z,0,sizeof(z_stream));
	z.zalloc = Z_NULL;
	z.zfree = Z_NULL;
	z.opaque = Z_NULL;
	z.avail_in = 0;
	z.next_in = Z_NULL;
    
    image_data = nullptr;
    width = height =0;


	//Load File + Check Sig
    Sig = read_int(data,&offset);
	if(Sig != IPNG && Sig != BPNG){
        log("Error @ Image::load -> invaid PNG signature %x\n",Sig);
        return -1;
    } //wrong filetype

    read_int(data,&offset);//Reserved 4 bytes with no use.

    //Read Chunk, its always (LENGTH,TYPE,DATA,CRC)
	while(!Done){
    Length = read_int(data,&offset);
    Type = read_int(data,&offset);
	switch(Type){
		case IHDR:{
            width = (int)read_int(data,&offset);
            height = (int)read_int(data,&offset);
            read_byte(data,&offset);//Bit Depth
			switch(read_byte(data,&offset)){ //color type
				case 0:BytesPerPixel=1;break;//greyscale
				case 2:BytesPerPixel=3;break;//RBG
				case 3:BytesPerPixel=1;// INDEX I32O PALLATE
						Paletted = 1;break;
				case 4:BytesPerPixel=2;break;// GRAY,A
				case 6:BytesPerPixel=4;break; //RGBA
			    default: break;
            }
			read_byte(data,&offset);//Compression Method. Is DEFLATE.
			read_byte(data,&offset);//Filter Method. If this not be 0, we got problems
			if(read_byte(data,&offset)){
                log("Warning @ Image::load -> (Interlacing unsupported) Image is interlaced, and will appear incorrectly. Please de-interlace the image with any image editor.");
            }

            DecompresseDataSz = (height+1)*width*BytesPerPixel;
			DecompressedData = (byte*)malloc(DecompresseDataSz);
			memset(DecompressedData,0x45,DecompresseDataSz);//why'd I choose hex 45 again?
			z.avail_out = DecompresseDataSz;
			z.next_out  = DecompressedData;
			ret = inflateInit(&z);
			break;}
		case IEND:
			Done = 1;
			break;
		case PLTE:{
			if((Length%3 !=0) |(Paletted== 0)){
                log("Error @ Image::load -> PNG palette is invalid.");
                return -2;
            }
			Palette = (byte*)malloc(Length);
            memcpy(Palette,&data[offset],Length);
            offset += Length;
			break;
            }//TODO: start here
		//Transparent Palleted images are not in PNG baseline.
		case tRNS:{
			TPalette = malloc(Length);
			File_Read(&f,TPalette,Length);break;}
		case IDAT:{ //Compressed, Possibly Filtered and/or Palleted Data
			z.avail_in = Length;
			z.next_in = (unsigned char*)(f.dat+f.read_index);//Saves an unnecessary copy.
			f.read_index +=Length;
			ret = inflate(&z,Z_NO_FLUSH);
			if(ret == Z_STREAM_END)inflateEnd(&z);
			if(ret < 0) {
				/*"ZStream Error"*/
				printf("Z Stream Error:%s\n",z.msg);
				return 0;
				}
			break;
			
			}
		default:f.read_index+=Length;break;
		}
	File_Read(&f,&CRC,sizeof(int)); //CRC value unused for now. Don't hex edit your pics kids.
	}

	long long size = i->width*i->height*4;
	File_Close(&f);
	i->image_data= malloc(size);
	FormatImage(DecompressedData,i->image_data,Palette,TPalette,i->width,i->height,BytesPerPixel);
	free(Palette);
	free(TPalette);
	free(DecompressedData);
	return 1;
}

void FormatImage(unsigned char *in,unsigned char *out,unsigned char *Palette,unsigned char *tPalette,int Width,int Height,int BPP){
	memset(out,0,Width*Height*4);
	unsigned char fil= in[0];
	for(int j=0; j<Width; j++){
		for(int k=0;k<BPP;++k){
			out[j*4+ k] = in[j*BPP+ k + 1];		
			}
		}

	//First line filtered independently
	if(fil==1){for(int j=0; j<Width*4; j++){out[j] += (j-4 >= 0)?out[j-4]:0;} }
	if(fil==3){for(int j=0; j<Width*4; j++){out[j] += (j-4 >= 0)?out[j-4]/2:0;} }
	/////////

	int row = Width*4;

	for(int i=1; i<Height;i++){
		fil = in[i*(Width*BPP+1)];
		for(int j=0; j<Width; j++){
			for(int k=0;k<BPP;k++){
				out[(i*row)+j*4+ k] = in[i*(Width*BPP + 1)+j*BPP+ k+1]; //shift everything over a byte, and space the array to 4BPP
				}
			}
		switch(fil){
		case 0: break;
		
		case 1:{
			for(int j=4; j<row; j++){
				out[i*row+j] +=	out[i*row+j -4];}
			break;} //Last

		case 2:{
			for(int j=0; j<row; j++){
				out[i*row+j] += out[i*row+j-row];}
			break;} //Up

		case 3:{
			for(int f=0; f<BPP;f++){
				out[i*row+f] += out[(i-1)*row+f]/2;}
			for(int j=4; j<row; j++){
				int avg = ((int)out[i*row+j-4]+(int)out[(i-1)*row+j])/2;
				out[i*row+j] += avg;}
			break;} //AVG(Last,Up)

		case 4:{
			for(int f=0; f<BPP;f++){
				out[i*row +f] +=PAETH(0,out[(i-1)*row+f],0);}
			for(int j=4; j<row; j++){
				out[i*row+j] +=PAETH(out[i*row+j-4],out[(i-1)*row+j],out[(i-1)*row+j-4]);}
			break;} //PAETH(Last,Up,Up+Last)

		default:printf("Invalid PNG filter method:%d\n",fil);break;
		}
	}
	///depalette
	if(Palette){
		unsigned int Index =0;
		for(unsigned int i=0; i<Height;i++){
			for(unsigned int j=0; j<Width;j++){
			Index = out[i*row+(j*4)];
			out[i*row+(j*4)]  =Palette[Index*3];
			out[i*row+(j*4)+1]=Palette[Index*3+1];
			out[i*row+(j*4)+2]=Palette[Index*3+2];
			out[i*row+(j*4)+3]=(tPalette)?tPalette[Index]:255;
				
			}
		}
	}
	//////convert to RGBA
	else{
		switch(BPP){
		case 1://greyscale
		for(unsigned int i=0; i<Height;i++){
			for(unsigned int j=0; j<Width;j++){
				out[(i*Width+j)*4+1]=out[(i*Width+j)*4];
				out[(i*Width+j)*4+2]=out[(i*Width+j)*4];
				out[(i*Width+j)*4+3]=255;
				}
			}
		break;
		case 2://greyscale w/alpha?
		for(unsigned int i=0; i<Height;i++){
			for(unsigned int j=0; j<Width;j++){
				out[(i*Width+j)*4+1]=out[(i*Width+j)*4];
				out[(i*Width+j)*4+2]=out[(i*Width+j)*4];
				out[(i*Width+j)*4+3]=255;
				}
			}
		break;
		case 3://RGB
		for(unsigned int i=0; i<Height;i++){
			for(unsigned int j=0; j<Width;j++){
				out[(i*Width+j)*4 +3]=255;
			}
		}
		break;

		case 4:/*Image is 4 channel RGBA, no work needed.*/break;
		default:break;
		}
	}
}

Image::Image(int width,int height){
    width = width;
    height = height;
    image_data = (byte*)malloc(width*height*4);
}

bool Image::blit(Image* dest,int x,int y){
    if(dest->width + x > width || dest->height + y > height || x <0 || y <0){return false;}
	for(unsigned int i=0; i<height;i++){
        memcpy(&dest->image_data[(y+i)*dest->width + (x)*4],&image_data[i*width*4],width*4);
	}
    return true;
}
