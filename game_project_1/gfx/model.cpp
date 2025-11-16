#include <game_project_1/gfx/model.hpp>
#include <game_project_1/io/asset_manager.hpp>
#include <game_project_1/io/stream.hpp>
#include <game_project_1/io/gltf.hpp>
#include <game_project_1/types/pool.hpp>
#include <game_project_1/io/log.hpp>
#include <game_project_1/gfx/primitive_shapes.h>
#include <string.h>


Pool<ModelCacheEntry> model_registry;
ModelData empty_model;

Mesh::Mesh(){
    tri_count=0;
    vertex_count=0;
};

void Mesh::Init(){
    ShaderManager::UseShader((ShaderRef)ShaderDef::MODEL_DYNAMIC_LIGHTING);
    glGenVertexArrays(1,&vertex_array_id);
    glBindVertexArray(vertex_array_id);
    CheckForGLError("GL error initializing mesh(1): %d\n");

    glEnableVertexAttribArray(Shader::ATTRIB_VERTEX);
    glEnableVertexAttribArray(Shader::ATTRIB_TEXCOORD);
    glEnableVertexAttribArray(Shader::ATTRIB_NORMAL);
    glEnableVertexAttribArray(Shader::ATTRIB_BONE_INDEX);
    glEnableVertexAttribArray(Shader::ATTRIB_BONE_WEIGHT);
    glEnableVertexAttribArray(Shader::ATTRIB_COLOR);


    CheckForGLError("GL error initializing mesh(2): %d\n");
    //TODO: Memory leak. Where to keep these values?
    if(!normal.Valid()){
        float* default_normals = (float*)malloc(vertex_count*3*3*sizeof(float));
        for(int i=0;i<vertex_count*3*3;i++){default_normals[i]=1.0f;}
        normal.Create(default_normals,GL_FLOAT,3,vertex_count*3,GL_ARRAY_BUFFER);
    }
    if(!texcoord_0.Valid()){ 
        float* default_texcoord = (float*)malloc(vertex_count*3*2*sizeof(float));
        for(int i=0;i<vertex_count*3*2;i++){default_texcoord[i]=0.0f;}
        texcoord_0.Create(default_texcoord,GL_FLOAT,2,vertex_count*3,GL_ARRAY_BUFFER);
    }
    if(!vertex_colors.Valid()){
        float* default_colors = (float*)malloc(vertex_count*3*3*sizeof(float));
        for(int i=0;i<vertex_count*3*3;i++){default_colors[i]=1.0f;}
        vertex_colors.Create(default_colors,GL_FLOAT,3,vertex_count*3,GL_ARRAY_BUFFER);
    }
    if(!bone_0_index.Valid()){ 
        short* default_bone_index = (short*)malloc(vertex_count*3*4*sizeof(short));
        for(int i=0;i<vertex_count*3*4;i++){default_bone_index[i]=0;}
        bone_0_index.Create(default_bone_index,GL_UNSIGNED_SHORT,4,vertex_count*3,GL_ARRAY_BUFFER);
    }
    if(!bone_0_weight.Valid()){
        float* default_bone_weight = (float*)malloc(vertex_count*3*4*sizeof(float));
        for(int i=0;i<vertex_count*3*4;i++){default_bone_weight[i]=0.0f;}
        bone_0_weight.Create(default_bone_weight,GL_FLOAT,4,vertex_count*3,GL_ARRAY_BUFFER);
    }
    
    
    CheckForGLError("GL error initializing mesh(3): %d\n");
    vertex.Bind(Shader::ATTRIB_VERTEX);
    texcoord_0.Bind(Shader::ATTRIB_TEXCOORD);
    normal.Bind(Shader::ATTRIB_NORMAL);
    bone_0_index.Bind(Shader::ATTRIB_BONE_INDEX);
    bone_0_weight.Bind(Shader::ATTRIB_BONE_WEIGHT);
    vertex_colors.Bind(Shader::ATTRIB_COLOR);


    CheckForGLError("GL error initializing mesh(4): %d\n");
    if(index.Valid()){
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index.buffer_id);
    }
    else{ 
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    }

    glBindVertexArray(0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    
    CheckForGLError("GL error initializing mesh(5). Error during index bind: %d\n");
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

void Mesh::Draw(Shader* shader,MeshGroupRenderOptions* mgro){
    glBindVertexArray(vertex_array_id);
    CheckForGLError("Mesh.Draw: GL Error %d binding vertex array\n");

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D,mat.texture.atlas_id);
    glUniform1i(shader->TEXTURE_0,0);

    glUniform4fv(shader->TEXTURE_LOCATION,1,(GLfloat*)&mat.texture.tex_coords);
    color_f base_color = mat.base_color.mult(mgro->color);
    glUniform4fv(shader->COLOR,1,(GLfloat*)&mat.base_color);
 
    if(index.Valid()){
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index.buffer_id);
        glDrawElements(GL_TRIANGLES,tri_count*3,index.element_type,nullptr);
    }
    else{
        glDrawArrays(GL_TRIANGLES,0,vertex_count);
    } 

    CheckForGLError("Mesh.Draw: GL Error %d during draw\n");
}

