#include "model.h"
#include <string.h>
#include "../log.h"
#include "../io/gltf.h"


ModelData::ModelData(){
    name=null;
    mesh_group_count=0;
	mesh_groups=null;
	skeleton=null;
}

ModelData::~ModelData(){
    if(skeleton != null){
        delete skeleton;
        skeleton = null;
    }
    if(name != null){free(name);name=null;}
    if(mesh_groups != null){
        for(int i=0;i<mesh_group_count;i++){DestroyMeshGroup(&mesh_groups[i]);}
        mesh_group_count=0;
        mesh_groups=null;
    }
    if(skeleton != null){
        delete(skeleton);
        skeleton=null;
    };
}

void ModelData::DrawMesh(Camera* cam,int group_index,int mesh_index){
    Mesh* m = &mesh_groups[group_index].meshes[mesh_index];
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
    
    if(pose != null){
        pose->Calculate(); 
        for(int i=0;i< pose->bone_count;i++){
            glUniformMatrix4fv(cam->shader->POSE_MATRICES+i,1,true,(GLfloat*)&pose->matrices[i]);
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
        for(int i=0;i< pose->bone_count;i++){ 
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

}

#include "model_registry.h"
#include "../io/gltf.h"

AssociativeArray cached_model_data(1);
ModelData empty_model;

void InitModelCache(){
    cached_model_data.Resize(8);
    ShapePrimitive error_cube(EPrimitiveShape::CUBE,"./dat/img/error.png",1,1,1);

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

void Model::Register(ModelID id, const char* filename){
    if(File::Exists(filename)){
        File model_file(filename);
        GLTFScene model_scene(model_file);
        //cached_models.Add((int)id,model_scene.Load);
    }
}

Model* Model::Instantiate(ModelID id){
    if(cached_model_data.IndexOf((int)id) >= 0){
        return new Model((ModelData*)cached_model_data.Get(id));
    }
    return new Model(&empty_model);
}

Model* ModelCache::Free(ModelID id){
    if(cached_models.IndexOf((int)id) >= 0){
        return cached_models.Get(id);
    }
    return &empty_model;
}

Model* ModelCache::Error(){
    return &empty_model;
}

