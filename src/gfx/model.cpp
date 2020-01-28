#include "model.h"
#include <string.h>

AssociativeArray cached_models(8);
Model empty_model;



Model::Model(){
    mesh_count=0;
    meshes=null;
}
Model::Model(int num_meshes){
    mesh_count=num_meshes;
    meshes=(Mesh*)malloc(sizeof(Mesh)*mesh_count);
}

Model::~Model(){
    free(meshes);
}

void Model::DrawMesh(Camera* cam,Mesh* m){
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D,m->mat->texture.atlas_id);
    glUniform1i(cam->shader->TEXTURE_0,0);
    glUniform4fv(cam->shader->TEXTURE_LOCATION,1,(GLfloat*)&m->mat->texture.tex_coords);

    glUniform3fv(cam->shader->AMBIENT,1,(GLfloat*)&m->mat->ambient);
    glUniform3fv(cam->shader->DIFFUSE,1,(GLfloat*)&m->mat->diffuse);
    glUniform3fv(cam->shader->SPECULAR,1,(GLfloat*)&m->mat->specular);
    
    glBindBuffer(GL_ARRAY_BUFFER,m->vertex_buffer);
    glVertexAttribPointer(0,3,GL_FLOAT,false,0,0);

    glBindBuffer(GL_ARRAY_BUFFER,m->texcoord_buffer);
    glVertexAttribPointer(1,2,GL_FLOAT,false,0,0);

    glBindBuffer(GL_ARRAY_BUFFER,m->normal_buffer);
    glVertexAttribPointer(2,3,GL_FLOAT,false,0,0);
    
    glDrawArrays(GL_TRIANGLES,0,m->element_count);
}

void Model::Draw(Camera* cam,mat4* view, mat4* projection){
    glEnableVertexAttribArray(cam->shader->ATTRIB_VERTEX);
    glEnableVertexAttribArray(cam->shader->ATTRIB_TEXCOORD);
    glEnableVertexAttribArray(cam->shader->ATTRIB_NORMAL);
    
    mat4 model;
    mat3 normal;

    model.identity();
    model.scale(scale);
    model.rotate(rotation);
    model.translate(x,y,z);
    view->multiply_by(&model);

    normal.set(view);
    normal.transpose();
    normal.invert();
    
    glUniformMatrix4fv(cam->shader->MODELVIEW_MATRIX,1,true,(GLfloat*)view);
    glUniformMatrix4fv(cam->shader->PROJECTION_MATRIX,1,true,(GLfloat*)projection);
    glUniformMatrix3fv(cam->shader->NORMAL_MATRIX,1,true,(GLfloat*)&normal);

    for(int i=0;i<mesh_count;i++){
        DrawMesh(cam,&meshes[i]);
    }

    glDisableVertexAttribArray(cam->shader->ATTRIB_TEXCOORD);
    glDisableVertexAttribArray(cam->shader->ATTRIB_VERTEX);
    glDisableVertexAttribArray(cam->shader->ATTRIB_NORMAL);
}

void ModelManager::Init(){ 
    //somehow init an error model.
}

Model* ModelManager::Add(const char* name, GLTFScene scene){
    Model* new_model = new Model();
    cached_models.Add((byte*)name,(byte*)new_model);
    return new_model;
}
Model* ModelManager::Get(const char* name){
    Model* ret = (Model*)cached_models.StrGet(name);
    if(ret == nullptr){ret = ErrorModel();}
    return ret;
}
void ModelManager::Remove(const char* name){
    Model* mdl = (Model*)cached_models.StrRemove(name);
    delete mdl;
}
Model* ModelManager::ErrorModel(){
    return &empty_model;
}

void ModelManager::BuildMesh(Mesh* dest,char* meshname,Material* m,int count,float* verts,float* texcoords, float* normals,int* bones){
    dest->name = cstr::new_copy(meshname);
    dest->mat = m;
    dest->element_count=count;

    glGenBuffers(1,&dest->vertex_buffer);
    glGenBuffers(1,&dest->texcoord_buffer);
    glGenBuffers(1,&dest->normal_buffer);
    glGenBuffers(1,&dest->bone_index_buffer);

    glBindBuffer(GL_ARRAY_BUFFER, dest->vertex_buffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float)*3*count, verts,GL_STATIC_DRAW);
    
    glBindBuffer(GL_ARRAY_BUFFER, dest->texcoord_buffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float)*2*count, texcoords,GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, dest->normal_buffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float)*3*count, normals,GL_STATIC_DRAW);

    //glBindBuffer(GL_ARRAY_BUFFER, bone_index_buffer);
    //glBufferData(GL_ARRAY_BUFFER, sizeof(int)*count, bones,GL_STATIC_DRAW);
}

void ModelManager::DeleteMesh(Mesh* m){
    free(m->name);
    delete(m->mat);
    if(m->vertex_buffer > 0) {glDeleteBuffers(1,&m->vertex_buffer);}
    if(m->texcoord_buffer > 0) {glDeleteBuffers(1,&m->texcoord_buffer);}
    if(m->normal_buffer > 0) {glDeleteBuffers(1,&m->normal_buffer);}	
    if(m->bone_index_buffer > 0) {glDeleteBuffers(1,&m->bone_index_buffer);}	
}