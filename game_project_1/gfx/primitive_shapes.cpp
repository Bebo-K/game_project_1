#include <game_project_1/gfx/primitive_shapes.h>
#include <game_project_1/io/log.hpp>

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
    cube_vertices.Create(cube_vert_data,GL_FLOAT,3,36,GL_ARRAY_BUFFER);
    cube_texcoords.Create(cube_texcoord_data,GL_FLOAT,2,36,GL_ARRAY_BUFFER);
    cube_normals.Create(cube_normal_data,GL_FLOAT,3,36,GL_ARRAY_BUFFER);
}

ShapePrimitive::ShapePrimitive(EPrimitiveShape shape,const char* texture,float w,float h,float d):mat(),color(1.0f,1.0f,1.0f,1.0f){
    offset.scale={w,h,d};

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

ShapePrimitive::ShapePrimitive(EPrimitiveShape shape,Texture tex,float w,float h,float d):mat(),color(1.0f,1.0f,1.0f,1.0f){
    offset.scale={w,h,d};
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

WirePrimitive::WirePrimitive(EPrimitiveShape shape,color_f prim_color,float w,float h,float d):mat(),color(prim_color){
    offset.scale={w,h,d};

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

WirePrimitive::~WirePrimitive(){}


void ShapePrimitive::Draw(Camera* cam){
    Shader* s = ShaderManager::UseShader(shader);
    glEnableVertexAttribArray(s->ATTRIB_VERTEX);
    glEnableVertexAttribArray(s->ATTRIB_TEXCOORD);
    glEnableVertexAttribArray(s->ATTRIB_NORMAL);
    
    glUniform3fv(s->AMBIENT,1,(GLfloat*)&mat.base_color);
    glUniform3fv(s->DIFFUSE,1,(GLfloat*)&mat.base_color);
    glUniform3fv(s->SPECULAR,1,(GLfloat*)&mat.base_color);
    
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D,mat.texture.atlas_id);
    glUniform1i(s->TEXTURE_0,0);
    glUniform4fv(s->TEXTURE_LOCATION,1,(GLfloat*)&mat.texture.tex_coords);

    mat4 model;
    mat3 normal;
    mat4 projection = cam->ProjectionMatrix();

    model.identity();
    offset.ApplyTo(model);
    mat4 modelview = cam->ViewMatrix();
    modelview.multiply_by(&model);
    //model.scale(scale);
    //model.rotate(transform.rotation);
    //model.translate(x,y,z);

    normal.set(&modelview);
    normal.transpose();
    normal.invert();

    
    glUniformMatrix4fv(s->MODELVIEW_MATRIX,1,true,(GLfloat*)&modelview);
    glUniformMatrix4fv(s->PROJECTION_MATRIX,1,true,(GLfloat*)&projection);
    glUniformMatrix3fv(s->NORMAL_MATRIX,1,true,(GLfloat*)&normal);
    
    vertices.Bind(0);
    tex_coords.Bind(1);
    normals.Bind(2);
    
    glDrawArrays(GL_TRIANGLES,0,vertex_count);

    CheckForGLError("ShapePrimitive.Draw: GL Error %d during draw\n");
}

void WirePrimitive::Draw(Camera* cam){
    Shader* s = ShaderManager::UseShader(shader);
    glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
    
    glUniform3fv(s->AMBIENT,1,(GLfloat*)&mat.base_color);
    glUniform3fv(s->DIFFUSE,1,(GLfloat*)&mat.base_color);
    glUniform3fv(s->SPECULAR,1,(GLfloat*)&mat.base_color);
    
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D,mat.texture.atlas_id);
    glUniform1i(s->TEXTURE_0,0);
    glUniform4fv(s->TEXTURE_LOCATION,1,(GLfloat*)&mat.texture.tex_coords);

    mat4 model;
    mat3 normal;
    mat4 projection = cam->ProjectionMatrix();

    model.identity();
    offset.ApplyTo(model);
    mat4 modelview = cam->ViewMatrix();
    modelview.multiply_by(&model);
    //model.scale(scale);
    //model.rotate(transform.rotation);
    //model.translate(x,y,z);

    normal.set(&modelview);
    normal.transpose();
    normal.invert();

    
    glUniformMatrix4fv(s->MODELVIEW_MATRIX,1,true,(GLfloat*)&modelview);
    glUniformMatrix4fv(s->PROJECTION_MATRIX,1,true,(GLfloat*)&projection);
    glUniformMatrix3fv(s->NORMAL_MATRIX,1,true,(GLfloat*)&normal);
    
    vertices.Bind(0);
    tex_coords.Bind(1);
    normals.Bind(2);
    
    glDrawArrays(GL_TRIANGLES,0,vertex_count);
    glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
    
    CheckForGLError("WirePrimitive.Draw: GL Error %d during draw\n");
}
 

