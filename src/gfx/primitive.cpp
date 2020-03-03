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

bool VBO::Valid(){
    return buffer_id > 0;
}
void VBO::Create(void* data,GLuint type,int per_vertex,int vertex_count){
    elements_per_vertex = per_vertex;
    element_type = type;
    int element_size;
	switch(type){
		case 5120:element_size=1;break;
		case 5121:element_size=1;break;
		case 5122:element_size=2;break;
		case 5123:element_size=2;break;
		case 5126:element_size=4;break;
		case 5125:element_size=4;break;
		default:element_size=4;break;
	}
    glGenBuffers(1,&buffer_id);
    glBindBuffer(GL_ARRAY_BUFFER, buffer_id);
    glBufferData(GL_ARRAY_BUFFER,element_size*elements_per_vertex*vertex_count, data,GL_STATIC_DRAW);
}
void VBO::Create(void* data,GLuint type,int per_vertex,int vertex_count,GLuint buffer_type){
    elements_per_vertex = per_vertex;
    element_type = type;
    int element_size;
	switch(type){
		case 5120:element_size=1;break;
		case 5121:element_size=1;break;
		case 5122:element_size=2;break;
		case 5123:element_size=2;break;
		case 5126:element_size=4;break;
		case 5125:element_size=4;break;
		default:element_size=4;break;
	}
    glGenBuffers(1,&buffer_id);
    glBindBuffer(GL_ARRAY_BUFFER, buffer_id);
    glBufferData(GL_ARRAY_BUFFER,element_size*elements_per_vertex*vertex_count, data,GL_STATIC_DRAW);
}

void VBO::Bind(int attrib_slot){
    glBindBuffer(GL_ARRAY_BUFFER,buffer_id);
    glVertexAttribPointer(attrib_slot,elements_per_vertex,element_type,false,0,0);
}
void VBO::Bind(int attrib_slot, int stride, int start){
    glBindBuffer(GL_ARRAY_BUFFER,buffer_id);
    glVertexAttribPointer(attrib_slot,elements_per_vertex,element_type,false,stride,(void*)start);
}

void VBO::Destroy(){
    if(Valid())glDeleteBuffers(1,&buffer_id);
    buffer_id = -1;
}

void ShapePrimitive::Draw(Camera* cam,mat4* view, mat4* projection){
    glEnableVertexAttribArray(cam->shader->ATTRIB_VERTEX);
    glEnableVertexAttribArray(cam->shader->ATTRIB_TEXCOORD);
    glEnableVertexAttribArray(cam->shader->ATTRIB_NORMAL);
    
    glUniform3fv(cam->shader->AMBIENT,1,(GLfloat*)&mat.base_color);
    glUniform3fv(cam->shader->DIFFUSE,1,(GLfloat*)&mat.base_color);
    glUniform3fv(cam->shader->SPECULAR,1,(GLfloat*)&mat.base_color);
    
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D,mat.texture.atlas_id);
    glUniform1i(cam->shader->TEXTURE_0,0);
    glUniform4fv(cam->shader->TEXTURE_LOCATION,1,(GLfloat*)&mat.texture.tex_coords);

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
    
    vertices.Bind(0);
    tex_coords.Bind(1);
    normals.Bind(2);
    
    glDrawArrays(GL_TRIANGLES,0,vertex_count);
    glDisableVertexAttribArray(cam->shader->ATTRIB_TEXCOORD);
    glDisableVertexAttribArray(cam->shader->ATTRIB_VERTEX);
    glDisableVertexAttribArray(cam->shader->ATTRIB_NORMAL);
}

void WirePrimitive::Draw(Camera* cam,mat4* view, mat4* projection){
    glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
    glEnableVertexAttribArray(cam->shader->ATTRIB_VERTEX);
    glEnableVertexAttribArray(cam->shader->ATTRIB_TEXCOORD);
    glEnableVertexAttribArray(cam->shader->ATTRIB_NORMAL);
    
    glUniform3fv(cam->shader->AMBIENT,1,(GLfloat*)&mat.base_color);
    glUniform3fv(cam->shader->DIFFUSE,1,(GLfloat*)&mat.base_color);
    glUniform3fv(cam->shader->SPECULAR,1,(GLfloat*)&mat.base_color);
    
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D,mat.texture.atlas_id);
    glUniform1i(cam->shader->TEXTURE_0,0);
    glUniform4fv(cam->shader->TEXTURE_LOCATION,1,(GLfloat*)&mat.texture.tex_coords);

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
    
    vertices.Bind(0);
    tex_coords.Bind(1);
    normals.Bind(2);
    
    glDrawArrays(GL_TRIANGLES,0,vertex_count);
    glDisableVertexAttribArray(cam->shader->ATTRIB_TEXCOORD);
    glDisableVertexAttribArray(cam->shader->ATTRIB_VERTEX);
    glDisableVertexAttribArray(cam->shader->ATTRIB_NORMAL);
    glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
}
 

