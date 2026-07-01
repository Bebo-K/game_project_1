export module AssetManager;

import Primitives;
import Logger;
import Areas;

char filepath_base[] = "dat/";


export class AssetManager{
    public:
    static void Init(){
        //test for data directory
        if(!IFile::Exists(cstr::append(filepath_base,"assets.txt"))){
            logger::fatal("Data directory not found! Please make sure the application is running from the installed directory.");
        }
    }
    static Stream* Texture(char* uri){
        char* filename = BuildFileName("textures/",uri,".png");
        FileStream* texture_stream = new FileStream(filename);
        free(filename);
        return texture_stream;
    }
    static Stream* SkyboxTexture(char* uri){
        char* filename = BuildFileName("textures/skybox/",uri,".png");
        FileStream* texture_stream = new FileStream(filename);
        free(filename);
        return texture_stream;
    }
    static Stream* Model(char* uri){
        char* filename = BuildFileName("models/",uri,".glb");
        FileStream* model_stream = new FileStream(filename);
        free(filename);
        return model_stream;
    }
    static Stream* UI_Image(char* uri){
        char* filename = BuildFileName("ui/img/",uri,".png");
        FileStream* image_stream = new FileStream(filename);
        free(filename);
        return image_stream;
    }
    static Stream* UI_Font(char* font_name){
        char* filename = BuildFileName("ui/fonts/",font_name,".ttf");
        FileStream* font_stream = new FileStream(filename);
        free(filename);
        return font_stream;
    }
    static Stream* VertexShader(char* uri){
        char* filename = BuildFileName("shaders/",uri,".vrt");
        FileStream* shader_stream = new FileStream(filename);
        free(filename);
        return shader_stream;
    }
    static Stream* FragmentShader(char* uri){
        char* filename = BuildFileName("shaders/",uri,".frg");
        FileStream* shader_stream = new FileStream(filename);
        free(filename);
        return shader_stream;
    }
    static Stream* Level(int area_id){
        Area* area = Areas.Has(area_id)? Areas.Get(area_id) :ERROR_ROOM;
        char* filename = BuildFileName("areas/",area->path,".lvl");
        FileStream* level_stream = new FileStream(filename);
        free(filename);
        return level_stream;
    }
};

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

