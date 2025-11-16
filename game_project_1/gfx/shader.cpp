#include <game_project_1/gfx/shader.hpp>
#include <game_project_1/io/asset_manager.hpp>
#include <game_project_1/io/log.hpp>
#include <game_project_1/types/map.hpp>
#include <game_project_1/types/primitives.hpp>

ShaderRef CURRENT_SHADER=(ShaderRef)ShaderDef::DEFAULT;
Map<int,Shader> CACHED_SHADERS(2);


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
    enabled_attribs_mask=0;
    feature_flags=0;
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

        const char* vertex_attrib = "a_vertex";
        const char* texcoord_attrib = "a_tex_coord";
        const char* normal_attrib = "a_normal";
        const char* bone_index_attrib = "a_bone_index";
        const char* bone_weight_attrib = "a_bone_weight";
        const char* color_attrib = "a_color";
        
        glBindAttribLocation(program,ATTRIB_VERTEX,vertex_attrib);
        glBindAttribLocation(program,ATTRIB_TEXCOORD,texcoord_attrib);
        glBindAttribLocation(program,ATTRIB_NORMAL,normal_attrib);
        glBindAttribLocation(program,ATTRIB_BONE_INDEX,bone_index_attrib);
        glBindAttribLocation(program,ATTRIB_BONE_WEIGHT,bone_weight_attrib);
        glBindAttribLocation(program,ATTRIB_COLOR,color_attrib);

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

            CheckForGLError("Shader::Shader -> got GLError %d setting shader uniforms.\n");
            

            if(glGetAttribLocation(program,vertex_attrib) >= 0){SET_BIT(enabled_attribs_mask,ATTRIB_VERTEX);}
            if(glGetAttribLocation(program,texcoord_attrib) >= 0){SET_BIT(enabled_attribs_mask,ATTRIB_TEXCOORD);}
            if(glGetAttribLocation(program,normal_attrib) >= 0){SET_BIT(enabled_attribs_mask,ATTRIB_NORMAL);}
            if(glGetAttribLocation(program,bone_index_attrib) >= 0){SET_BIT(enabled_attribs_mask,ATTRIB_BONE_INDEX);}
            if(glGetAttribLocation(program,bone_weight_attrib) >= 0){SET_BIT(enabled_attribs_mask,ATTRIB_BONE_WEIGHT);}
            if(glGetAttribLocation(program,color_attrib) >= 0){SET_BIT(enabled_attribs_mask,ATTRIB_COLOR);}

            CheckForGLError("Shader::Shader -> got GLError %d enabling shader attribs.\n");
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
    CheckForGLError("Shader::Use-> GL error: %d \n");
}

void Shader::OnStartUse(){
    if(GET_BIT(feature_flags,FEATURE_DISABLE_DEPTH_TEST)){
        glDisable(GL_DEPTH_TEST);
        CheckForGLError("Shader::OnStartUse-> GL error: %d enabling feature \n");
    }

    for(int i=0;i<MAX_ATTRIBS;i++){
        if(GET_BIT(enabled_attribs_mask,i)){
            glEnableVertexAttribArray(i);
            CheckForGLError("Shader::OnStartUse-> GL error: %d enabling attrib \n");
        }
    }
    CheckForGLError("Shader::OnStartUse -> got GLError %d enabling shader features\n");
}

void Shader::OnFinishUse(){
    glBindVertexArray(0);
    CheckForGLError("Shader::OnFinishUse -> got GLError %d binding 0 attrib array\n");

    if(GET_BIT(feature_flags,FEATURE_DISABLE_DEPTH_TEST)){
        glEnable(GL_DEPTH_TEST);
        CheckForGLError("Shader::OnFinishUse -> got GLError %d disabling feature\n");
    }

    for(int i=0;i<MAX_ATTRIBS;i++){
        if(GET_BIT(enabled_attribs_mask,i)){
            glDisableVertexAttribArray(i);
            CheckForGLError("Shader::OnFinishUse -> got GLError %d disabling attrib\n");
        }
    }
}

void Shader::SetFeature(unsigned int feature_id,bool enabled){
    if(enabled){
        SET_BIT(feature_flags,feature_id);
    }
    else{
        CLEAR_BIT(feature_flags,feature_id);
    }
}

