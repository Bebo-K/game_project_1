#include <game_project_1/gfx/drawable.hpp>
#include <game_project_1/log.hpp>

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

WirePrimitive::~WirePrimitive(){}


void ShapePrimitive::Draw(Camera* cam){
    Shader* shader = ShaderManager::UseShader(shader_name);
    glEnableVertexAttribArray(shader->ATTRIB_VERTEX);
    glEnableVertexAttribArray(shader->ATTRIB_TEXCOORD);
    glEnableVertexAttribArray(shader->ATTRIB_NORMAL);
    
    glUniform3fv(shader->AMBIENT,1,(GLfloat*)&mat.base_color);
    glUniform3fv(shader->DIFFUSE,1,(GLfloat*)&mat.base_color);
    glUniform3fv(shader->SPECULAR,1,(GLfloat*)&mat.base_color);
    
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D,mat.texture.atlas_id);
    glUniform1i(shader->TEXTURE_0,0);
    glUniform4fv(shader->TEXTURE_LOCATION,1,(GLfloat*)&mat.texture.tex_coords);

    mat4 model;
    mat3 normal;

    model.identity();
    model.scale(scale);
    model.rotate(rotation);
    model.translate(x,y,z);

    normal.set(&cam->view_matrix);
    normal.transpose();
    normal.invert();

    cam->view_matrix.multiply_by(&model);
    
    glUniformMatrix4fv(shader->MODELVIEW_MATRIX,1,true,(GLfloat*)&cam->view_matrix);
    glUniformMatrix4fv(shader->PROJECTION_MATRIX,1,true,(GLfloat*)&cam->projection_matrix);
    glUniformMatrix3fv(shader->NORMAL_MATRIX,1,true,(GLfloat*)&normal);
    
    vertices.Bind(0);
    tex_coords.Bind(1);
    normals.Bind(2);
    
    glDrawArrays(GL_TRIANGLES,0,vertex_count);

    CheckForGLError("ShapePrimitive.Draw: GL Error %d during draw\n");
}

void WirePrimitive::Draw(Camera* cam){
    Shader* shader = ShaderManager::UseShader(shader_name);
    glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
    
    glUniform3fv(shader->AMBIENT,1,(GLfloat*)&mat.base_color);
    glUniform3fv(shader->DIFFUSE,1,(GLfloat*)&mat.base_color);
    glUniform3fv(shader->SPECULAR,1,(GLfloat*)&mat.base_color);
    
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D,mat.texture.atlas_id);
    glUniform1i(shader->TEXTURE_0,0);
    glUniform4fv(shader->TEXTURE_LOCATION,1,(GLfloat*)&mat.texture.tex_coords);

    mat4 model;
    mat3 normal;

    model.identity();
    model.scale(scale);
    model.rotate(rotation);
    model.translate(x,y,z);

    normal.set(&cam->view_matrix);
    normal.transpose();
    normal.invert();

    cam->view_matrix.multiply_by(&model);
    
    glUniformMatrix4fv(shader->MODELVIEW_MATRIX,1,true,(GLfloat*)&cam->view_matrix);
    glUniformMatrix4fv(shader->PROJECTION_MATRIX,1,true,(GLfloat*)&cam->projection_matrix);
    glUniformMatrix3fv(shader->NORMAL_MATRIX,1,true,(GLfloat*)&normal);
    
    vertices.Bind(0);
    tex_coords.Bind(1);
    normals.Bind(2);
    
    glDrawArrays(GL_TRIANGLES,0,vertex_count);
    glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
    
    CheckForGLError("WirePrimitive.Draw: GL Error %d during draw\n");
}
 

