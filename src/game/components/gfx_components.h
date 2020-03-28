#ifndef GFX_COMPONENTS_H
#define GFX_COMPONENTS_H

#include "../component.h"
#include "../../structs/list.h"
#include "../../structs/math_types.h"
#include "../../structs/3d_types.h"
#include "../../gfx/camera.h"
#include "../../gfx/model.h"
#include "../../gfx/sprite.h"
#include "../../gfx/animation.h"


class CameraTarget : Component{
	public:
	float			zoom;
    Camera* 		camera;
	vec3			offset;
    quaternion      rotation;
	float_range		zoom_range;
	float_range		zoom_pitch;

	CameraTarget(Camera* c,vec3 cam_offset,float_range zoom_scale,float_range tilt);
    ~CameraTarget();
};

class ModelSet : List<Model> , Component{
	public:
	
	Model* Add(char* name);

	~ModelSet();
};

class SpriteSet : List<Sprite> , Component{
	public:

	//Sprite* Add(char* name);

	~SpriteSet();
};

#endif