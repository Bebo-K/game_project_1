#include "drawable.h"
#include "../log.h"

const float lo = 0.0f;
const float hi = 1.0f;
const float x = 0.5f;
const float y = 0.5f;
const float z = 0.5f;

float cube_vert_data[] ={
     x,  y,  z,   x, -y,  z,  -x,  -y,  z,   -x, -y,  z,  -x,  y,  z,   x,  y,  z,// Front face
    -x,  y, -z,  -x, -y, -z,   x,  -y, -z,    x, -y, -z,   x,  y, -z,  -x,  y, -z,// Back face
    -x,  y, -z,  -x,  y,  z,   x,  y,  z,    -x,  y, -z,   x,  y,  z,   x,  y, -z,// Top face
    -x, -y, -z,   x, -y, -z,   x, -y,  z,    -x, -y, -z,   x, -y,  z,  -x, -y,  z,// Bottom face
     x, -y, -z,   x,  y, -z,   x,  y,  z,     x, -y, -z,   x,  y,  z,   x, -y,  z,// Right face
    -x, -y, -z,  -x, -y,  z,  -x,  y,  z,    -x, -y, -z,  -x,  y,  z,  -x,  y, -z// Left face
};

float cube_texcoord_data[] = {
    hi,lo, hi,hi, lo,hi,  lo,hi, lo,lo, hi,lo, //Front
    hi,lo, hi,hi, lo,hi,  lo,hi, lo,lo, hi,lo, //Back
    lo,hi, lo,lo, hi,lo,  lo,hi, hi,lo, hi,hi, //Top
    lo,hi, lo,lo, hi,lo,  lo,hi, hi,lo, hi,hi, //Bottom
    hi,hi, hi,lo, lo,lo,  hi,hi, lo,lo, lo,hi, //Right
    lo,hi, hi,hi, hi,lo,  lo,hi, hi,lo, lo,lo //Left
};

float cube_normal_data[] = {
    0, 0, 1,  0, 0, 1,  0, 0, 1,  0, 0, 1,  0, 0, 1,  0, 0, 1, // Front face
    0, 0,-1,  0, 0,-1,  0, 0,-1,  0, 0,-1,  0, 0,-1,  0, 0,-1, // Back face
    0, 1, 0,  0, 1, 0,  0, 1, 0,  0, 1, 0,  0, 1, 0,  0, 1, 0, // Top face
    0,-1, 0,  0,-1, 0,  0,-1, 0,  0,-1, 0,  0,-1, 0,  0,-1, 0, // Bottom face
    1, 0, 0,  1, 0, 0,  1, 0, 0,  1, 0, 0,  1, 0, 0,  1, 0, 0, // Right face
    -1, 0, 0, -1, 0, 0, -1, 0, 0, -1, 0, 0, -1, 0, 0, -1, 0, 0  // Left face
};

VBO cube_vertices;
VBO cube_texcoords;
VBO cube_normals;

void BuildCubePrimitive(){
    cube_vertices.Create(cube_vert_data,GL_FLOAT,3,36);
    cube_texcoords.Create(cube_texcoord_data,GL_FLOAT,2,36);
    cube_normals.Create(cube_normal_data,GL_FLOAT,3,36);
}

ShapePrimitive::ShapePrimitive(EPrimitiveShape shape,const char* texture,float w,float h,float d):mat(){
    scale.x=w;
    scale.y=h;
    scale.z=d;

    switch(shape){
        case EPrimitiveShape::CUBE:
            if(!cube_vertices.Valid()){BuildCubePrimitive();}
            vertices=cube_vertices;
            tex_coords=cube_texcoords;
            normals=cube_normals;
            vertex_count = 36;
            break;
        default:
            logger::warn("tried to instantiate primitive with invalid shape %d",shape);
            return;
    }
}

ShapePrimitive::ShapePrimitive(EPrimitiveShape shape,Texture tex,float w,float h,float d):mat(){
    scale.x=w;
    scale.y=h;
    scale.z=d;
    mat.texture=tex;

    switch(shape){
        case EPrimitiveShape::CUBE:
            if(!cube_vertices.Valid()){BuildCubePrimitive();}
            vertices=cube_vertices;
            tex_coords=cube_texcoords;
            normals=cube_normals;
            vertex_count = 36;
            break;
        default:
            logger::warn("tried to instantiate primitive with invalid shape %d",shape);
            return;
    }
}

ShapePrimitive::~ShapePrimitive(){
}

WirePrimitive::WirePrimitive(EPrimitiveShape shape,vec3 prim_color,float w,float h,float d):mat(){
    color[0] = prim_color.x;
    color[1] = prim_color.y;
    color[2] = prim_color.z;
    color[3] = 1.0f;

    scale.x=w;
    scale.y=h;
    scale.z=d;

    switch(shape){
        case EPrimitiveShape::CUBE:
            if(!cube_vertices.Valid()){BuildCubePrimitive();}
            vertices=cube_vertices;
            tex_coords=cube_texcoords;
            normals=cube_normals;
            vertex_count = 36;
            break;
        default:
            logger::warn("tried to instantiate primitive with invalid shape %d",shape);
            return;
    }
}

WirePrimitive::~WirePrimitive(){
}