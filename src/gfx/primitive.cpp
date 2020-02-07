#include "primitive.h"
#include "../log.h"


void Primitive::Draw(Camera* cam,mat4* modelview, mat4* projection){}

Primitive::Primitive(){ 
    x=0;y=0;z=0;
    scale={1,1,1};
    rotation={0,0,0};
    hidden=false;
    layer=1;
}

Primitive::~Primitive(){}


void ShapePrimitive::Draw(Camera* cam,mat4* view, mat4* projection){
    glEnableVertexAttribArray(cam->shader->ATTRIB_VERTEX);
    glEnableVertexAttribArray(cam->shader->ATTRIB_TEXCOORD);
    glEnableVertexAttribArray(cam->shader->ATTRIB_NORMAL);
    
    glUniform3fv(cam->shader->AMBIENT,1,(GLfloat*)&mat->base_color);
    glUniform3fv(cam->shader->DIFFUSE,1,(GLfloat*)&mat->base_color);
    glUniform3fv(cam->shader->SPECULAR,1,(GLfloat*)&mat->base_color);
    
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D,mat->texture.atlas_id);
    glUniform1i(cam->shader->TEXTURE_0,0);
    glUniform4fv(cam->shader->TEXTURE_LOCATION,1,(GLfloat*)&mat->texture.tex_coords);

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
    
    glBindBuffer(GL_ARRAY_BUFFER,vertex_buffer);
    glVertexAttribPointer(0,3,GL_FLOAT,false,0,0);

    glBindBuffer(GL_ARRAY_BUFFER,texcoord_buffer);
    glVertexAttribPointer(1,2,GL_FLOAT,false,0,0);

    glBindBuffer(GL_ARRAY_BUFFER,normal_buffer);
    glVertexAttribPointer(2,3,GL_FLOAT,false,0,0);
    
    glDrawArrays(GL_TRIANGLES,0,vertices);
    glDisableVertexAttribArray(cam->shader->ATTRIB_TEXCOORD);
    glDisableVertexAttribArray(cam->shader->ATTRIB_VERTEX);
    glDisableVertexAttribArray(cam->shader->ATTRIB_NORMAL);
}

void WirePrimitive::Draw(Camera* cam,mat4* view, mat4* projection){
    glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
    glEnableVertexAttribArray(cam->shader->ATTRIB_VERTEX);
    glEnableVertexAttribArray(cam->shader->ATTRIB_TEXCOORD);
    glEnableVertexAttribArray(cam->shader->ATTRIB_NORMAL);
    
    glUniform3fv(cam->shader->AMBIENT,1,(GLfloat*)&mat->base_color);
    glUniform3fv(cam->shader->DIFFUSE,1,(GLfloat*)&mat->base_color);
    glUniform3fv(cam->shader->SPECULAR,1,(GLfloat*)&mat->base_color);
    
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D,mat->texture.atlas_id);
    glUniform1i(cam->shader->TEXTURE_0,0);
    glUniform4fv(cam->shader->TEXTURE_LOCATION,1,(GLfloat*)&mat->texture.tex_coords);

    mat4 model;
    mat3 normal;

    model.identity();
    model.translate(x,y,z);
    model.rotate(rotation);
    model.scale(scale);
    view->multiply_by(&model);

    normal.set(view);
    normal.transpose();
    normal.invert();
    
    glUniformMatrix4fv(cam->shader->MODELVIEW_MATRIX,1,true,(GLfloat*)view);
    glUniformMatrix4fv(cam->shader->PROJECTION_MATRIX,1,true,(GLfloat*)projection);
    glUniformMatrix3fv(cam->shader->NORMAL_MATRIX,1,true,(GLfloat*)&normal);
    
    glBindBuffer(GL_ARRAY_BUFFER,vertex_buffer);
    glVertexAttribPointer(0,3,GL_FLOAT,false,0,0);

    glBindBuffer(GL_ARRAY_BUFFER,texcoord_buffer);
    glVertexAttribPointer(1,2,GL_FLOAT,false,0,0);

    glBindBuffer(GL_ARRAY_BUFFER,normal_buffer);
    glVertexAttribPointer(2,3,GL_FLOAT,false,0,0);
    
    glDrawArrays(GL_TRIANGLES,0,vertices);
    glDisableVertexAttribArray(cam->shader->ATTRIB_TEXCOORD);
    glDisableVertexAttribArray(cam->shader->ATTRIB_VERTEX);
    glDisableVertexAttribArray(cam->shader->ATTRIB_NORMAL);
    glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
}
 

