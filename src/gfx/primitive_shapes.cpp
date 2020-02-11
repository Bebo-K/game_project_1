#include "primitive.h"
#include "../log.h"

const float lo = 0.0f;
const float hi = 1.0f;
const float x = 0.5f;
const float y = 0.5f;
const float z = 0.5f;

float cube_verts[] ={
    -x, -y,  z,   x, -y,  z,   x,  y,  z,    -x, -y,  z,   x,  y,  z,  -x,  y,  z,// Front face
    -x, -y, -z,  -x,  y, -z,   x,  y, -z,    -x, -y, -z,   x,  y, -z,   x, -y, -z,// Back face
    -x,  y, -z,  -x,  y,  z,   x,  y,  z,    -x,  y, -z,   x,  y,  z,   x,  y, -z,// Top face
    -x, -y, -z,   x, -y, -z,   x, -y,  z,    -x, -y, -z,   x, -y,  z,  -x, -y,  z,// Bottom face
     x, -y, -z,   x,  y, -z,   x,  y,  z,     x, -y, -z,   x,  y,  z,   x, -y,  z,// Right face
    -x, -y, -z,  -x, -y,  z,  -x,  y,  z,    -x, -y, -z,  -x,  y,  z,  -x,  y, -z// Left face
};

float cube_texcoords[] = {
    lo,hi, hi,hi, hi,lo,  lo,hi, hi,lo, lo,lo, //Front
    hi,hi, hi,lo, lo,lo,  hi,hi, lo,lo, lo,hi, //Back
    lo,lo, lo,hi, hi,hi,  lo,lo, hi,hi, hi,lo, //Top
    lo,hi, hi,hi, hi,lo,  lo,hi, hi,lo, lo,lo, //Bottom
    hi,hi, hi,lo, lo,lo,  hi,hi, lo,lo, lo,hi, //Right
    lo,hi, hi,hi, hi,lo,  lo,hi, hi,lo, lo,lo //Left
};

float cube_normals[] = {
    0, 0, 1,  0, 0, 1,  0, 0, 1,  0, 0, 1,  0, 0, 1,  0, 0, 1, // Front face
    0, 0,-1,  0, 0,-1,  0, 0,-1,  0, 0,-1,  0, 0,-1,  0, 0,-1, // Back face
    0, 1, 0,  0, 1, 0,  0, 1, 0,  0, 1, 0,  0, 1, 0,  0, 1, 0, // Top face
    0,-1, 0,  0,-1, 0,  0,-1, 0,  0,-1, 0,  0,-1, 0,  0,-1, 0, // Bottom face
    1, 0, 0,  1, 0, 0,  1, 0, 0,  1, 0, 0,  1, 0, 0,  1, 0, 0, // Right face
    -1, 0, 0, -1, 0, 0, -1, 0, 0, -1, 0, 0, -1, 0, 0, -1, 0, 0  // Left face
};

GLuint cube_vertex_buffer=0;
GLuint cube_texcoord_buffer=0;
GLuint cube_normal_buffer=0;


void BuildPrimitive(GLuint* vertex_buffer,GLuint* texcoord_buffer,GLuint* normal_buffer,
    float* verts,float* texcoords,float* normals,int tris){
    glGenBuffers(1,vertex_buffer);
    glGenBuffers(1,texcoord_buffer);
    glGenBuffers(1,normal_buffer);

    glBindBuffer(GL_ARRAY_BUFFER, *vertex_buffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float)*tris*3, verts,GL_STATIC_DRAW);
    
    glBindBuffer(GL_ARRAY_BUFFER, *texcoord_buffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float)*tris*2, texcoords,GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, *normal_buffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float)*tris*3, normals,GL_STATIC_DRAW);
}

void BuildCubePrimitive(){
    BuildPrimitive(&cube_vertex_buffer,&cube_texcoord_buffer,&cube_normal_buffer,
            cube_verts,cube_texcoords,cube_normals,36);
}

ShapePrimitive::ShapePrimitive(EPrimitiveShape shape,const char* texture,float w,float h,float d){
    mat = new Material();

    scale.x=w;
    scale.y=h;
    scale.z=d;

    switch(shape){
        case EPrimitiveShape::CUBE:
            if(cube_vertex_buffer==0){BuildCubePrimitive();}
            vertex_buffer=cube_vertex_buffer;
            texcoord_buffer=cube_texcoord_buffer;
            normal_buffer=cube_normal_buffer;
            vertices = 36;
            break;
        default:
            logger::warn("tried to instantiate primitive with invalid shape %d",shape);
            return;
    }



}

ShapePrimitive::~ShapePrimitive(){
    delete(mat);
    if(vertex_buffer > 0) {glDeleteBuffers(1,&vertex_buffer);}
    if(texcoord_buffer > 0) {glDeleteBuffers(1,&texcoord_buffer);}
    if(normal_buffer > 0) {glDeleteBuffers(1,&normal_buffer);}	
}

WirePrimitive::WirePrimitive(EPrimitiveShape shape,vec3 prim_color,float w,float h,float d){
    mat = new Material();
    color[0] = prim_color.x;
    color[1] = prim_color.y;
    color[2] = prim_color.z;
    color[3] = 1.0f;

    scale.x=w;
    scale.y=h;
    scale.z=d;

    float *verts,*texcoords,*normals;

    switch(shape){
        case EPrimitiveShape::CUBE:
            verts = cube_verts;
            texcoords = cube_texcoords;
            normals = cube_normals;
            vertices = 36;
            break;
        default:
            logger::warn("tried to instantiate primitive with invalid shape %d",shape);
            return;
    }

    glGenBuffers(1, &vertex_buffer);
    glGenBuffers(1,&texcoord_buffer);
    glGenBuffers(1,&normal_buffer);

    glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float)*108, verts,GL_STATIC_DRAW);
    
    glBindBuffer(GL_ARRAY_BUFFER, texcoord_buffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float)*72, texcoords,GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, normal_buffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float)*108, normals,GL_STATIC_DRAW);
}

WirePrimitive::~WirePrimitive(){
    delete(mat);
    if(vertex_buffer > 0) {glDeleteBuffers(1,&vertex_buffer);}
    if(texcoord_buffer > 0) {glDeleteBuffers(1,&texcoord_buffer);}
    if(normal_buffer > 0) {glDeleteBuffers(1,&normal_buffer);}	
}