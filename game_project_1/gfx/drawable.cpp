#include <game_project_1/gfx/drawable.hpp>
#include <game_project_1/io/log.hpp>


Drawable::Drawable(){ 
    x=0;y=0;z=0;
    scale={1,1,1};
    rotation={0,0,0};
    hidden=false;
    layer=1;
    shader_name=null;
}

Drawable::~Drawable(){}


void Drawable::SetLocation(Location l){
    x=l.position.x;
    y=l.position.y;
    z=l.position.z;
    rotation = l.rotation;
    scale = l.scale;
}

VBO::VBO(){
    buffer_id=0;
    element_type=0;
    elements_per_vertex=0;
}

bool VBO::Valid(){
    return buffer_id > 0;
}
/*
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
    CheckForGLError("VBO.Create: GL Error %d\n");
}
*/

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
    CheckForGLError("VBO.Create: prefix GL Error %d\n");
    glGenBuffers(1,&buffer_id);
    glBindBuffer(buffer_type, buffer_id);//GL_ARRAY_BUFFER OR GL_ELEMENT_ARRAY_BUFFER
    glBufferData(buffer_type,element_size*elements_per_vertex*vertex_count, data,GL_STATIC_DRAW);
    CheckForGLError("VBO.Create: GL Error %d\n");
}

/*
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
*/


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

    CheckForGLError("VBO.CreateEmptyWeights: GL Error %d\n");
}


void VBO::Bind(int attrib_slot){
    glBindBuffer(GL_ARRAY_BUFFER,buffer_id);
    if(element_type <= GL_UNSIGNED_INT){
        glVertexAttribIPointer(attrib_slot,elements_per_vertex,element_type,0,0);
    }
    else{
        glVertexAttribPointer(attrib_slot,elements_per_vertex,element_type,false,0,0);
    }
    CheckForGLError("VBO.Bind: GL Error %d\n");
}

/**/
void VBO::Bind(int attrib_slot, int stride, int offset){
    byte* ofs = (byte*)(null) + offset;
    glBindBuffer(GL_ARRAY_BUFFER,buffer_id);
    glVertexAttribPointer(attrib_slot,elements_per_vertex,element_type,false,stride,(void*)ofs);
    CheckForGLError("VBO.Bind: GL Error %d\n");
}


void VBO::Destroy(){
    if(Valid())glDeleteBuffers(1,&buffer_id);
    CheckForGLError("VBO.Destroy: GL Error %d\n");
    buffer_id = -1;
}
