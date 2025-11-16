#ifndef PRIMITIVE_H
#define PRIMITIVE_H

#include <game_project_1/gfx/camera.hpp>
#include <game_project_1/gfx/material.hpp>
#include <game_project_1/types/math_types.hpp>

class Drawable{
    public:
    Transform offset;
    char layer;
    bool hidden;
    ShaderRef shader;
    Drawable();
    Drawable(const Drawable& other);
    Drawable& operator= (const Drawable& other);
    
    virtual void Draw(Camera* cam)=0;

    Drawable(Transform* parent);
    void Copy(Drawable* copy);
    virtual ~Drawable();
};

struct VBO{
    GLuint buffer_id;
    GLuint element_type;
    short  elements_per_vertex;

    VBO();
    bool Valid();
    //void Create(void* data,GLuint type,int per_vertex,int count);
    void Create(void* data,GLuint type,int per_vertex,int count,GLuint buffer_type);
    //void CreateEmpty(GLuint type,int per_vertex,int count,GLuint buffer_type);
    void CreateEmptyWeights(GLuint type,int per_vertex,int count,GLuint buffer_type);
    void Bind(int attrib_slot);
    void Bind(int attrib_slot, int stride, int offset);
    void Destroy();
};

#endif