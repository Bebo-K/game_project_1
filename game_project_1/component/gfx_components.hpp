#ifndef GFX_COMPONENTS_H
#define GFX_COMPONENTS_H

#include <game_project_1/component/component.hpp>
#include <game_project_1/types/pool.hpp>
#include <game_project_1/types/math_types.hpp>
#include <game_project_1/types/3d_types.hpp>
#include <game_project_1/gfx/camera.hpp>
#include <game_project_1/gfx/model.hpp>
#include <game_project_1/gfx/sprite.hpp>
#include <game_project_1/gfx/animation.hpp>
#include <game_project_1/gfx/drawable.hpp>

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
	void Draw(Camera* cam);

	~SpriteSet();
};

#endif