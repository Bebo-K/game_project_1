#include <game_project_1/gfx/renderer.hpp>
#include <game_project_1/config.hpp>
#include <game_project_1/io/log.hpp>

Renderer::Renderer():camera(),primitives(8){
    glClearColor(0.1, 0.1, 0.1, 1.0);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_DEPTH_TEST);
}
void Renderer::Resize(int w,int h){
    camera.width=w;
    camera.height=h;
}
void Renderer::Clear(){
    primitives.Clear();
}

void Renderer::Add(Drawable* p){
    primitives.Add(p);
}

void Renderer::Remove(Drawable* p){
    primitives.Remove(p);
}

void Renderer::Draw(){
    if(primitives.Count() <= 0){return;}
    
    Drawable** sorted_list = SortPrimitives();
    for(int p=0; p < primitives.Count(); p++){
        sorted_list[p]->Draw(&camera);
    }
    free(sorted_list);
}

boolean PrimitiveIsFarther(Drawable* p1,Drawable* p2,vec3 camera_pos,vec3 camera_axis){
    if(p1->layer < p2->layer)return false;
    if(p1->layer > p2->layer)return true;
    float p1_zdist = camera_axis.dot({p1->offset.x-camera_pos.x, p1->offset.y-camera_pos.y, p1->offset.z-camera_pos.z});
    float p2_zdist = camera_axis.dot({p2->offset.x-camera_pos.x, p2->offset.y-camera_pos.y, p2->offset.z-camera_pos.z});

    return p1_zdist >= p2_zdist;
}

//return a sorted list of primitive pointers based on layer and distance on camera's z axis
//From farthest/highest layer to closest/lowest layer
Drawable** Renderer::SortPrimitives(){
	Drawable** ret = new Drawable*[primitives.Count()];
    memset(ret,0,sizeof(Drawable*)*primitives.Count());

    vec3 camera_axis = {0.0f,0.0f,1.0f};
    vec3 camera_pos = {camera.transform.x,camera.transform.y,camera.transform.z};

    camera.ViewMatrix().multiply_vec3(&camera_axis);
    
    //insertion sort because it's quick and works well if we pre-sort. Also am lazy
    Drawable* to_insert =  null;
    Drawable* current = null;
    int last=0;
    for(Drawable* p: primitives){
        to_insert = p;
        for(int i=0;i <last;i++){
            current=ret[i];
            if(PrimitiveIsFarther(to_insert,current,camera_pos,camera_axis)){
                ret[i] = to_insert;
                to_insert=current;
            }
        }
        ret[last]=to_insert;
        last++;
    }

    return ret;
}
