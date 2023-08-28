#ifndef CLIENT_LEVEL_LOADER_H
#define CLIENT_LEVEL_LOADER_H

#include <game_project_1/gfx/camera.hpp>
#include <game_project_1/gfx/skybox.hpp>
#include <game_project_1/gfx/model.hpp>
#include <game_project_1/phys/level_colliders.hpp>

class ClientLevel:public Drawable{
    private:
    static char* default_shader;
    public:
    Skybox skybox;
    List<ModelData>     models;
    List<MeshCollider> collmeshes;


    ClientLevel();
    ~ClientLevel();
    virtual void Draw(Camera* cam);
    void LoadArea(int area_id);
    void Unload();
};




#endif