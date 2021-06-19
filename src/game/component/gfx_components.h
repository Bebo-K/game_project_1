#ifndef GFX_COMPONENTS_H
#define GFX_COMPONENTS_H

#include "component.h"
#include "../../struct/list.h"
#include "../../struct/math_types.h"
#include "../../struct/3d_types.h"
#include "../../gfx/camera.h"
#include "../../gfx/model.h"
#include "../../gfx/ui_sprite.h"
#include "../../gfx/animation.h"
#include "../../gfx/drawable.h"

struct Entity;

class CameraTarget : Component{
	public:
	Camera*			camera;
	float			zoom;
	vec3			offset;
    quaternion      rotation;
	float_range		zoom_range;
	float_range		zoom_pitch;

	CameraTarget(Camera* cam,vec3 cam_offset,float_range zoom_scale,float_range tilt);
    ~CameraTarget();
};

class ModelSet : public List<Model> ,public Component,public Drawable{
	public:
	
	Model* Add(ModelID model_id);
	void Draw(Camera* cam,mat4* view, mat4* projection);
	~ModelSet();
};

class SpriteSet : List<Sprite> , Component, Drawable{
	public:

	//Sprite* Add(char* name);
	void Draw(Camera* cam,mat4* view, mat4* projection);

	~SpriteSet();
};

#endif