#include "shader.h"
#include "../log.h"
#include "../struct/data_types.h"

Shader* DEFAULT_SHADER=nullptr;
Shader* CURRENT_SHADER=nullptr;
StringMap CACHED_SHADERS(2);


Shader::Shader(char* vertex_file,char* fragment_file){
    FileBuffer vertex_program(vertex_file);
    FileBuffer fragment_program(fragment_file);

    if(vertex_program.contents == null){
        logger::exception("Shader::Shader -> Vertex shader not found: %s\n",vertex_file);
    }
    if(fragment_program.contents == null){
        logger::exception("Shader::Shader -> Fragment shader not found: %s\n",fragment_file);
    }

    vertex = glCreateShader(GL_VERTEX_SHADER);
    fragment = glCreateShader(GL_FRAGMENT_SHADER);
    program = glCreateProgram();
    glShaderSource(vertex, 1, (GLchar**)&vertex_program.contents,nullptr);
    glShaderSource(fragment, 1, (GLchar**)&fragment_program.contents,nullptr);

    glCompileShader(vertex);
    glCompileShader(fragment);

    GLint vtx_compile_status = 0;
    GLint frg_compile_status = 0;
    GLint link_status = 0;
    glGetShaderiv(vertex,GL_COMPILE_STATUS,&vtx_compile_status);
    glGetShaderiv(fragment,GL_COMPILE_STATUS,&frg_compile_status);
    
    if(vtx_compile_status != 0 && frg_compile_status != 0){
        glAttachShader(program,vertex);
        glAttachShader(program,fragment);
        
        glBindAttribLocation(program,ATTRIB_VERTEX,"a_vertex");
        glBindAttribLocation(program,ATTRIB_TEXCOORD,"a_tex_coord");
        glBindAttribLocation(program,ATTRIB_NORMAL,"a_normal");
        glBindAttribLocation(program,ATTRIB_BONE_INDEX,"a_bone_index");
        glBindAttribLocation(program,ATTRIB_BONE_WEIGHT,"a_bone_weight");
        glBindAttribLocation(program,ATTRIB_COLOR,"a_color");

        glLinkProgram(program);
        glGetProgramiv(program,GL_LINK_STATUS,&link_status);
        
        if(link_status != 0){
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
    DEFAULT_SHADER = new Shader("dat/gfx/default.vrt","dat/gfx/default.frg");
    CACHED_SHADERS.Add("default",(byte*)DEFAULT_SHADER);
}
void ShaderManager::Free(){
    Shader* pShader;
    for(int i=0;i<CACHED_SHADERS.Max();i++){
        pShader = (Shader*)CACHED_SHADERS.At(i);
        if(pShader)delete pShader;
    }
    CACHED_SHADERS.Clear();
}

void ShaderManager::AddShader(char* name,char* vertex_file,char* fragment_file){
    if(CACHED_SHADERS.Get(name)!=null)return;
    Shader* newshader = new Shader(vertex_file,fragment_file);
    CACHED_SHADERS.Add(cstr::new_copy(name),(byte*)newshader);
}

/*
Shader* ShaderManager::GetShader(char* name){
    Shader* ret = (Shader*)CACHED_SHADERS.Get(name);
    if(ret== null){
        logger::warn("Shader not found, using default.");
        ret=DEFAULT_SHADER;
    }
    return ret;
}
*/

Shader* ShaderManager::UseShader(char* name){
    Shader* ret = (Shader*)CACHED_SHADERS.Get(name);
    if(ret== null){
        logger::warn("Shader not found, using default.");
        ret=DEFAULT_SHADER;
    }
    if(ret != CURRENT_SHADER){
        ret->Use();
        CURRENT_SHADER=ret;
    }
    return ret;
}



void ShaderManager::RemoveShader(char* name){
    Shader* ret = (Shader*)CACHED_SHADERS.Remove(name);
    if(ret != null){
        delete ret;
    }   
}
/*
Shader* ShaderManager::DefaultShader(){
    return defaultShader;
}
*/