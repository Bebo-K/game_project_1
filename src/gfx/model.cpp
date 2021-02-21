#include "model.h"
#include <string.h>
#include "../log.h"
#include "../io/gltf.h"
#include "../structs/list.h"

List<ModelCacheEntry> model_registry;
ModelData empty_model;

Mesh::Mesh(){
    vertex_count=0;
};

void Mesh::Init(){
    glGenVertexArrays(1,&vertex_array_id);
    glBindVertexArray(vertex_array_id);

    glEnableVertexAttribArray(Shader::ATTRIB_VERTEX);
    glEnableVertexAttribArray(Shader::ATTRIB_TEXCOORD);
    glEnableVertexAttribArray(Shader::ATTRIB_NORMAL);
    glEnableVertexAttribArray(Shader::ATTRIB_BONE_INDEX);
    glEnableVertexAttribArray(Shader::ATTRIB_BONE_WEIGHT);
    glEnableVertexAttribArray(Shader::ATTRIB_COLOR);

    float default_normal[]={0,0,-1};
    float default_vert_color[]={1,1,1};
    float default_texcoord[]={0,0};
    int default_bone_index = 0;
    float default_bone_weight = 0;

    if(!normal.Valid()){
                normal.Create(default_normal,GL_FLOAT,3,1);}
    if(!texcoord_0.Valid()){ 
           texcoord_0.Create(default_texcoord,GL_FLOAT,2,1);}
    if(!vertex_colors.Valid()){
        //TODO:same process for the rest of the defaults. Also, where to keep these values? Also, any way to not waste memory like this?
        float* default_colors = (float*)malloc(vertex_count*sizeof(float)*3);
        for(int i=0;i<vertex_count;i++){memcpy(&default_colors[i*3],default_vert_color,sizeof(float)*3);}
         vertex_colors.Create(default_colors,GL_FLOAT,3,vertex_count);}
    if(!bone_0_index.Valid()){ 
         bone_0_index.Create(&default_bone_index,GL_INT,1,1);}
    if(!bone_0_weight.Valid()){
         bone_0_weight.Create(&default_bone_weight,GL_FLOAT,1,1);}

    vertex.Bind(Shader::ATTRIB_VERTEX);
    texcoord_0.Bind(Shader::ATTRIB_TEXCOORD);
    normal.Bind(Shader::ATTRIB_NORMAL);
    bone_0_index.Bind(Shader::ATTRIB_BONE_INDEX);
    bone_0_weight.Bind(Shader::ATTRIB_BONE_WEIGHT);
    vertex_colors.Bind(Shader::ATTRIB_COLOR);

    if(index.Valid()){
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index.buffer_id);
    }

    glBindVertexArray(0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

Mesh::~Mesh(){
    vertex.Destroy();
    index.Destroy();
    vertex_colors.Destroy();
    texcoord_0.Destroy();
    normal.Destroy();
    bone_0_index.Destroy();
    bone_0_weight.Destroy();
    glDeleteVertexArrays(1,&vertex_array_id);
};

MeshGroup::MeshGroup(){
    meshes=null;
    mesh_count=0;
    name=null;
}

MeshGroup::~MeshGroup(){
    if(name != null){
        free(name);
        name=null;
    }
    if(meshes != null){
        delete[] meshes;
        meshes = null;
        mesh_count=0;
    }
}

ModelData::ModelData(){
    mesh_group_count=0;
	mesh_groups=null;
	skeleton=null;
}

ModelData::~ModelData(){
    if(skeleton != null){
        delete skeleton;
        skeleton = null;
    }
    if(mesh_groups != null){
        delete[] mesh_groups;
        mesh_groups=null;
        mesh_group_count=0;
    }
}

void ModelData::DrawMesh(Camera* cam,int group_index,int mesh_index){
    if(group_index < 0 || group_index > mesh_group_count){return;}
    if(mesh_index < 0 || mesh_index > mesh_groups[group_index].mesh_count){return;}
    
    Mesh* m = &mesh_groups[group_index].meshes[mesh_index];
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D,m->mat.texture.atlas_id);
    glUniform1i(cam->shader->TEXTURE_0,0);
    glUniform4fv(cam->shader->TEXTURE_LOCATION,1,(GLfloat*)&m->mat.texture.tex_coords);
    glUniform4fv(cam->shader->COLOR,1,(GLfloat*)&m->mat.base_color);
    glBindVertexArray(m->vertex_array_id);
    
    if(m->index.Valid()){
       ///glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m->index.buffer_id);
        glDrawElements(GL_TRIANGLES,m->vertex_count,m->index.element_type,nullptr);
    }
    else{
        glDrawArrays(GL_TRIANGLES,0,m->vertex_count);
    } 
    
    int err = glGetError(); 
    if(err != 0){
        logger::warn("ModelData.DrawMesh() -> GL Error: %d \n",err);
    }
}

Model::Model(ModelID type){
    type_id = type;
    data = ModelManager::Use(type);
    if(data->skeleton != null){
        pose = new Pose(data->skeleton);
    }
    else{
        pose = null;
    }
}

Model::Model(ModelData* dat){//Does not use cache. 
    type_id= NONE;
    data = dat;
    if(data->skeleton != null){
        pose = new Pose(data->skeleton);
    }
    else{
        pose = null;
    }
}

Model::~Model(){
    ModelManager::Return(type_id);
    data = null;
    if(pose != null){
        delete pose;
        pose = null;
    }
}

