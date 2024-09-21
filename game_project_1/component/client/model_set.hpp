#ifndef MODEL_SET_H
#define MODEL_SET_H

#include <game_project_1/component/component.hpp>
#include <game_project_1/types/pool.hpp>
#include <game_project_1/types/math_types.hpp>
#include <game_project_1/types/primitives.hpp>
#include <game_project_1/gfx/camera.hpp>
#include <game_project_1/gfx/model.hpp>
#include <game_project_1/gfx/sprite.hpp>
#include <game_project_1/gfx/drawable.hpp>


class ModelSet: public Pool<Model>,public Component,public Drawable{
	public:
	ModelSet();
	~ModelSet();

	Model* Add(ModelID model_id);
	virtual void Draw(Camera* cam);
	void SetPosition(vec3 pos);
	void StartAnimation(char* animation_name);
	void QueueAnimation(char* animation_name);
	void StopAnimation();
	void PauseAnimation();
	void SetAnimationTimescale(float timescale);

    Component* Clone();
    virtual void Clear();

    inline int SerializedLength(){return 0;}
    inline void Read(Deserializer& dat){}
    inline void Write(Serializer& dat){}
};



#endif