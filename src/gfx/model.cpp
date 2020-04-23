#include "model.h"
#include <string.h>
#include "../log.h"

AssociativeArray cached_models(8);
Model empty_model;


Model::Model(){
    name=null;
    mesh_group_count=0;
	mesh_groups=null;
	skeleton=null;
}

Model::~Model(){
    if(skeleton != null){
        delete skeleton;
        skeleton = null;
    }
}

void Model::DrawMesh(Camera* cam,Mesh* m){
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D,m->mat.texture.atlas_id);
    glUniform1i(cam->shader->TEXTURE_0,0);
    glUniform4fv(cam->shader->TEXTURE_LOCATION,1,(GLfloat*)&m->mat.texture.tex_coords);

    glUniform3fv(cam->shader->AMBIENT,1,(GLfloat*)&m->mat.base_color);
    glUniform3fv(cam->shader->DIFFUSE,1,(GLfloat*)&m->mat.base_color);
    glUniform3fv(cam->shader->SPECULAR,1,(GLfloat*)&m->mat.base_color);
    
    m->vertex.Bind(cam->shader->ATTRIB_VERTEX);
    m->normal.Bind(cam->shader->ATTRIB_NORMAL);
    m->texcoord_0.Bind(cam->shader->ATTRIB_TEXCOORD);

    //m->bone_0_index.Bind(cam->shader->ATTRIB_BONE_INDEX);
    glBindBuffer(GL_ARRAY_BUFFER,m->bone_0_index.buffer_id);
    glVertexAttribIPointer(cam->shader->ATTRIB_BONE_INDEX,4,GL_SHORT,0,0);

    if(m->index.Valid()){
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m->index.buffer_id);
        glDrawElements(GL_TRIANGLES,m->vertex_count,m->index.element_type,nullptr);
    }
    else{
        glDrawArrays(GL_TRIANGLES,0,m->vertex_count);
    } 
    int err = glGetError();
    if(err != 0){
        logger::warn("Model.Mesh.glDrawXXX-> GL error: %d \n",err);
    }
}

void Model::Draw(Camera* cam,mat4* view, mat4* projection){
    glEnableVertexAttribArray(cam->shader->ATTRIB_VERTEX);
    glEnableVertexAttribArray(cam->shader->ATTRIB_TEXCOORD);
    glEnableVertexAttribArray(cam->shader->ATTRIB_NORMAL);
    glEnableVertexAttribArray(cam->shader->ATTRIB_BONE_INDEX);
    
    mat4 model;
    mat3 normal;
    mat4 identity;

    identity.identity();

    model.identity();
    model.scale(scale);
    model.rotate(rotation);
    model.translate(x,y,z);
    view->multiply_by(&model);

    normal.set(view);
    normal.transpose();
    normal.invert();
    
    if(skeleton != null){
        skeleton->CalculatePose();   
        for(int i=0;i< skeleton->bone_count;i++){
            glUniformMatrix4fv(cam->shader->POSE_MATRICES+i,1,true,(GLfloat*)&skeleton->pose_matrices[i]);
        } 
    }

    glUniformMatrix4fv(cam->shader->MODELVIEW_MATRIX,1,true,(GLfloat*)view);
    glUniformMatrix4fv(cam->shader->PROJECTION_MATRIX,1,true,(GLfloat*)projection);
    glUniformMatrix3fv(cam->shader->NORMAL_MATRIX,1,true,(GLfloat*)&normal);

    for(int i=0;i<mesh_group_count;i++){
        for(int j=0;j<mesh_groups[i].mesh_count;j++){
            DrawMesh(cam,&mesh_groups[i].meshes[j]);
        }
    }

    if(skeleton != null){
        for(int i=0;i< skeleton->bone_count;i++){ 
            glUniformMatrix4fv(cam->shader->POSE_MATRICES+i,1,true,(GLfloat*)&identity);
        }
    }

    glDisableVertexAttribArray(cam->shader->ATTRIB_BONE_INDEX);
    glDisableVertexAttribArray(cam->shader->ATTRIB_NORMAL);
    glDisableVertexAttribArray(cam->shader->ATTRIB_TEXCOORD);
    glDisableVertexAttribArray(cam->shader->ATTRIB_VERTEX);
}


void DestroyMesh(Mesh* m){
    //TODO: texture, model smart pointers?
    m->vertex.Destroy();
    m->normal.Destroy();
    m->texcoord_0.Destroy();
    m->bone_0_index.Destroy();
    m->bone_0_weight.Destroy();
    m->index.Destroy();
}

void DestroyMeshGroup(MeshGroup* m){
    if(m->name != nullptr){free(m->name);}
    for(int i=0;i<m->mesh_count;i++){
        DestroyMesh(&m->meshes[i]);
    }
}

void Model::Clone(Model* dest){
    dest->name=name;
    dest->mesh_groups=mesh_groups;
    dest->mesh_group_count=mesh_group_count;
    if(skeleton != null){
        dest->skeleton=new Skeleton();
        skeleton->Clone(dest->skeleton);
    }
    dest->bounds = bounds;
}

void Model::DestroySharedData(){//Will break all instances of a model, be careful!
    free(name);
    if(mesh_groups != null){
        for(int i=0;i<mesh_group_count;i++){DestroyMeshGroup(&mesh_groups[i]);}
        mesh_group_count=0;
        mesh_groups=null;
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
    empty_model.mesh_groups=(MeshGroup*)calloc(1,sizeof(MeshGroup));
    empty_model.mesh_group_count=1;
    empty_model.mesh_groups[0].meshes = (Mesh*)calloc(1,sizeof(Mesh));
    empty_model.mesh_groups[0].name = "ErrorModel.MeshGroup";
    empty_model.mesh_groups[0].meshes[0].vertex_count = error_cube.vertex_count;
    empty_model.mesh_groups[0].meshes[0].mat = error_cube.mat;
    empty_model.mesh_groups[0].meshes[0].vertex = error_cube.vertices;
    empty_model.mesh_groups[0].meshes[0].texcoord_0 = error_cube.tex_coords;
    empty_model.mesh_groups[0].meshes[0].normal = error_cube.normals;
}

void ModelManager::Add(const char* name, Model* model){
    cached_models.Add((byte*)name,(byte*)model);
}
Model* ModelManager::Get(const char* name){
    Model* ret = (Model*)cached_models.StrGet(name);
    if(ret == nullptr){ret = ErrorModel();}
    return ret;
}
void ModelManager::Remove(const char* name){
    Model* mdl = (Model*)cached_models.StrRemove(name);
    mdl->DestroySharedData();
    delete mdl;
}
Model* ModelManager::ErrorModel(){
    return &empty_model;
}