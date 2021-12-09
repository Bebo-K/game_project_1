#ifndef ASSET_MANAGER_H
#define ASSET_MANAGER_H

#include "io/stream.h"


class AssetManager{
    public:
    static void Init();
    static Stream* Texture(char* uri);
    static Stream* Model(char* uri);
    static Stream* UI_Image(char* uri);
    static Stream* UI_Font(char* name);
    static Stream* VertexShader(char* uri);
    static Stream* FragmentShader(char* uri);
    static Stream* Level(char* uri);
};










#endif