MeshGroup::MeshGroup():meshes(){
    name=null;
}

MeshGroup::~MeshGroup(){
    DEALLOCATE(name)
    meshes.Destroy();
}

void MeshGroup::DebugPrint(){
    logger::info("..Meshes: %d\n",meshes.length);
    for(int i=0; i< meshes.length;i++){
        logger::info("..Mesh %d:\n",i);
        meshes[i]->DebugPrint();
    }
}


MeshGroupRenderOptions::MeshGroupRenderOptions():color(1.0f,1.0f,1.0f,1.0f){
    hide=false;
}

ModelData::ModelData():mesh_groups(){
	skeleton=null;
}

ModelData::~ModelData(){
    if(skeleton != null){
        delete skeleton;
        skeleton = null;
    }
    mesh_groups.Destroy();
}

void ModelData::DebugPrint(){
    logger::info("Model printout:---------------\n");
    logger::info("Mesh Groups: %d\n",mesh_groups.length);
    for(int i=0; i< mesh_groups.length;i++){
        logger::info("Mesh Group %d:\n",i);
        mesh_groups[i]->DebugPrint();
    }
    if(skeleton != null){
        logger::info("Skeleton:\n");
        skeleton->DebugPrint();
    }

    logger::info("End model printout:---------------\n");
}

Model::Model(Transform* parent,ModelRef type):Drawable(parent){
    shader = (ShaderRef)ShaderDef::MODEL_DYNAMIC_LIGHTING;
    type = type;
    data = ModelManager::Use(type);
    mgro.Init(data->mesh_groups.length);
    if(data->skeleton != null){
        pose = new Pose(&offset,data->skeleton);
    }
    else{
        pose = null;
    }
}

Model::Model(Transform* parent,ModelData* dat):Drawable(parent){//Does not use cache. 
    shader = (ShaderRef)ShaderDef::MODEL_DYNAMIC_LIGHTING;
    type=0;
    data = dat;
    mgro.Init(data->mesh_groups.length);
    if(data->skeleton != null){
        pose = new Pose(&offset,data->skeleton);
    }
    else{
        pose = null;
    }
}

Model::~Model(){
    ModelManager::Return(type);
    data = null;
    if(pose != null){
        delete pose;
        pose = null;
    }
}

