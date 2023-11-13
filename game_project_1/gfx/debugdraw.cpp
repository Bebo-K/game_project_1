#include <game_project_1/gfx/debugdraw.hpp>
#include <game_project_1/gfx/drawable.hpp>
#include <game_project_1/io/log.hpp>
#include <math.h>


float rect3d_vert_data[] = { -0.5f, -0.5f, -0.5f,  -0.5f, -0.5f, 0.5f,  -0.5f, 0.5f, -0.5f,  -0.5f, 0.5f, 0.5f,  
    0.5f, -0.5f, -0.5f,  0.5f, -0.5f, 0.5f,  0.5f, 0.5f, -0.5f,  0.5f, 0.5f, 0.5f };
int rect3d_vert_count = 36;
GLuint rect3d_vertex_array_id;

float* sphere_vert_data;
int sphere_vert_count;
GLuint sphere_vertex_array_id;

float* cylinder_vert_data;
int cylinder_vert_count;
GLuint cylinder_vertex_array_id;


void InitDebugVertexBuffer(GLuint* array,float* verts,int vert_count){
    glGenBuffers(1,array);
    glBindBuffer(GL_ARRAY_BUFFER, *array);
    glBufferData(GL_ARRAY_BUFFER,sizeof(float)*3*vert_count, verts, GL_STATIC_DRAW);
    
    int gl_err = glGetError();
    if(gl_err != 0){logger::warn("GL error initializing primitive: %d",&gl_err);}
}

void CalcualateCylinderVertices(){
    int segments = 18;
    float segment_arc = PI_OVER_180 * 360/segments;
    cylinder_vert_count = segments * 18;
    
    cylinder_vert_data = new float[cylinder_vert_count];
    vec3 face[] = { {0,0,0},{0,0,0},{0,0,0},  {0,0,0},{0,0,0},{0,0,0} };

    for(int i=0;i<segments;i++){
        float x1 = 0.5f* sinf(i*segment_arc);
        float x2 = 0.5f* sinf((i+1)*segment_arc);
        float y1 = 0.5f;
        float y2 = -0.5f;
        float z1 = 0.5f* cosf(i*segment_arc);
        float z2 = 0.5f* cosf((i+1)*segment_arc);


        face[0] = {x1,y1,z1}; face[1] = {x2,y1,z2};
        face[2] = {x1,y2,z1};
                              face[3] = {x2,y1,z2};
        face[5] = {x1,y2,z1}; face[4] = {x2,y2,z2};

        memcpy(&cylinder_vert_data[i*18],(float*)face,sizeof(float)*18);
    }
}

void CalcualateSphereVertices(){
    int h_facets = 12;float h_seg_arc = PI_OVER_180 * 360/h_facets;
    int v_facets = 8;float v_seg_arc = PI_OVER_180 * 360/v_facets;
    cylinder_vert_count = h_facets * v_facets * 18;
    
    cylinder_vert_data = new float[cylinder_vert_count];
    vec3 face[] = { {0,0,0},{0,0,0},{0,0,0},  {0,0,0},{0,0,0},{0,0,0} };

    for(int h=0;h<h_facets;h++){
        for(int v=0;v<v_facets;v++){
            float x1 = 0.5f* cos( v * v_seg_arc ) * cos( h * h_seg_arc );
            float x2 = 0.5f* cos( v * v_seg_arc ) * cos( (h+1) * h_seg_arc);
            float x3 = 0.5f* cos( (v+1) * v_seg_arc ) * cos( h * h_seg_arc );
            float x4 = 0.5f* cos( (v+1) * v_seg_arc ) * cos( (h+1) * h_seg_arc);

            float y1 = 0.5f* sinf( v * v_seg_arc );
            float y2 = 0.5f* sinf( (v+1) * v_seg_arc );

            float z1 = 0.5f* cos( v * v_seg_arc ) * sin( h * h_seg_arc );
            float z2 = 0.5f* cos( v * v_seg_arc ) * sin( (h+1) * h_seg_arc);
            float z3 = 0.5f* cos( (v+1) * v_seg_arc ) * sin( h * h_seg_arc );
            float z4 = 0.5f* cos( (v+1) * v_seg_arc ) * sin( (h+1) * h_seg_arc);

            face[0] = {x1,y1,z1}; face[1] = {x2,y1,z2};
            face[2] = {x3,y2,z3};
                                  face[3] = {x2,y1,z2};
            face[5] = {x3,y2,z3}; face[4] = {x4,y2,z4};

            memcpy(&sphere_vert_data[ (v*v_facets + h) *18],(float*)face,sizeof(float)*18);
        }
    }
}

