#ifndef GFX_COMPONENTS_H
#define GFX_COMPONENTS_H

#include <game_project_1/types/pool.hpp>
#include <game_project_1/types/math_types.hpp>
#include <game_project_1/types/3d_types.hpp>
#include <game_project_1/gfx/camera.hpp>
#include <game_project_1/gfx/model.hpp>
#include <game_project_1/gfx/sprite.hpp>
#include <game_project_1/gfx/animation.hpp>
#include <game_project_1/gfx/drawable.hpp>


class ModelSet : public Pool<Model> ,public Drawable{
	public:
	
	Model* Add(ModelID model_id);
	virtual void Draw(Camera* cam);
	void SetPosition(vec3 pos);
	ModelSet();
	~ModelSet();
};

class SpriteSet : public Pool<Sprite>,public Drawable{
	public:

	//Sprite* Add(char* name);
	virtual void Draw(Camera* cam);

	~SpriteSet();
};

#endif