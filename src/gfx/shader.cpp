#include "shader.h"
#include "../io/log.h"
#include "../io/data_structs.h"

Shader defaultShader;
AssociativeArray cachedShaders(2);


bool Shader::Load(const char* vertexFile,const char* fragmentFile){

    FileBuffer vertexProgram(vertexFile);
    FileBuffer fragmentProgram(fragmentFile);

    if(vertexProgram.contents == null){
        logger::exception("Shader::Load -> Vertex shader not found: %s",vertexFile);
        return false;
    }
    if(vertexProgram.contents == null){
        logger::exception("Shader::Load -> Fragment shader not found: %s",fragmentFile);
        return false;
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
        glBindAttribLocation(program,ATTRIB_POSE_INDEX,"a_pose_index");

        glLinkProgram(program);
        glGetProgramiv(program,GL_LINK_STATUS,&linkStatus);
        
        if(linkStatus != 0){
            MODELVIEW_MATRIX 	= glGetUniformLocation(program,"modelview_matrix");
            PROJECTION_MATRIX 	= glGetUniformLocation(program,"projection_matrix");
            TEXTURE_0 			= glGetUniformLocation(program,"texture0");
            AMBIENT				= glGetUniformLocation(program,"ambient");
            DIFFUSE				= glGetUniformLocation(program,"diffuse");
            SPECULAR			= glGetUniformLocation(program,"specular");
            EMISSIVE			= glGetUniformLocation(program,"emissive");
            TEXTURE_LOCATION 	= glGetUniformLocation(program,"texture_location");
            IMAGE_SIZE 			= glGetUniformLocation(program,"image_size");

            char pose_matrix_index_str[32];
            for(int i=0;i<MAX_BONES;i++){
                sprintf(pose_matrix_index_str,"pose_matrix[%d]",i);
                POSE_MATRIX[i] 	= glGetUniformLocation(program,pose_matrix_index_str);
            }

            int err = glGetError();
            if(err !=0){
                logger::warn("Shader::Shader -> got GLError %d binding shader.",err);
                return false;
            }
        }
        else{//Linker error
            char* error_info_log = (char*)malloc(MAX_INFO_LOG_LENGTH);
            int error_info_log_length = 0;

            glGetProgramInfoLog(program,MAX_INFO_LOG_LENGTH,&error_info_log_length,error_info_log);
            error_info_log[error_info_log_length]=0;

            logger::warn("Shader::Shader -> GLSL Linker Error: %n%s",error_info_log);
            free(error_info_log);
            return false;
            }
    }
    else{//Compile Error
            char* error_info_log = (char*)malloc(MAX_INFO_LOG_LENGTH);
            int error_info_log_length = 0;

            glGetShaderInfoLog(vertex,MAX_INFO_LOG_LENGTH,&error_info_log_length,error_info_log);
            error_info_log[error_info_log_length]=0;
            logger::info("Vertex Shader:%n%s",error_info_log);

            glGetShaderInfoLog(fragment,MAX_INFO_LOG_LENGTH,&error_info_log_length,error_info_log);
            error_info_log[error_info_log_length]=0;
            logger::info("Fragment Shader:%n%s",error_info_log);

            logger::warn("Shader::Shader -> GLSL Compile Error.");
            free(error_info_log);
            return false;
    }
    return true;
}

void Shader::Unload(){
    glDetachShader(program,vertex);
    glDetachShader(program,fragment);
    glDeleteShader(vertex);
    glDeleteShader(fragment);
    glDeleteProgram(program);
}

void Shader::Use(){
    glUseProgram(program);
}


void ShaderManager::Init(){ 
    bool success = defaultShader.Load("dat/gfx/blank.vrt","dat/gfx/blank.frg");
    if(!success){
        logger::fatal("ShaderManager::DefaultShader -> Failed to initialize default shader!");
    }
}

void ShaderManager::AddShader(char* name, char* vertexFile,char* fragmentFile){
    if(cachedShaders.StrGet(name)!=null)return;
    Shader* newshader = new Shader();
    newshader->Load(vertexFile,fragmentFile);
    cachedShaders.Add((byte*)name,(byte*)newshader);
}

Shader* ShaderManager::GetShader(char* name){
    Shader* ret = (Shader*)cachedShaders.StrGet(name);
    if(ret == null){
        ret = DefaultShader();
    }
    return ret;
}
void ShaderManager::RemoveShader(char* name){
    Shader* ret = (Shader*)cachedShaders.StrGet(name);
    if(ret != null){
        ret->Unload();
    }
    cachedShaders.StrRemove(name);
}
Shader* ShaderManager::DefaultShader(){
    return &defaultShader;
}