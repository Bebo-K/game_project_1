#include "shader.h"
#include "../log.h"
#include "../structs/data_types.h"

Shader* defaultShader=nullptr;
StringMap cachedShaders(2);


Shader::Shader(char* vertexFile,char* fragmentFile){
    FileBuffer vertexProgram(vertexFile);
    FileBuffer fragmentProgram(fragmentFile);

    if(vertexProgram.contents == null){
        logger::exception("Shader::Shader -> Vertex shader not found: %s\n",vertexFile);
    }
    if(fragmentProgram.contents == null){
        logger::exception("Shader::Shader -> Fragment shader not found: %s\n",fragmentFile);
    }

    vertex = glCreateShader(GL_VERTEX_SHADER);
    fragment = glCreateShader(GL_FRAGMENT_SHADER);
    program = glCreateProgram();
    glShaderSource(vertex, 1, (GLchar**)&vertexProgram.contents,nullptr);
    glShaderSource(fragment, 1, (GLchar**)&fragmentProgram.contents,nullptr);

    glCompileShader(vertex);
    glCompileShader(fragment);

    GLint vtxCompileStatus = 0;
    GLint frgCompileStatus = 0;
    GLint linkStatus = 0;
    glGetShaderiv(vertex,GL_COMPILE_STATUS,&vtxCompileStatus);
    glGetShaderiv(fragment,GL_COMPILE_STATUS,&frgCompileStatus);
    
    if(vtxCompileStatus != 0 && frgCompileStatus != 0){
        glAttachShader(program,vertex);
        glAttachShader(program,fragment);
        
        glBindAttribLocation(program,ATTRIB_VERTEX,"a_vertex");
        glBindAttribLocation(program,ATTRIB_TEXCOORD,"a_tex_coord");
        glBindAttribLocation(program,ATTRIB_NORMAL,"a_normal");
        glBindAttribLocation(program,ATTRIB_BONE_INDEX,"a_bone_index");
        glBindAttribLocation(program,ATTRIB_BONE_WEIGHT,"a_bone_weight");
        glBindAttribLocation(program,ATTRIB_COLOR,"a_color");

        glLinkProgram(program);
        glGetProgramiv(program,GL_LINK_STATUS,&linkStatus);
        
        if(linkStatus != 0){
            MODELVIEW_MATRIX 	= glGetUniformLocation(program,"modelview_matrix");
            PROJECTION_MATRIX 	= glGetUniformLocation(program,"projection_matrix");
            NORMAL_MATRIX    	= glGetUniformLocation(program,"normal_matrix");
            TEXTURE_0 			= glGetUniformLocation(program,"texture0");
            COLOR				= glGetUniformLocation(program,"base_color");
            AMBIENT				= glGetUniformLocation(program,"ambient");
            DIFFUSE				= glGetUniformLocation(program,"diffuse");
            SPECULAR			= glGetUniformLocation(program,"specular");
            TEXTURE_LOCATION 	= glGetUniformLocation(program,"texture_location");
            IMAGE_POS           = glGetUniformLocation(program,"image_pos");
            IMAGE_SIZE 			= glGetUniformLocation(program,"image_size");
            WINDOW_SIZE 		= glGetUniformLocation(program,"window_size");
            POSE_MATRICES   	= glGetUniformLocation(program,"pose_matrices");

            int err = glGetError();
            if(err !=0){
                logger::warn("Shader::Shader -> got GLError %d binding shader.\n",err);
            }
        }
        else{//Linker error
            char* error_info_log = (char*)malloc(MAX_INFO_LOG_LENGTH);
            int error_info_log_length = 0;

            glGetProgramInfoLog(program,MAX_INFO_LOG_LENGTH,&error_info_log_length,error_info_log);
            error_info_log[error_info_log_length]=0;

            logger::warn("Shader::Shader -> GLSL Linker Error: %n%s\n",error_info_log);
            free(error_info_log);
        }
    }
    else{//Compile Error
            char* error_info_log = (char*)malloc(MAX_INFO_LOG_LENGTH);
            int error_info_log_length = 0;

            glGetShaderInfoLog(vertex,MAX_INFO_LOG_LENGTH,&error_info_log_length,error_info_log);
            error_info_log[error_info_log_length]=0;
            logger::info("Vertex Shader:\n");
            logger::info(error_info_log);

            glGetShaderInfoLog(fragment,MAX_INFO_LOG_LENGTH,&error_info_log_length,error_info_log);
            error_info_log[error_info_log_length]=0;
            logger::info("Fragment Shader:\n");
            logger::info(error_info_log);
            
            logger::warn("Shader::Shader -> GLSL Compile Error.\n");
            logger::flush();
            free(error_info_log);
    }
    int err = glGetError();
    if(err != 0){
        logger::warn("Shader.Build-> GL error: %d \n",err);
    }
}

Shader::~Shader(){
    glDetachShader(program,vertex);
    glDetachShader(program,fragment);
    glDeleteShader(vertex);
    glDeleteShader(fragment);
    glDeleteProgram(program);
}

void Shader::Use(){
    glUseProgram(program);
    int err = glGetError();
     if(err != 0){logger::warn("Shader.Use-> GL error: %d \n",err);}
}

void ShaderManager::Init(){
    defaultShader = new Shader("dat/gfx/default.vrt","dat/gfx/default.frg");
    cachedShaders.Add("default",(byte*)defaultShader);
}
void ShaderManager::Free(){
    Shader* pShader;
    for(int i=0;i<cachedShaders.Max();i++){
        pShader = (Shader*)cachedShaders.At(i);
        if(pShader)delete pShader;
    }
    cachedShaders.Clear();
}

void ShaderManager::AddShader(char* name,char* vertexFile,char* fragmentFile){
    if(cachedShaders.Get(name)!=null)return;
    Shader* newshader = new Shader(vertexFile,fragmentFile);
    cachedShaders.Add(cstr::new_copy(name),(byte*)newshader);
}

Shader* ShaderManager::GetShader(char* name){
    Shader* ret = (Shader*)cachedShaders.Get(name);
    if(ret == null){
        ret = DefaultShader();//4/11 rename your shaders
    }
    return ret;
}
void ShaderManager::RemoveShader(char* name){
    Shader* ret = (Shader*)cachedShaders.Remove(name);
    if(ret != null){
        delete ret;
    }   
}
Shader* ShaderManager::DefaultShader(){
    return defaultShader;
}