void ShaderManager::Init(){
    logger::debug("Loading default shaders..\n");
    AddShader((int)ShaderDef::DEFAULT,"default","default");
    AddShader((int)ShaderDef::LEVEL_DEBUG,"level_debug");
    AddShader((int)ShaderDef::SHAPE_DEBUG,"shape_debug");
    AddShader((int)ShaderDef::MODEL_DYNAMIC_LIGHTING,"model_dynamic_lighting");
    AddShader((int)ShaderDef::MODEL_STATIC_SHADELESS,"model_static_shadeless");
    AddShader((int)ShaderDef::SKYBOX,"skybox");
    AddShader((int)ShaderDef::SKYBOX_FLAT,"skybox_flat");
    AddShader((int)ShaderDef::UI_2D_QUAD,"ui_2d_quad")->SetFeature(Shader::FEATURE_DISABLE_DEPTH_TEST,true);
    AddShader((int)ShaderDef::UI_SHAPE,"ui_shape")->SetFeature(Shader::FEATURE_DISABLE_DEPTH_TEST,true);
    UseShader((int)ShaderDef::DEFAULT);
}

void ShaderManager::Free(){
    CACHED_SHADERS.Clear();
}

Shader* ShaderManager::AddShader(ShaderRef id_constant,char* name){
    AddShader(id_constant,name,name);
    logger::debug("Caching shader %s\n",name);
    return CACHED_SHADERS.Get(id_constant);
}
ShaderRef ShaderManager::RegisterShader(char* vertex_uri,char* fragment_uri){
    ShaderRef new_id = (ShaderRef)CACHED_SHADERS.Count();
    AddShader(new_id,vertex_uri,fragment_uri);
    return new_id;
}
Shader* ShaderManager::AddShader(ShaderRef id_constant,char* vertex_uri,char* fragment_uri){
    if(CACHED_SHADERS.Has(id_constant)){return CACHED_SHADERS.Get(id_constant);}
    return new (CACHED_SHADERS.Add(id_constant)) Shader(vertex_uri,fragment_uri);
}
ShaderRef ShaderManager::Get(char* name){
    //Remove calls to this function?
    return -1;
}

Shader* ShaderManager::UseShader(ShaderRef id){
    Shader* ret = GetShader(id);
    if(id != CURRENT_SHADER){
        GetShader(CURRENT_SHADER)->OnFinishUse();
        ret->Use();
        CURRENT_SHADER=id;
    }
    ret->OnStartUse();
    return ret;
}

Shader* ShaderManager::GetShader(ShaderRef id){
    Shader* ret = CACHED_SHADERS.Get((int)id);
    if(ret== nullptr){
        logger::warn("ShaderManager.GetShader: Shader id '%d' not found, returning default.",id);
        ret=CACHED_SHADERS.Get((ShaderRef)ShaderDef::DEFAULT);
    }
    return ret;
}
void ShaderManager::RemoveShader(ShaderRef id){
    if(CURRENT_SHADER == id){
        UseShader((ShaderRef)ShaderDef::DEFAULT);
    }
    Shader* ret = CACHED_SHADERS.Get(id);
    CACHED_SHADERS.Remove(id);
    if(ret != nullptr){
        delete ret;
    }   
}
bool ShaderManager::IsCurrentShader(ShaderRef id){return CURRENT_SHADER == id;}
ShaderRef ShaderManager::CurrentShader(){return CURRENT_SHADER;}
//Shader* GetShader(char* name);
//Shader* DefaultShader();
/*
Shader* ShaderManager::AddShader(char* name){
    if(CACHED_SHADERS.Has(name)){return CACHED_SHADERS.Get(name);}
    logger::debug("Caching shader %s\n",name);
    return new (CACHED_SHADERS.Add(name)) Shader(name,name);
}

Shader* ShaderManager::AddShader(char* name,char* vertex_uri,char* fragment_uri){
    if(CACHED_SHADERS.Has(name)){return CACHED_SHADERS.Get(name);}
    logger::debug("Caching shader %s\n",name);
    return new (CACHED_SHADERS.Add(name)) Shader(vertex_uri,fragment_uri);
}

Shader* ShaderManager::UseShader(char* name){
}

void ShaderManager::RemoveShader(char* name){
    Shader* ret = CACHED_SHADERS.Get(name);
    CACHED_SHADERS.Remove(name);
    if(ret != nullptr){
        delete ret;
    }   
}

bool ShaderManager::IsCurrentShader(char* name){
    if(CURRENT_SHADER==nullptr){return false;}
    Shader* ret = CACHED_SHADERS.Get(name);
    if(CURRENT_SHADER == ret){return true;}
    return false;
}

Shader* ShaderManager::CurrentShader(){return CURRENT_SHADER;}
*/