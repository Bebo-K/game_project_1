#include "shader.h"
#include "../asset_manager.h"
#include "../log.h"
#include "../struct/map.h"

Shader* DEFAULT_SHADER=nullptr;
Shader* CURRENT_SHADER=nullptr;
Map<char*,Shader*> CACHED_SHADERS(2);


Shader::Shader(char* vertex_uri,char* fragment_uri){
    Stream* vertex_shader_stream = AssetManager::VertexShader(vertex_uri);
    Stream* fragment_shader_stream = AssetManager::FragmentShader(vertex_uri);
    byte* vertex_program = vertex_shader_stream->readAll();
    byte* fragment_program = fragment_shader_stream->readAll();
    delete vertex_shader_stream;
    delete fragment_shader_stream;

    if(vertex_program == nullptr){
        logger::exception("Shader::Shader -> Vertex shader not found: %s\n",vertex_uri);
    }
    if(fragment_program == nullptr){
        logger::exception("Shader::Shader -> Fragment shader not found: %s\n",fragment_uri);
    }

    vertex = glCreateShader(GL_VERTEX_SHADER);
    fragment = glCreateShader(GL_FRAGMENT_SHADER);
    program = glCreateProgram();
    glShaderSource(vertex, 1, (GLchar**)&vertex_program,nullptr);
    glShaderSource(fragment, 1, (GLchar**)&fragment_program,nullptr);

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
            COLOR				= glGetUniformLocation(program,"color");
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
    logger::debug("Loading default shader..");
    DEFAULT_SHADER = new Shader("default","default");
    CACHED_SHADERS.Add("default",DEFAULT_SHADER);
    
    AddShader("level_debug");
    AddShader("model_dynamic_lighting");
    AddShader("model_static_shadeless");
    AddShader("skybox");
    AddShader("skybox_flat");
    AddShader("ui_default");
    AddShader("ui_shape");
    AddShader("ui_sprite");
    AddShader("ui_text");
}

void ShaderManager::Free(){
    for(Tuple<char*,Shader*> shader: CACHED_SHADERS){
        if(shader.value) delete shader.value;
    }
    CACHED_SHADERS.Clear();
}

void ShaderManager::AddShader(char* name){
    if(CACHED_SHADERS.Has(name))return;
    logger::debug("Caching shader %s\n",name);
    CACHED_SHADERS.Add(name,new Shader(name,name));
}

void ShaderManager::AddShader(char* name,char* vertex_uri,char* fragment_uri){
    if(CACHED_SHADERS.Has(name))return;
    logger::debug("Caching shader %s\n",name);
    CACHED_SHADERS.Add(name,new Shader(vertex_uri,fragment_uri));
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
    Shader* ret = CACHED_SHADERS.Get(name);
    if(ret== nullptr){
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
    Shader* ret = CACHED_SHADERS.Get(name);
    CACHED_SHADERS.Remove(name);
    if(ret != nullptr){
        delete ret;
    }   
}
/*
Shader* ShaderManager::DefaultShader(){
    return defaultShader;
}
*/