void DebugDraw::Init(){
    ShaderManager::UseShader("shape_debug");
    glEnableVertexAttribArray(Shader::ATTRIB_VERTEX);
    glBindVertexArray(0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    CalcualateCylinderVertices();
    CalcualateSphereVertices();

    InitDebugVertexBuffer(&rect3d_vertex_array_id,rect3d_vert_data,rect3d_vert_count);
    InitDebugVertexBuffer(&sphere_vertex_array_id,sphere_vert_data,sphere_vert_count);
    InitDebugVertexBuffer(&cylinder_vertex_array_id,cylinder_vert_data,cylinder_vert_count);
}

void DebugDraw::DrawUIRect(int x,int y,int w,int h,color_f color){
    //TODO;
}

void DebugDraw::DrawCube(Camera* cam,Transform t,float size,color_f color){ Draw3DRect(cam,t,vec3(size,size,size),color);}
void DebugDraw::Draw3DRect(Camera* cam,Transform t,vec3 size,color_f color){
    Shader* shape_shader = ShaderManager::UseShader("shape_debug");
    
    cam->view_matrix.translate(t.x,t.y,t.z);
    cam->view_matrix.rotate(t.rotation);
    cam->view_matrix.scale(t.scale);
    cam->view_matrix.scale(size);

    glBindVertexArray(rect3d_vertex_array_id);
    CheckForGLError("DebugDraw.DrawCapsule: GL Error %d binding vertex array\n");

    glUniform4fv(shape_shader->COLOR,1,(GLfloat*)&color);
    glDrawArrays(GL_TRIANGLES,0,rect3d_vertex_array_id);
 
    CheckForGLError("Debug.DrawCapsule: GL Error %d during draw\n");
}

void DebugDraw::DrawSphere(Camera* cam,Transform t,float radius,color_f color){ DrawEllipse(cam,t,radius,radius,color);}
void DebugDraw::DrawEllipse(Camera* cam,Transform t,float height,float radius,color_f color){
    Shader* shape_shader = ShaderManager::UseShader("shape_debug");
    
    cam->view_matrix.translate(t.x,t.y,t.z);
    cam->view_matrix.rotate(t.rotation);
    cam->view_matrix.scale(t.scale);
    cam->view_matrix.scale(radius,height,radius);

    glBindVertexArray(sphere_vertex_array_id);
    CheckForGLError("DebugDraw.DrawCapsule: GL Error %d binding vertex array\n");

    glUniform4fv(shape_shader->COLOR,1,(GLfloat*)&color);
    glDrawArrays(GL_TRIANGLES,0,sphere_vert_count);
 
    CheckForGLError("Debug.DrawCapsule: GL Error %d during draw\n");
}

void DebugDraw::DrawCapsule(Camera* cam,Transform t,float height,float radius,color_f color){
    Shader* shape_shader = ShaderManager::UseShader("shape_debug");
    
    cam->view_matrix.translate(t.x,t.y,t.z);
    cam->view_matrix.rotate(t.rotation);
    cam->view_matrix.scale(t.scale);

    glBindVertexArray(cylinder_vertex_array_id);
    CheckForGLError("DebugDraw.DrawCapsule: GL Error %d binding vertex array\n");

    glUniform4fv(shape_shader->COLOR,1,(GLfloat*)&color);
    glDrawArrays(GL_TRIANGLES,0,cylinder_vertex_array_id);
    
    //Draw end caps
    CheckForGLError("Debug.DrawCapsule: GL Error %d during draw\n");
}