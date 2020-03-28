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
ModelSet::~ModelSet(){Clear();}

SpriteSet::~SpriteSet(){Clear();}
