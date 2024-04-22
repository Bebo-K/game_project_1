#ifndef HITBOXES_H
#define HITBOXES_H

#include <game_project_1/component/component.hpp>
#include <game_project_1/phys/collider.hpp>
#include <game_project_1/gfx/animation.hpp>
#include <game_project_1/types/arrays.hpp>
#include <game_project_1/types/pool.hpp>

Animation::Target* BuildAnimationTargetForShapeCollider(ShapeCollider* s,Animation::Target* slot);

struct HitPath{
    ShapeCollider collider;
    Animation::Clip* path;
    float spawn_time;
    float despawn_time;

    HitPath();
    HitPath(ShapeCollider collider,Animation::Clip* path,float spawn_t,float despawn_t);
};

class HitPattern {
    public:
    Array<HitPath>  hitpaths;
    float max_lifetime;

    HitPattern(int hitpath_count);
    ~HitPattern();
};

class HitBoxes: public Component{
    public: 
    HitPattern* current_pattern;
    float       current_pattern_active_time;

    Pool<ShapeCollider> hit_colliders;
    Pool<Animation::Target> hit_collider_targets;
    //List<ShapeCollider> hittable_colliders; for now only the world hit capsule can recieve hits
    
	public: 
	HitBoxes();
	~HitBoxes();
    void CleanupPattern();

    Component* Clone();
    virtual void Clear();

    inline int SerializedLength(){return 0;}
    inline void Read(Deserializer& dat){}
    inline void Write(Serializer& dat){}

    
};




#endif