void Model::Draw(Camera* cam){
    Shader* s = ShaderManager::UseShader(shader);
    
    mat4 model;
    mat3 normal;
    mat4 identity;
    int bones=(pose->bone_count < Shader::MAX_BONES)?pose->bone_count:Shader::MAX_BONES;

    identity.identity();

    model.identity();
    offset.ApplyTo(model);
    //model.translate(x,y,z);
    //model.rotate(rotation);
    //model.scale(scale);
    mat4 modelview = cam->ViewMatrix();
    modelview.multiply_by(&model);

    normal.set(&modelview);
    normal.transpose();
    normal.invert();

    mat4 projection = cam->ProjectionMatrix();
    
    if(pose != null){
        pose->Calculate(); 
        for(int i=0;i< bones;i++){
            glUniformMatrix4x3fv(s->POSE_MATRICES+i,1,true,(GLfloat*)&pose->matrices[i]);
        } 
    }

    glUniformMatrix4fv(s->MODELVIEW_MATRIX,1,true,(GLfloat*)&modelview);
    glUniformMatrix4fv(s->PROJECTION_MATRIX,1,true,(GLfloat*)&projection);
    glUniformMatrix3fv(s->NORMAL_MATRIX,1,true,(GLfloat*)&normal);

    int i=0;
    for(MeshGroup* group:data->mesh_groups){
        for(Mesh* mesh:group->meshes){
            if(!mgro[i]->hide){
                mesh->Draw(s,mgro[i]);
            }
        }
        i++;
    }

    if(pose != null){
        for(int i=0;i< bones;i++){ 
            glUniformMatrix4x3fv(s->POSE_MATRICES+i,1,true,(GLfloat*)&identity);
        }
    }
}

void Model::StartAnimation(char* anim_name){
    if(pose != null){pose->StartAnimation(anim_name);}
}

void Model::StopAnimations(){
    if(pose != null){pose->StopAnimations();}
}

void Model::QueueAnimation(char* anim_name){
    if(pose == null){return;}
    pose->QueueAnimation(anim_name);
}

void ModelManager::Init(){
    ShapePrimitive error_cube(EPrimitiveShape::CUBE,"./dat/img/error.png",1,1,1);

    empty_model.mesh_groups.length=0;
    empty_model.mesh_groups.data=nullptr;
    empty_model.skeleton= null;
    empty_model.bounds = {{-0.5f,-0.5f,-0.5f},{0.5f,0.5f,0.5f}};
    empty_model.mesh_groups.Init(1);
    MeshGroup* empty_mesh_group = empty_model.mesh_groups[0];
        empty_mesh_group->meshes.Init(1);
        empty_mesh_group->name = cstr::new_copy("ErrorModel.MeshGroup");
    Mesh* empty_mesh = empty_mesh_group->meshes[0];
        empty_mesh->vertex_count = error_cube.vertex_count;
        empty_mesh->tri_count = empty_mesh->vertex_count/3;
        empty_mesh->mat = error_cube.mat;
        empty_mesh->vertex = error_cube.vertices;
        empty_mesh->texcoord_0 = error_cube.tex_coords;
        empty_mesh->normal = error_cube.normals;
}


void ModelManager::Free(){
    for(ModelCacheEntry* entry:model_registry){
        SAFE_DELETE(entry->data)
        //if(entry->uri){free(entry->uri);entry->uri=null;}
    }
    model_registry.Clear();
}

ModelData* ModelManager::Use(ModelRef id){
    if(id < 0){return ErrorModel();}
    for(ModelCacheEntry* cache:model_registry){
        if(cache->id == id){
            if(cache->data==null){
                Stream* model_stream = AssetManager::Model(cache->uri);
                if(!model_stream->error){
                    GLTFScene scene(model_stream);
                    cache->data = scene.Load();
                    delete model_stream;
                    return cache->data;
                }
                else{
                    delete model_stream;
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

void ModelManager::Return(ModelRef id){
    if(id < 0){return;}
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

void ModelManager::Register(ModelRef id, char* uri){
    ModelCacheEntry* cache = model_registry.Add();
    cache->data=null;
    cache->uri=uri;
    cache->users=0;
    cache->id=id;
}

ModelRef ModelManager::GetByName(char* name){
    for(ModelCacheEntry* cache:model_registry){
        if(cstr::compare(name,cache->uri)){return cache->id;}
    }
    return -1;
}

void ModelManager::Unregister(ModelRef id){
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

