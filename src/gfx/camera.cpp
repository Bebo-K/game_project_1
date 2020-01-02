#include "camera.h"
#include "../io/log.h"

#define SHADER_MAX_INFO_LOG_LENGTH 1024

Camera::Camera(){
    x = 0.0f;
    y = 0.0f;
    z = 0.0f;
    rotation = vec3();

    ortho = true;
    width = 600.0f;
    height = 400.0f;
    fov = 60.0f;
    near_clip = 1.0f;
    far_clip = 100.0f;
    shader = new Shader();
}

void Camera::ToCameraSpace(mat4* m){
    if(rotation.x != 0){m->rotate_x(rotation.x);}
    if(rotation.y != 0){m->rotate_y(rotation.y);}
    if(rotation.z != 0){m->rotate_z(rotation.z);}
    m->translate(-x,-y,-z);
}
vec3 Camera::ToWorldSpace(vec3 v){
    v.rotate_y(rotation.y);
    return v;
}

Shader::Shader(){
    Shader("dat/gfx/blank.vrt","dat/gfx/blank.frg");
}

Shader::Shader(const char* vertexFile,const char* fragmentFile){

    FileBuffer vertexProgram(vertexFile);
    FileBuffer fragmentProgram(fragmentFile);

    GLuint vertex_handle = glCreateShader(GL_VERTEX_SHADER);
    GLuint fragment_handle = glCreateShader(GL_FRAGMENT_SHADER);
    program = glCreateProgram();
    glShaderSource(vertex_handle, 1, (GLchar**)&vertexProgram.contents,nullptr);
    glShaderSource(fragment_handle, 1, (GLchar**)&fragmentProgram.contents,nullptr);

    glCompileShader(vertex_handle);
    glCompileShader(fragment_handle);

    GLint vtxCompileStatus = 0;
    GLint frgCompileStatus = 0;
    GLint linkStatus = 0;
    glGetShaderiv(vertex_handle,GL_COMPILE_STATUS,&vtxCompileStatus);
    glGetShaderiv(fragment_handle,GL_COMPILE_STATUS,&frgCompileStatus);
    
    if(vtxCompileStatus != 0 && frgCompileStatus != 0){
        glAttachShader(program,vertex_handle);
        glAttachShader(program,fragment_handle);
        
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
            }
        }
        else{//Linker error
            char* error_info_log = (char*)malloc(SHADER_MAX_INFO_LOG_LENGTH);
            int error_info_log_length = 0;

            glGetProgramInfoLog(program,SHADER_MAX_INFO_LOG_LENGTH,&error_info_log_length,error_info_log);
            error_info_log[error_info_log_length]=0;

            logger::warn("Shader::Shader -> GLSL Linker Error: %n%s",error_info_log);
            free(error_info_log);
            }
    }
    else{//Compile Error
            char* error_info_log = (char*)malloc(SHADER_MAX_INFO_LOG_LENGTH);
            int error_info_log_length = 0;

            glGetShaderInfoLog(vertex_handle,SHADER_MAX_INFO_LOG_LENGTH,&error_info_log_length,error_info_log);
            error_info_log[error_info_log_length]=0;
            logger::info("Vertex Shader:%n%s",error_info_log);

            glGetShaderInfoLog(fragment_handle,SHADER_MAX_INFO_LOG_LENGTH,&error_info_log_length,error_info_log);
            error_info_log[error_info_log_length]=0;
            logger::info("Fragment Shader:%n%s",error_info_log);

            logger::warn("Shader::Shader -> GLSL Compile Error.");
            free(error_info_log);
    }
}

void Shader::Use(){
    glUseProgram(program);
}