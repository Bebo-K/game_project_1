#include <game_project_1/io/asset_manager.hpp>
#include <game_project_1/types/data_types.hpp>
#include <game_project_1/log.hpp>
#include <game_project_1/game/areas.hpp>

char filepath_base[] = "dat/";

char* BuildFileName(char* data_dir, char* file_name, char* file_type){
    int base_len = cstr::len(filepath_base);
    int path_len = cstr::len(data_dir);
    int name_len = cstr::len(file_name);
    int type_len = cstr::len(file_type);
    int filename_len = base_len + path_len + name_len + type_len + 1;
    char* filename = (char*)calloc(filename_len,sizeof(char));
    memcpy(filename,filepath_base,base_len);
    memcpy(&filename[base_len],data_dir,path_len);
    memcpy(&filename[base_len+path_len],file_name,name_len);
    memcpy(&filename[base_len+path_len+name_len],file_type,type_len);
    return filename;
}

void AssetManager::Init(){
    //test for data directory
    if(!IFile::Exists(cstr::append(filepath_base,"assets.txt"))){
        logger::fatal("Data directory not found! Please make sure the application is running from the installed directory.");
    }
}
Stream* AssetManager::Texture(char* uri){
    char* filename = BuildFileName("textures/",uri,".png");
    FileStream* texture_stream = new FileStream(filename);
    free(filename);
    return texture_stream;
}
Stream* AssetManager::SkyboxTexture(char* uri){
    char* filename = BuildFileName("textures/skybox/",uri,".png");
    FileStream* texture_stream = new FileStream(filename);
    free(filename);
    return texture_stream;
}
Stream* AssetManager::Model(char* uri){
    char* filename = BuildFileName("models/",uri,".glb");
    FileStream* model_stream = new FileStream(filename);
    free(filename);
    return model_stream;
}
Stream* AssetManager::UI_Image(char* uri){
    char* filename = BuildFileName("ui/img/",uri,".png");
    FileStream* image_stream = new FileStream(filename);
    free(filename);
    return image_stream;
}
Stream* AssetManager::UI_Font(char* font_name){
    char* filename = BuildFileName("ui/fonts/",font_name,".ttf");
    FileStream* font_stream = new FileStream(filename);
    free(filename);
    return font_stream;
}
Stream* AssetManager::VertexShader(char* uri){
    char* filename = BuildFileName("gfx/",uri,".vrt");
    FileStream* shader_stream = new FileStream(filename);
    free(filename);
    return shader_stream;
}
Stream* AssetManager::FragmentShader(char* uri){
    char* filename = BuildFileName("gfx/",uri,".frg");
    FileStream* shader_stream = new FileStream(filename);
    free(filename);
    return shader_stream;
}
Stream* AssetManager::Level(int area_id){
    char* uri = Areas::GetMapName(area_id);
    char* filename = BuildFileName("levels/",uri,".lvl");
    FileStream* level_stream = new FileStream(filename);
    free(filename);
    return level_stream;
}