void Model::Draw(Camera* cam,mat4* view, mat4* projection){
    cam->SetShader("basic_lighting");
    glEnableVertexAttribArray(Shader::ATTRIB_VERTEX);
    glEnableVertexAttribArray(Shader::ATTRIB_TEXCOORD);
    glEnableVertexAttribArray(Shader::ATTRIB_NORMAL);
    glEnableVertexAttribArray(Shader::ATTRIB_BONE_INDEX);
    glEnableVertexAttribArray(Shader::ATTRIB_BONE_WEIGHT);
    glEnableVertexAttribArray(Shader::ATTRIB_COLOR);
    
    mat4 model;
    mat3 normal;
    mat4 identity;
    int bones=(pose->bone_count < Shader::MAX_BONES)?pose->bone_count:Shader::MAX_BONES;

    identity.identity();

    model.identity();
    model.translate(x,y,z);
    model.rotate(rotation);
    model.scale(scale);
    view->multiply_by(&model);

    normal.set(view);
    normal.transpose();
    normal.invert();
    
    if(pose != null){
        pose->Calculate(); 
        
        for(int i=0;i< bones;i++){
            glUniformMatrix4x3fv(cam->shader->POSE_MATRICES+i,1,true,(GLfloat*)&pose->matrices[i]);
        } 
    }

    glUniformMatrix4fv(cam->shader->MODELVIEW_MATRIX,1,true,(GLfloat*)view);
    glUniformMatrix4fv(cam->shader->PROJECTION_MATRIX,1,true,(GLfloat*)projection);
    glUniformMatrix3fv(cam->shader->NORMAL_MATRIX,1,true,(GLfloat*)&normal);

    for(int i=0;i<data->mesh_group_count;i++){
        for(int j=0;j<data->mesh_groups[i].mesh_count;j++){
            data->DrawMesh(cam,i,j);
        }
    }

    if(pose != null){
        for(int i=0;i< bones;i++){ 
            glUniformMatrix4x3fv(cam->shader->POSE_MATRICES+i,1,true,(GLfloat*)&identity);
        }
    }


    //glDisableVertexAttribArray(Shader::ATTRIB_COLOR);
    glDisableVertexAttribArray(Shader::ATTRIB_BONE_WEIGHT);
    glDisableVertexAttribArray(Shader::ATTRIB_BONE_INDEX);
    glDisableVertexAttribArray(Shader::ATTRIB_NORMAL);
    glDisableVertexAttribArray(Shader::ATTRIB_TEXCOORD);
    glDisableVertexAttribArray(Shader::ATTRIB_VERTEX);
    glBindVertexArray(0);
}

void Model::StartAnimation(char* anim_name){
    if(pose != null){pose->StartAnimation(anim_name);}
}

void Model::StartAnimation(char* anim_name, AnimationOptions options){
    if(pose != null){pose->StartAnimation(anim_name,options);}
}

void ModelManager::Init(){
    ShapePrimitive error_cube(EPrimitiveShape::CUBE,"./dat/img/error.png",1,1,1);

    empty_model.skeleton= null;
    empty_model.bounds = {{-0.5f,-0.5f,-0.5f},{0.5f,0.5f,0.5f}};
    empty_model.mesh_groups=new MeshGroup[1]();
    empty_model.mesh_group_count=1;
    empty_model.mesh_groups[0].meshes = new Mesh[1]();
    empty_model.mesh_groups[0].name = cstr::new_copy("ErrorModel.MeshGroup");
    empty_model.mesh_groups[0].meshes[0].vertex_count = error_cube.vertex_count;
    empty_model.mesh_groups[0].meshes[0].mat = error_cube.mat;
    empty_model.mesh_groups[0].meshes[0].vertex = error_cube.vertices;
    empty_model.mesh_groups[0].meshes[0].texcoord_0 = error_cube.tex_coords;
    empty_model.mesh_groups[0].meshes[0].normal = error_cube.normals;
}


void ModelManager::Free(){
    for(ModelCacheEntry* entry:model_registry){
        if(entry->data){delete entry->data;entry->data=null;}
        //if(entry->filename){free(entry->filename);entry->filename=null;}
    }
    model_registry.Clear();
}

ModelData* ModelManager::Use(ModelID id){
    if(id == NONE){return ErrorModel();}
    for(ModelCacheEntry* cache:model_registry){
        if(cache->id == id){
            if(cache->data==null){
                if(File::Exists(cache->filename)){
                    File model_file(cache->filename);
                    GLTFScene scene(model_file);
                    cache->data = scene.Load();
                    model_file.close();
                    return cache->data;
                }
                else{
                    return ErrorModel();
                }
            }
            else{
                cache->users++;
                return cache->data;
            }
        }
    }
    return ErrorModel();
}

void ModelManager::Return(ModelID id){
    if(id == NONE){return;}
    for(ModelCacheEntry* cache:model_registry){
        if(cache->id == id){
            cache->users--;
        }
    }
}

void ModelManager::Clean(){//deletes all model data not in use.
    for(ModelCacheEntry* cache:model_registry){
        if(cache->users <= 0){
            cache->users =0;
            delete cache->data;
            cache->data = null;
        }
    }
}

void ModelManager::Register(ModelID id, const char* filename){
    ModelCacheEntry* cache = model_registry.Add();
    cache->data=null;
    cache->filename=filename;
    cache->users=0;
    cache->id=id;
}

void ModelManager::Unregister(ModelID id){
    for(ModelCacheEntry* cache:model_registry){
        if(cache->id== id){
            if(cache->users != 0){
                logger::warn("Warning, deregistering model of type %d even though it's in use!\n",id);
            }
            delete cache->data;
            model_registry.Delete(cache);
            return;
        }
    }
}

ModelData* ModelManager::ErrorModel(){
    return &empty_model;
}