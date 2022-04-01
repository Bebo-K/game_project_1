#include <game_project_1/component/gfx_components.hpp>
#include <game_project_1/types/math_types.hpp>
#include <game_project_1/game/entity.hpp>
#include <game_project_1/system/camera_manager.hpp>

CameraTarget::CameraTarget(Camera* cam,vec3 cam_offset,FloatRange zoom_scale,FloatRange tilt){
    camera = cam;
    offset=cam_offset;
    rotation.clear();
    zoom_range = zoom_scale;
    zoom_pitch = tilt;
    zoom = zoom_range.Average();
    rotation.rotate_by(zoom_pitch.ScaleTo(zoom,zoom_range),0,0);
}

CameraTarget::~CameraTarget(){}


Model* ModelSet::Add(ModelID model_id){
    return new (Allocate()) Model(model_id);
}

void ModelSet::Draw(Camera* cam){
    cam->view_matrix.translate(x,y,z);
    cam->view_matrix.rotate(rotation);
    cam->view_matrix.scale(scale);
    for(Model* m: (*this)){
        m->Draw(cam);
    }
}


ModelSet::~ModelSet(){Clear();}


void SpriteSet::Draw(Camera* cam){
    for(Sprite* s: (*this)){
        s->Draw();
    }
}
SpriteSet::~SpriteSet(){Clear();}
