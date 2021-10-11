#include "image.h"
#include "../log.h"
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

byte GET_BITS(byte val,byte width,byte pos){
	return (val >> (8-(width+pos))) & (255 >> (8-width));
}

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
		ret = (byte)*base;
	}
	return ret;
}

unsigned char* DefilterScanline(unsigned char *scanline,unsigned char *prev,int scanline_len, int elements_per_px, int bits_per_element);
void FormatImageScanline(unsigned char* data,int y,int width,unsigned char* scanline,int elements_per_pixel,int bits_per_element,unsigned char* palette,unsigned char* alpha_pallete);

Image::Image(){
    width=0;height=0;
    image_data=nullptr;
}
Image::Image(int wid,int hgt){
    width = wid;
    height = hgt;
    image_data = (byte*)malloc(width*height*4);
}
Image::Image(Stream* image_stream){
    if(!image_stream->error){
        byte* filedata = (byte*)malloc(image_stream->length);
        image_stream->read(filedata,image_stream->length);
        if(!LoadData(filedata)){
            logger::warn("Image::Image -> Failed to parse file as PNG: %s\n",image_stream->uri);
        }
        free(filedata);
    }else{
        logger::warn("Image::Image -> Failed to read file %s\n",image_stream->uri);
		width=0;
		height=0;
		image_data=0;
    }
}
Image::Image(byte* raw_png){
    if(!LoadData(raw_png)){
        logger::warn("Image::Image -> Failed to parse PNG data.");
    }
}

Image::~Image(){
	free(image_data);
}
bool Image::LoadData(byte* data){
    int offset = 0;

	int elements_per_pixel =3; //will be 4 by the end of this
	int bits_per_element=8;
	int elements_per_scanline;
	int bytes_per_scanline;
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
        logger::exception("Image::LoadData -> invaid PNG signature %x\n",Sig);
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
            bits_per_element = read_byte(data,&offset);
			if(bits_per_element > 8){
                logger::exception("Image::LoadData -> (16+ bit channels unsupported) Image color channels are larger than 8 bits. What are we supposed to do with all those colors?!\n");
                return -2;
			}
			switch(read_byte(data,&offset)){ //color type
				case 0:elements_per_pixel=1;break;//greyscale
				case 2:elements_per_pixel=3;break;//RBG
				case 3:elements_per_pixel=1;// index in Pallete
						Paletted = 1;break;
				case 4:elements_per_pixel=2;break;// grayscale, alpha pairs
				case 6:elements_per_pixel=4;break; //RGBA
			    default: break;
            }
			read_byte(data,&offset);//Compression Method. Must be 0 (Deflate)
			read_byte(data,&offset);//Filter Method. Must be 0
			if(read_byte(data,&offset)){
                logger::warn("Image::LoadData -> (Interlacing unsupported) Image is interlaced, and will appear incorrectly. Please de-interlace the image with any image editor.\n");
            }

			elements_per_scanline = width*elements_per_pixel;
			bytes_per_scanline = 1 + (elements_per_scanline*bits_per_element/8);
			if((elements_per_scanline*bits_per_element)%8 != 0){bytes_per_scanline++;}

            DecompresseDataSz = height + bytes_per_scanline*height;
			DecompressedData = (byte*)malloc(DecompresseDataSz);

			memset(DecompressedData,0xBA,DecompresseDataSz);//why'd I choose hex 45 again?
			z.avail_out = DecompresseDataSz;
			z.next_out  = DecompressedData;
			ret = inflateInit(&z);
			break;}
		case IEND:
			Done = 1;
			break;
		case PLTE:{
			if((Length%3 !=0) |(Paletted== 0)){
                logger::exception("Image::LoadData -> PNG palette is invalid.\n");
                return -2;
            }
			Palette = (byte*)malloc(Length);
            memcpy(Palette,&data[offset],Length);
            offset += Length;
			break;
            }
		//Transparent Palleted images are not in PNG baseline.
		case tRNS:{
			TPalette = (byte*)malloc(Length);
			memcpy(TPalette,&data[offset],Length);
			offset += Length;
			break;
			}
		case IDAT:{ //Compressed, Possibly Filtered and/or Palleted Data
			z.avail_in = Length;
			z.next_in = (unsigned char*)(&data[offset]);//Saves an unnecessary copy.
			offset += Length;
			ret = inflate(&z,Z_NO_FLUSH);
			if(ret == Z_STREAM_END){inflateEnd(&z);}
			if(ret < 0) {
				logger::exception("Image::LoadData -> Z Stream Error:%s\n",z.msg);
				return 0;
			}
			break;
			}
		default:offset+=Length;break;
		}
	CRC = read_int(data,&offset);
	if(CRC == 0){
		//wow that's noteworthy
	}
	}

	long long size = width*height*4;
	image_data= (byte*)malloc(size);
	memset(image_data,0,width*height*4);

	unsigned char* scanline=nullptr;
	unsigned char* previous_scanline=nullptr;
	
	for(int i=0; i<height;i++){
 		scanline = DefilterScanline(&DecompressedData[i*bytes_per_scanline],previous_scanline,bytes_per_scanline,elements_per_pixel,bits_per_element);
		FormatImageScanline(image_data,i,width,scanline,elements_per_pixel,bits_per_element,Palette,TPalette);
		if(previous_scanline != nullptr){free(previous_scanline);}
		previous_scanline = scanline;
	}
	free(previous_scanline);
	free(Palette);
	free(TPalette);
	free(DecompressedData);
	return 1;
}


