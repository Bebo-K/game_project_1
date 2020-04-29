#include "gfx_components.h"
#include "../../structs/math_types.h"

CameraTarget::CameraTarget(Camera* c,vec3 cam_offset,float_range zoom_scale,float_range tilt){
    camera = c;;
    offset=cam_offset;
    rotation.clear();
    zoom_range = zoom_scale;
    zoom_pitch = tilt;
    zoom = zoom_range.Average();
    rotation.rotate_by(zoom_pitch.ScaleTo(zoom),0,0);
}

CameraTarget::~CameraTarget(){}

Model* ModelSet::Add(char* name){
    Model* ret = New();
    ModelManager::Get(name)->Clone(ret);
    return ret;
}

void ModelSet::Draw(Camera* cam,mat4* view, mat4* projection){
    view->scale(scale);
    view->rotate(rotation);
    view->translate(x,y,z);
    for(int i=Begin();Has(i);i=Next(i)){
        (*this)[i]->Draw(cam,view,projection);
    }
}

ModelSet::~ModelSet(){Clear();}



void SpriteSet::Draw(Camera* cam,mat4* view, mat4* projection){

}
SpriteSet::~SpriteSet(){Clear();}
