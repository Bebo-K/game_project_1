#ifndef PRIMITIVE_H
#define PRIMITIVE_H

#include "camera.h"

class Primitive{
    float x,y,z;
    vec3    rotation;
    vec3    scale;
    //Texture* texture;
    GLuint  vertex_buffer, texcoord_buffer;
    unsigned int vertex_count;
    bool hide;


    void draw(Camera* cam);
};


#endif