#ifndef GFX_COMPONENTS_H
#define GFX_COMPONENTS_H

#include "component.h"
#include "../../struct/pool.h"
#include "../../struct/math_types.h"
#include "../../struct/3d_types.h"
#include "../../gfx/camera.h"
#include "../../gfx/model.h"
#include "../../gfx/sprite.h"
#include "../../gfx/animation.h"
#include "../../gfx/drawable.h"

struct Entity;

class CameraTarget : Component{
	public:
	Camera*			camera;
	float			zoom;
	vec3			offset;
    quaternion      rotation;
	FloatRange		zoom_range;
	FloatRange		zoom_pitch;

	CameraTarget(Camera* cam,vec3 cam_offset,FloatRange zoom_scale,FloatRange tilt);
    ~CameraTarget();
};

class ModelSet : public Pool<Model> ,public Component,public Drawable{
	public:
	
	Model* Add(ModelID model_id);
	void Draw(Camera* cam);
	~ModelSet();
};

class SpriteSet : Pool<Sprite> , Component, Drawable{
	public:

	//Sprite* Add(char* name);
	void Draw(Camera* cam,mat4* view, mat4* projection);

	~SpriteSet();
};

#endif