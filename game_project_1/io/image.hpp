#ifndef IMAGE_H
#define IMAGE_H

#define IPNG 0x49504E47
#define BPNG 0x89504E47
#define IHDR 0x49484452
#define IEND 0x49454E44
#define IDAT 0x49444154
#define PLTE 0x504C5445
#define tRNS 0x74524E53
#define PNG_LITTLE_ENDIAN true

#include <game_project_1/io/stream.hpp>


class Image{
    public:
        byte* image_data;
        int width;
        int height;

    Image();
    Image(int width,int height);
    Image(Stream* image_file);
    Image(byte* raw_png);
    ~Image();

    bool LoadData(byte* data);
    bool Blit(Image* dest,int x,int y);
};

namespace AlgorithmicImage{
    Image* Gradient(int size);


}


#endif
