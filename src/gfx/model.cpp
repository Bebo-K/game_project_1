#include "model.h"
#include <string.h>
#include "../log.h"

AssociativeArray cached_models(8);
Model empty_model;


Model::Model(){
    name=null;
    mesh_count=0;
	meshes=null;
	skeleton=null;
}
Model::~Model(){
    if(skeleton != null){delete skeleton;}
}


void Model::DrawMesh(Camera* cam,Mesh* m){
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D,m->mat->texture.atlas_id);
    glUniform1i(cam->shader->TEXTURE_0,0);
    glUniform4fv(cam->shader->TEXTURE_LOCATION,1,(GLfloat*)&m->mat->texture.tex_coords);

    glUniform3fv(cam->shader->AMBIENT,1,(GLfloat*)&m->mat->base_color);
    glUniform3fv(cam->shader->DIFFUSE,1,(GLfloat*)&m->mat->base_color);
    glUniform3fv(cam->shader->SPECULAR,1,(GLfloat*)&m->mat->base_color);
    
    glBindBuffer(GL_ARRAY_BUFFER,m->vertex_buffer);
    glVertexAttribPointer(0,3,GL_FLOAT,false,0,0);

    glBindBuffer(GL_ARRAY_BUFFER,m->texcoord_0_buffer);
    glVertexAttribPointer(1,2,GL_FLOAT,false,0,0);

    glBindBuffer(GL_ARRAY_BUFFER,m->normal_buffer);
    glVertexAttribPointer(2,3,GL_FLOAT,false,0,0);
    
    if(m->index_buffer > 0){
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m->index_buffer);
        glDrawElements(GL_TRIANGLES,m->element_count,GL_UNSIGNED_SHORT,nullptr);
    }
    else{
        glDrawArrays(GL_TRIANGLES,0,m->element_count);
    } 
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


void DestroyMesh(Mesh* m){
    delete(m->mat);
    if(m->index_buffer > 0) {glDeleteBuffers(1,&m->index_buffer);}
    if(m->vertex_buffer > 0) {glDeleteBuffers(1,&m->vertex_buffer);}
    if(m->texcoord_0_buffer > 0) {glDeleteBuffers(1,&m->texcoord_0_buffer);}
    if(m->normal_buffer > 0) {glDeleteBuffers(1,&m->normal_buffer);}	
    if(m->bone_0_index_buffer > 0) {glDeleteBuffers(1,&m->bone_0_index_buffer);}	
    if(m->bone_0_weight_buffer > 0){glDeleteBuffers(1,&m->bone_0_weight_buffer);}
}

Model* Model::Clone(){
    Model* ret = new Model;

    ret->name=name;
    ret->mesh_count=mesh_count;
    ret->meshes=meshes;
    if(skeleton != null){
        ret->skeleton=skeleton->Clone();
    }
    ret->bounds = bounds;

    return ret;
}

void Model::DestroySharedData(){//Will break all instances of a model, be careful!
    free(name);
    if(meshes != null){
        for(int i=0;i<mesh_count;i++){DestroyMesh(&meshes[i]);}
        mesh_count=0;
        meshes=null;
    }
    if(skeleton != null){
        skeleton->DestroySharedData();
        delete(skeleton);
        skeleton=null;
    };
}

void ModelManager::Init(){ 
    ShapePrimitive error_cube(EPrimitiveShape::CUBE,"./dat/img/error.png",1,1,1);

    empty_model.x=empty_model.y=empty_model.z=0;
    empty_model.name="ErrorModel";
    empty_model.mesh_count=1;
    empty_model.meshes = (Mesh*)calloc(1,sizeof(Mesh));
    empty_model.meshes[0].index_buffer = 0;
    empty_model.meshes[0].element_count = error_cube.vertices;
    empty_model.meshes[0].mat = error_cube.mat;error_cube.mat=null;
    empty_model.meshes[0].vertex_buffer = error_cube.vertex_buffer;error_cube.vertex_buffer=0;
    empty_model.meshes[0].texcoord_0_buffer = error_cube.texcoord_buffer;error_cube.texcoord_buffer=0;
    empty_model.meshes[0].normal_buffer = error_cube.normal_buffer;error_cube.vertex_buffer=0;
}

void ModelManager::Add(const char* name, Model* model){
    cached_models.Add((byte*)name,(byte*)model);
}
Model* ModelManager::Get(const char* name){
    Model* ret = (Model*)cached_models.StrGet(name);
    if(ret == nullptr){ret = ErrorModel();}
    return ret->Clone();
}
void ModelManager::Remove(const char* name){
    Model* mdl = (Model*)cached_models.StrRemove(name);
    mdl->DestroySharedData();
    delete mdl;
}
Model* ModelManager::ErrorModel(){
    return &empty_model;
}