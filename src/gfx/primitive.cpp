#include "primitive.h"
#include "../io/log.h"


void Primitive::Draw(Camera* cam,mat4* modelview, mat4* projection){}

Primitive::Primitive(){ 
    x=0;y=0;z=0;
    scale={1,1,1};
    rotation={0,0,0};
    hidden=false;
    layer=1;
}


void CubePrimitive::Draw(Camera* cam,mat4* view, mat4* projection){
    glEnableVertexAttribArray(cam->shader->ATTRIB_VERTEX);
    glEnableVertexAttribArray(cam->shader->ATTRIB_TEXCOORD);
    glEnableVertexAttribArray(cam->shader->ATTRIB_NORMAL);
    
    glUniform3fv(cam->shader->AMBIENT,1,(GLfloat*)&mat->ambient);
    glUniform3fv(cam->shader->DIFFUSE,1,(GLfloat*)&mat->diffuse);
    glUniform3fv(cam->shader->SPECULAR,1,(GLfloat*)&mat->specular);
    
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
}

CubePrimitive::CubePrimitive(const char* texture,float w,float h,float d){
    mat = new Material(texture);

    float x = w/2;
    float y = h/2;
    float z = d/2;

    float verts[] ={
        -x, -y,  z,   x, -y,  z,   x,  y,  z,    -x, -y,  z,   x,  y,  z,  -x,  y,  z,// Front face
        -x, -y, -z,  -x,  y, -z,   x,  y, -z,    -x, -y, -z,   x,  y, -z,   x, -y, -z,// Back face
        -x,  y, -z,  -x,  y,  z,   x,  y,  z,    -x,  y, -z,   x,  y,  z,   x,  y, -z,// Top face
        -x, -y, -z,   x, -y, -z,   x, -y,  z,    -x, -y, -z,   x, -y,  z,  -x, -y,  z,// Bottom face
         x, -y, -z,   x,  y, -z,   x,  y,  z,     x, -y, -z,   x,  y,  z,   x, -y,  z,// Right face
        -x, -y, -z,  -x, -y,  z,  -x,  y,  z,    -x, -y, -z,  -x,  y,  z,  -x,  y, -z// Left face
    };

    float lo = 0.0;
    float hi = 1.0;
    float texcoords[] = {
        lo,hi, hi,hi, hi,lo,  lo,hi, hi,lo, lo,lo, //Front
        hi,hi, hi,lo, lo,lo,  hi,hi, lo,lo, lo,hi, //Back
        lo,lo, lo,hi, hi,hi,  lo,lo, hi,hi, hi,lo, //Top
        lo,hi, hi,hi, hi,lo,  lo,hi, hi,lo, lo,lo, //Bottom
        hi,hi, hi,lo, lo,lo,  hi,hi, lo,lo, lo,hi, //Right
        lo,hi, hi,hi, hi,lo,  lo,hi, hi,lo, lo,lo //Left
    };

    float normals[] = {
         0, 0, 1,  0, 0, 1,  0, 0, 1,  0, 0, 1,  0, 0, 1,  0, 0, 1, // Front face
         0, 0,-1,  0, 0,-1,  0, 0,-1,  0, 0,-1,  0, 0,-1,  0, 0,-1, // Back face
         0, 1, 0,  0, 1, 0,  0, 1, 0,  0, 1, 0,  0, 1, 0,  0, 1, 0, // Top face
         0,-1, 0,  0,-1, 0,  0,-1, 0,  0,-1, 0,  0,-1, 0,  0,-1, 0, // Bottom face
         1, 0, 0,  1, 0, 0,  1, 0, 0,  1, 0, 0,  1, 0, 0,  1, 0, 0, // Right face
        -1, 0, 0, -1, 0, 0, -1, 0, 0, -1, 0, 0, -1, 0, 0, -1, 0, 0  // Left face
    };

    glGenBuffers(1, &vertex_buffer);
    glGenBuffers(1,&texcoord_buffer);
    glGenBuffers(1,&normal_buffer);
    vertices = 36;

    glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float)*108, verts,GL_STATIC_DRAW);
    
    glBindBuffer(GL_ARRAY_BUFFER, texcoord_buffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float)*72, texcoords,GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, normal_buffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float)*108, normals,GL_STATIC_DRAW);
}

CubePrimitive::~CubePrimitive(){
    free(mat);
    if(vertex_buffer > 0) {glDeleteBuffers(1,&vertex_buffer);}
    if(texcoord_buffer > 0) {glDeleteBuffers(1,&texcoord_buffer);}
    if(normal_buffer > 0) {glDeleteBuffers(1,&normal_buffer);}	
}
 

