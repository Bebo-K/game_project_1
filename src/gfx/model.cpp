#include "model.h"
#include <string.h>
#include "../log.h"
#include "../io/gltf.h"
#include "../struct/list.h"

List<ModelCacheEntry> model_registry;
ModelData empty_model;

Mesh::Mesh(){
    tri_count=0;
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

    //TODO: Memory leak. Where to keep these values?
    if(!normal.Valid()){
        float* default_normals = (float*)malloc(vertex_count*3*3*sizeof(float));
        for(int i=0;i<vertex_count*3*3;i++){default_normals[i]=1.0f;}
        normal.Create(default_normals,GL_FLOAT,3,vertex_count*3);
    }
    if(!texcoord_0.Valid()){ 
        float* default_texcoord = (float*)malloc(vertex_count*3*2*sizeof(float));
        for(int i=0;i<vertex_count*3*2;i++){default_texcoord[i]=0.0f;}
        texcoord_0.Create(default_texcoord,GL_FLOAT,2,vertex_count*3);
    }
    if(!vertex_colors.Valid()){
        float* default_colors = (float*)malloc(vertex_count*3*3*sizeof(float));
        for(int i=0;i<vertex_count*3*3;i++){default_colors[i]=1.0f;}
        vertex_colors.Create(default_colors,GL_FLOAT,3,vertex_count*3);
    }
    if(!bone_0_index.Valid()){ 
        short* default_bone_index = (short*)malloc(vertex_count*3*4*sizeof(short));
        for(int i=0;i<vertex_count*3*4;i++){default_bone_index[i]=0;}
        bone_0_index.Create(default_bone_index,GL_UNSIGNED_SHORT,4,vertex_count*3);
    }
    if(!bone_0_weight.Valid()){
        float* default_bone_weight = (float*)malloc(vertex_count*3*4*sizeof(float));
        for(int i=0;i<vertex_count*3*4;i++){default_bone_weight[i]=0.0f;}
        bone_0_weight.Create(default_bone_weight,GL_FLOAT,4,vertex_count*3);
    }
    
    vertex.Bind(Shader::ATTRIB_VERTEX);
    texcoord_0.Bind(Shader::ATTRIB_TEXCOORD);
    normal.Bind(Shader::ATTRIB_NORMAL);
    bone_0_index.Bind(Shader::ATTRIB_BONE_INDEX);
    bone_0_weight.Bind(Shader::ATTRIB_BONE_WEIGHT);
    vertex_colors.Bind(Shader::ATTRIB_COLOR);

    if(index.Valid()){
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index.buffer_id);
    }
    else{ 
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    }

    glBindVertexArray(0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    
    int gl_err = glGetError();
    if(gl_err != 0){logger::warn("GL error initializing mesh. Error during index bind: %d",&gl_err);}
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

void Mesh::DebugPrint(){
    logger::info("....Triangle Count: %d\n",tri_count);
    logger::info("....Vertex Count: %d\n",vertex_count);
    logger::info("....Vertex Array ID: %d\n",vertex_array_id);
    logger::info("....Vertices ID: %d\n",vertex.buffer_id);
    logger::info("....TexCoords ID: %d\n",texcoord_0.buffer_id);
    logger::info("....Normals ID: %d\n",normal.buffer_id);
    logger::info("....Colors ID: %d\n",vertex_colors.buffer_id);
    logger::info("....Bone0 indices ID: %d\n",bone_0_index.buffer_id);
    logger::info("....Bone0 weight ID: %d\n",bone_0_weight.buffer_id);
    logger::info("....Texture Material ID: %d\n",mat.texture.atlas_id);
    logger::info("....Indices ID: %d\n",index.buffer_id);
}

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

void MeshGroup::DebugPrint(){
    logger::info("..Meshes: %d\n",mesh_count);
    for(int i=0; i< mesh_count;i++){
        logger::info("..Mesh %d:\n",i);
        meshes[i].DebugPrint();
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
    if(group_index < 0 || group_index >= mesh_group_count){return;}
    if(mesh_index < 0 || mesh_index >= mesh_groups[group_index].mesh_count){return;}
    
    Mesh* m = &mesh_groups[group_index].meshes[mesh_index];
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D,m->mat.texture.atlas_id);
    glUniform1i(cam->shader->TEXTURE_0,0);
    glUniform4fv(cam->shader->TEXTURE_LOCATION,1,(GLfloat*)&m->mat.texture.tex_coords);
    glUniform4fv(cam->shader->COLOR,1,(GLfloat*)&m->mat.base_color);
    glBindVertexArray(m->vertex_array_id);
    
    if(m->index.Valid()){
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m->index.buffer_id);
        glDrawElements(GL_TRIANGLES,m->tri_count*3,m->index.element_type,nullptr);
    }
    else{
        glDrawArrays(GL_TRIANGLES,0,m->vertex_count);
    } 
    
    glBindVertexArray(0);
    
    int err = glGetError(); 
    if(err != 0){
        logger::warn("ModelData.DrawMesh() -> GL Error: %d \n",err);
    }
}

void ModelData::DebugPrint(){
    logger::info("Model printout:---------------\n");
    logger::info("Mesh Groups: %d\n",mesh_group_count);
    for(int i=0; i< mesh_group_count;i++){
        logger::info("Mesh Group %d:\n",i);
        mesh_groups[i].DebugPrint();
    }
    if(skeleton != null){
        logger::info("Skeleton:\n");
        skeleton->DebugPrint();
    }

    logger::info("End model printout:---------------\n");
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
    cam->SetShader("model_dynamic_lighting");
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
        int group_count = data->mesh_groups[i].mesh_count;
        for(int j=group_count-1;j>=0;j--){
            data->DrawMesh(cam,i,j);
        }
    }

    if(pose != null){
        for(int i=0;i< bones;i++){ 
            glUniformMatrix4x3fv(cam->shader->POSE_MATRICES+i,1,true,(GLfloat*)&identity);
        }
    }


    glDisableVertexAttribArray(Shader::ATTRIB_COLOR);
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
    empty_model.mesh_groups[0].meshes[0].tri_count = empty_model.mesh_groups[0].meshes[0].vertex_count/3;
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