unsigned char* DefilterScanline(unsigned char *scanline,unsigned char *prev,int scanline_len, int elements_per_px, int bits_per_element){
	int len = scanline_len-1;
	unsigned char* out = (unsigned char*) malloc(len);
	memcpy(out,&scanline[1],len);

	int bpp = elements_per_px*bits_per_element/8; //bpp= bytes per pixel
	if(elements_per_px*bits_per_element % 8 != 0)bpp++;
	
	switch(scanline[0]){
		case 0: break;
		case 1: 
			for(int i=bpp;i<len;i++){out[i]+=out[i-bpp];};break;
		case 2: 
			if(prev==nullptr){break;}
			for(int i=0;i<len;i++){out[i] += prev[i];};break;
		case 3: 
			for(int i=0;i<len;i++){
				out[i] += ( ((i >= bpp)?out[i-bpp] : 0)  + ((prev != nullptr)?prev[i] : 0) ) / 2;         
			};break;
		case 4:
			for(int i=0;i<len;i++){
				out[i] += PAETH((i >= bpp)?out[i-bpp] : 0  , (prev != nullptr)?prev[i] : 0 , (i>= bpp && prev !=nullptr)? prev[i-bpp] : 0);       
			}break;
		default:logger::exception("Invalid PNG filter method:%d\n",scanline[0]);break;
	}
	return out;
}

char GetPixelElement(unsigned char *scanline,int x,int element,int elements_per_px,int bits_per_element){
	int bit_index = (x*elements_per_px + element) * bits_per_element;
	int byte_index = bit_index/8;
	int bit_offset = bit_index%8;
	return GET_BITS(scanline[byte_index],bits_per_element,bit_offset);
}


void FormatImageScanline(unsigned char* data,int y,int width,unsigned char* scanline,int elements_per_pixel,int bits_per_element,unsigned char* palette,unsigned char* alpha_pallete){
	if(palette != nullptr){
		int Index =0;
		for(int i=0; i<width; i++){
			Index = GetPixelElement(scanline,i,0,elements_per_pixel,bits_per_element);
			data[(y*width + i)* 4]  = palette[Index*3];
			data[(y*width + i)* 4 + 1]=palette[Index*3+1];
			data[(y*width + i)* 4 + 2]=palette[Index*3+2];
			data[(y*width + i)* 4 + 3]=(alpha_pallete)?alpha_pallete[Index]:255;	
		}
	}
	else{
		for(int i=0; i<width; i++){
			data[(y*width + i)* 4] = GetPixelElement(scanline,i,0,elements_per_pixel,bits_per_element);
			data[(y*width + i)* 4 + 1] = (elements_per_pixel > 2)?GetPixelElement(scanline,i,1,elements_per_pixel,bits_per_element) : data[(y*width + i)* 4];
			data[(y*width + i)* 4 + 2] = (elements_per_pixel > 2)?GetPixelElement(scanline,i,2,elements_per_pixel,bits_per_element) : data[(y*width + i)* 4];
			if(elements_per_pixel == 2){
				data[(y*width + i)* 4 + 3] = GetPixelElement(scanline,i,1,elements_per_pixel,bits_per_element);
			}
			else if(elements_per_pixel == 4){
				data[(y*width + i)* 4 + 3] = GetPixelElement(scanline,i,3,elements_per_pixel,bits_per_element);
			}
			else{
				data[(y*width + i)* 4 + 3] = 255;
			}
		}
	}
}


bool Image::Blit(Image* dest,int x,int y){
    if(x + width > dest->width || y + height > dest->height || x <0 || y <0){return false;}
	for(int i=0; i<height;i++){
        memcpy(&dest->image_data[((y+i)*dest->width + x)*4],&image_data[i*width*4],width*4);
	}
    return true;
}


Image* AlgorithmicImage::Gradient(int size){
	Image* ret = new Image(size,size);
	byte* image_data = ret->image_data;


	for(int i=0;i<size;i++){
		for(int j=0;j<size;j++){
			int ivalue = (int)((float)255.0f*i/size);
			int jvalue = (int)((float)255.0f*j/size);
			image_data[4*(i*size+j)] = ivalue;
			image_data[4*(i*size+j)+1] = 0;
			image_data[4*(i*size+j)+2] = jvalue;
			image_data[4*(i*size+j)+3] = 255;
			//if(ivalue%8==0){
			//	image_data[4*(i*size+j)] = 255;
			//	image_data[4*(i*size+j)+1] = 255;
			//	image_data[4*(i*size+j)+2] = 255;
			//}
		}
	}
	return ret;
}