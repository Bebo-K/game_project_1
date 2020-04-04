#include "solid_geometry.h"
#include "../log.h"

SolidGeometry::SolidGeometry(){
    group_count=0;
    groups=null;
}

SolidGeometry::~SolidGeometry(){
    if(groups != null){
        delete[] groups;
    }
}

SolidMesh::SolidMesh(){
    vertices = nullptr;
    vertex_count=0;
}
SolidMesh::~SolidMesh(){
    if(vertices != null){free(vertices);vertices=null;}
    vertex.Destroy();
    normal.Destroy();
    index.Destroy();
    texcoord_0.Destroy();
    index.Destroy();
}

SolidGroup::SolidGroup(){
    mesh_count=0;
    meshes=0;
    name=nullptr;
}
SolidGroup::~SolidGroup(){
    if(name != null){free(name);name=null;}
    if(meshes != null){delete[] meshes;meshes=null;}
}

void SolidGeometry::Draw(Camera* cam,mat4* view, mat4* projection){
    glEnableVertexAttribArray(cam->shader->ATTRIB_VERTEX);
    glEnableVertexAttribArray(cam->shader->ATTRIB_TEXCOORD);
    glEnableVertexAttribArray(cam->shader->ATTRIB_NORMAL);
    
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
    
    glUniformMatrix4fv(cam->shader->MODELVIEW_MATRIX,1,true,(GLfloat*)view);
    glUniformMatrix4fv(cam->shader->PROJECTION_MATRIX,1,true,(GLfloat*)projection);
    glUniformMatrix3fv(cam->shader->NORMAL_MATRIX,1,true,(GLfloat*)&normal);

    for(int i=0;i<group_count;i++){
        for(int j=0;j<groups[i].mesh_count;j++){
            DrawMesh(cam,&groups[i].meshes[j]);
        }
    }

    glDisableVertexAttribArray(cam->shader->ATTRIB_NORMAL);
    glDisableVertexAttribArray(cam->shader->ATTRIB_TEXCOORD);
    glDisableVertexAttribArray(cam->shader->ATTRIB_VERTEX);    
}

void SolidGeometry::DrawMesh(Camera* cam,SolidMesh* mesh){
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D,mesh->mat.texture.atlas_id);
    glUniform1i(cam->shader->TEXTURE_0,0);
    glUniform4fv(cam->shader->TEXTURE_LOCATION,1,(GLfloat*)&mesh->mat.texture.tex_coords);

    glUniform3fv(cam->shader->AMBIENT,1,(GLfloat*)&mesh->mat.base_color);
    glUniform3fv(cam->shader->DIFFUSE,1,(GLfloat*)&mesh->mat.base_color);
    glUniform3fv(cam->shader->SPECULAR,1,(GLfloat*)&mesh->mat.base_color);
    
    mesh->vertex.Bind(cam->shader->ATTRIB_VERTEX);
    mesh->normal.Bind(cam->shader->ATTRIB_NORMAL);
    mesh->texcoord_0.Bind(cam->shader->ATTRIB_TEXCOORD);

    if(mesh->index.Valid()){
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->index.buffer_id);
        glDrawElements(GL_TRIANGLES,mesh->vertex_count,mesh->index.element_type,nullptr);
    }
    else{
        glDrawArrays(GL_TRIANGLES,0,mesh->vertex_count);
    } 
    int err = glGetError();
    if(err != 0){
        logger::warn("SolidGeometry.Mesh.glDrawXXX-> GL error: %d \n",err);
    }
};