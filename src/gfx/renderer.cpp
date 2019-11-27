#include "renderer.h"

Renderer::Renderer(){
    modelview_matrix.identity();
    projection_matrix.identity();

    glClearColor(0.1, 0.1, 0.1, 1.0);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_DEPTH_TEST);
}

void Renderer::add(Primitive* p){
    //int first_empty_slot = -1;
    /*
    for(var i=0;i< draw_objects.length;i++){
        if(drawable === this.draw_objects[i])return;
        if(this.draw_objects[i]===null && first_empty_slot< 0){
            first_empty_slot = i;
        }
    }
    if(first_empty_slot > 0){
        this.draw_objects[first_empty_slot] = drawable;
    }
    else{
        this.draw_objects.push(drawable);
    }
    */
}

void Renderer::remove(Primitive* p){
/*
    for(var i=0;i<this.draw_objects.length;i++){
        if(drawable === this.draw_objects[i]){
            this.draw_objects[i] = null;
        }
    }
    */
}

void Renderer::draw(Camera* cam){
    /*
    camera->shader.Use();
    modelview_matrix.SetIdentity();
    if(cam->ortho == true){
        projection_matrix.SetOrtho(camera.width,camera.height,camera.near,camera.far);
    }
    else{
        projection_matrix.SetPerspective(camera.width,camera.height,camera.near,camera.far,camera.fov);
    }
    
    glEnableVertexAttribArray(camera.shader.VERTICES);
    glEnableVertexAttribArray(camera.shader.TEXCOORDS);

    camera.SetToCameraSpace(modelview_matrix);

    for(var i=0;i<draw_objects.length;i++){
        if(draw_objects[i]){
            draw_objects[i]->Draw(camera,&modelview_matrix,&projection_matrix);
        }
    }
    */
}