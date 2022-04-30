#ifndef ASSET_MANAGER_H
#define ASSET_MANAGER_H

#include <game_project_1/io/stream.hpp>


class AssetManager{
    public:
    static void Init();
    static Stream* Texture(char* uri);
    static Stream* SkyboxTexture(char* uri);
    static Stream* Model(char* uri);
    static Stream* UI_Image(char* uri);
    static Stream* UI_Font(char* name);
    static Stream* VertexShader(char* uri);
    static Stream* FragmentShader(char* uri);
    static Stream* Level(int area_id);
};










#endif