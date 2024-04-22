#ifndef SPRITE_SET_H
#define SPRITE_SET_H

#include <game_project_1/component/component.hpp>
#include <game_project_1/types/pool.hpp>
#include <game_project_1/gfx/drawable.hpp>
#include <game_project_1/types/primitives.hpp>
#include <game_project_1/gfx/camera.hpp>
#include <game_project_1/gfx/sprite.hpp>


class SpriteSet: public Component, public Pool<Sprite>,public Drawable{
	public:
	SpriteSet();
	~SpriteSet();

	//Sprite* Add(char* name);
	virtual void Draw(Camera* cam);
	void SetPosition(vec3 pos);

    Component* Clone();
    virtual void Clear();

    inline int SerializedLength(){return 0;}
    inline void Read(Deserializer& dat){}
    inline void Write(Serializer& dat){}
};


#endif