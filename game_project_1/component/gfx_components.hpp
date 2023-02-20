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


class ModelSet:public ClientComponent, public Pool<Model> ,public Drawable{
	public:
	int ClientComponentID=0;
	
	Model* Add(ModelID model_id);
	virtual void Draw(Camera* cam);
	void SetPosition(vec3 pos);

	inline int ID(){return ModelSet::ClientComponentID;}
	ModelSet();
	~ModelSet();
};

class SpriteSet: public ClientComponent, public Pool<Sprite>,public Drawable{
	public:
	int ClientComponentID=1;

	//Sprite* Add(char* name);
	virtual void Draw(Camera* cam);

	inline int ID(){return SpriteSet::ClientComponentID;}
	~SpriteSet();
};

#endif