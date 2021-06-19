#include "gfx_components.h"
#include "../../struct/math_types.h"
#include "../entity.h"
#include "../system/camera_manager.h"

CameraTarget::CameraTarget(Camera* cam,vec3 cam_offset,float_range zoom_scale,float_range tilt){
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
    Model* ret = new Model(model_id);
    data.Add(ret);
    count++;
    return ret;
}

void ModelSet::Draw(Camera* cam,mat4* view, mat4* projection){
    view->translate(x,y,z);
    view->rotate(rotation);
    view->scale(scale);
    for(Model* m: (*this)){
        m->Draw(cam,view,projection);
    }
}


ModelSet::~ModelSet(){Clear();}


void SpriteSet::Draw(Camera* cam,mat4* view, mat4* projection){

}
SpriteSet::~SpriteSet(){Clear();}
