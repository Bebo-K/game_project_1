#include <game_project_1/gfx/drawable.hpp>
#include <game_project_1/log.hpp>


void Drawable::Draw(Camera* cam){}

Drawable::Drawable(){ 
    x=0;y=0;z=0;
    scale={1,1,1};
    rotation={0,0,0};
    hidden=false;
    layer=1;
    shader_name=null;
}

Drawable::~Drawable(){}

VBO::VBO(){
    buffer_id=0;
    element_type=0;
    elements_per_vertex=0;
}

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

void VBO::CreateEmpty(GLuint type,int per_vertex,int vertex_count,GLuint buffer_type){
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
    byte* empty_data = (byte*)calloc(vertex_count,element_size*elements_per_vertex);
    glGenBuffers(1,&buffer_id);
    glBindBuffer(GL_ARRAY_BUFFER, buffer_id);
    glBufferData(GL_ARRAY_BUFFER,element_size*elements_per_vertex*vertex_count, empty_data,GL_STATIC_DRAW);
    free(empty_data);
}


void VBO::CreateEmptyWeights(GLuint type,int per_vertex,int vertex_count,GLuint buffer_type){
    elements_per_vertex = per_vertex;
    element_type = type;
    int element_size=4;
    float* weight_data = (float*)calloc(vertex_count,element_size*elements_per_vertex);
    for(int i=0;i<vertex_count*elements_per_vertex;i++){
        if( (i%4) ==2){weight_data[i]=1.0f;}
        else{weight_data[i]=0.0f;}
    }
    glGenBuffers(1,&buffer_id);
    glBindBuffer(GL_ARRAY_BUFFER, buffer_id);
    glBufferData(GL_ARRAY_BUFFER,element_size*elements_per_vertex*vertex_count, weight_data,GL_STATIC_DRAW);
    //free(weight_data);
}


void VBO::Bind(int attrib_slot){
    glBindBuffer(GL_ARRAY_BUFFER,buffer_id);
    if(element_type <= GL_UNSIGNED_INT){
        glVertexAttribIPointer(attrib_slot,elements_per_vertex,element_type,0,0);
    }
    else{
        glVertexAttribPointer(attrib_slot,elements_per_vertex,element_type,false,0,0);
    }
}

void VBO::Bind(int attrib_slot, int stride, int start){
    glBindBuffer(GL_ARRAY_BUFFER,buffer_id);
    glVertexAttribPointer(attrib_slot,elements_per_vertex,element_type,false,stride,i_to_p(start));
}

void VBO::Destroy(){
    if(Valid())glDeleteBuffers(1,&buffer_id);
    buffer_id = -1;
}

void ShapePrimitive::Draw(Shader* shader,mat4* view, mat4* projection){
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
    view->multiply_by(&model);

    normal.set(view);
    normal.transpose();
    normal.invert();
    
    glUniformMatrix4fv(shader->MODELVIEW_MATRIX,1,true,(GLfloat*)view);
    glUniformMatrix4fv(shader->PROJECTION_MATRIX,1,true,(GLfloat*)projection);
    glUniformMatrix3fv(shader->NORMAL_MATRIX,1,true,(GLfloat*)&normal);
    
    vertices.Bind(0);
    tex_coords.Bind(1);
    normals.Bind(2);
    
    glDrawArrays(GL_TRIANGLES,0,vertex_count);
    glDisableVertexAttribArray(shader->ATTRIB_TEXCOORD);
    glDisableVertexAttribArray(shader->ATTRIB_VERTEX);
    glDisableVertexAttribArray(shader->ATTRIB_NORMAL);
}

void WirePrimitive::Draw(Shader* shader,mat4* view, mat4* projection){
    glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
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
    model.translate(x,y,z);
    model.rotate(rotation);
    model.scale(scale);
    view->multiply_by(&model);

    normal.set(view);
    normal.transpose();
    normal.invert();
    
    glUniformMatrix4fv(shader->MODELVIEW_MATRIX,1,true,(GLfloat*)view);
    glUniformMatrix4fv(shader->PROJECTION_MATRIX,1,true,(GLfloat*)projection);
    glUniformMatrix3fv(shader->NORMAL_MATRIX,1,true,(GLfloat*)&normal);
    
    vertices.Bind(0);
    tex_coords.Bind(1);
    normals.Bind(2);
    
    glDrawArrays(GL_TRIANGLES,0,vertex_count);
    glDisableVertexAttribArray(shader->ATTRIB_TEXCOORD);
    glDisableVertexAttribArray(shader->ATTRIB_VERTEX);
    glDisableVertexAttribArray(shader->ATTRIB_NORMAL);
    glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
}
 

