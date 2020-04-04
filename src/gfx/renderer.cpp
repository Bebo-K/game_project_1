#include "renderer.h"
#include "../config.h"
#include "../log.h"

Renderer::Renderer():camera(),primitives(){
    view_matrix.identity();
    projection_matrix.identity();

    glClearColor(0.1, 0.1, 0.1, 1.0);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_DEPTH_TEST);
}

void Renderer::Load(){
    camera.SetShader(ShaderManager::DefaultShader());

    camera.width= config::ui_width;
    camera.height= config::ui_height;

    primitives.Resize(8);
}

void Renderer::Unload(){
    for(int i=0;i<primitives.slots;i++){
        primitives.Remove(i);
    }
    primitive_count=0;
}

void Renderer::Add(Primitive* p){
    primitives.Add(p);
    primitive_count++;
}

void Renderer::Remove(Primitive* p){
    primitives.Remove(p);
    primitive_count--;
}


void Renderer::Draw(){
    if(primitive_count <= 0){return;}
    camera.shader->Use();
    
    view_matrix.identity(); 
    if(camera.ortho == true){
        projection_matrix.ortho(camera.width,camera.height,camera.near_clip,camera.far_clip);
    }
    else{
        projection_matrix.perspective(camera.width,camera.height,camera.near_clip,camera.far_clip,camera.fov);
    }
     
    camera.ToCameraSpace(&view_matrix);

    mat4 view,projection;
    Primitive** sorted_list = SortPrimitives();
    for(int p=0; p < primitive_count; p++){
        view.set(&view_matrix);
        projection.set(&projection_matrix); 
        sorted_list[p]->Draw(&camera,&view,&projection);
    }
    free(sorted_list);
}

boolean PrimitiveIsCloser(Primitive* p1,Primitive* p2,vec3 camera_pos,vec3 camera_axis){
    if(p1->layer < p2->layer)return true;
    if(p2->layer > p1->layer)return false;
    float p1_zdist = camera_axis.dot({p1->x-camera_pos.x, p1->y-camera_pos.y, p1->z-camera_pos.z});
    float p2_zdist = camera_axis.dot({p2->x-camera_pos.x, p2->y-camera_pos.y, p2->z-camera_pos.z});

    return p1_zdist <= p2_zdist;
}

//return a sorted list of primitive pointers based on layer and distance on camera's z axis
Primitive** Renderer::SortPrimitives(){
	Primitive** ret = new Primitive*[primitive_count];

    vec3 camera_axis = {0.0f,0.0f,1.0f};
    vec3 camera_pos = {camera.x,camera.y,camera.z};

    view_matrix.multiply_vec3(&camera_axis);	
    
    //insertion sort because it's quick and works well if we pre-sort. Also am lazy
    Primitive* to_insert =  null;
    Primitive* current = null;
    int last=0;
    for(int p=0;p<primitives.slots;p++){
        to_insert=(Primitive*)primitives.Get(p);
        if(to_insert==null)continue;

        for(int i=0;i <last;i++){
            current=ret[i];
            if(PrimitiveIsCloser(to_insert,current,camera_pos,camera_axis)){
                ret[i] = to_insert;
                to_insert=current;
            }
        }
        ret[last]=to_insert;
        last++;
    }
    return ret;
}
