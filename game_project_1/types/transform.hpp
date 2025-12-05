#ifndef TRANSFORM_HPP
#define TRANSFORM_HPP

#include <game_project_1/types/primitives.hpp>
#include <game_project_1/types/map.hpp>

//A coordinate space relative to either nothing (global space) or another transform
class Transform{
    public:
        float x,y,z;
        quaternion rotation;
        vec3 scale;
        Transform* parent;
        
    public:
    Transform();
    Transform(float x,float y,float z);
    Transform(float x,float y,float z,vec3 rotation);
    Transform(float x,float y,float z,quaternion rotation);
    Transform(float x,float y,float z,vec3 rotation,vec3 scale);
    Transform(float x,float y,float z,quaternion rotation,vec3 scale);
    Transform(float x,float y,float z,quaternion rotation,vec3 scale,Transform* parent);
    Transform(vec3 pos);
    Transform(vec3 pos,vec3 rotation);
    Transform(vec3 pos,quaternion rotation);
    Transform(vec3 pos,vec3 rotation,vec3 scale);
    Transform(vec3 pos,quaternion rotation,vec3 scale);
    Transform(vec3 pos,quaternion rotation,vec3 scale,Transform* parent);
    Transform(Transform* parent);

    
    Transform(const Transform& other);
    Transform& operator= (const Transform& other);
    ~Transform();

    void operator= (Transform& copy);
    void Copy(Transform* copy);
    void Clear();

    void AttachTo(Transform* new_parent);//converts this transform to local space
    void Unattach();//converts this transform to world space

    vec3 ToGlobalSpace(vec3 local_point);
    vec3 ToLocalSpace(vec3 global_point);

    void ApplyTo(mat4& space_matrix);//forward-applies transformations to a matrix, resulting in a global transform matrix including this transform
    void ApplyFrom(mat4& view_matrix);//reverse-applies transformations to a matrix, resulting in a matrix relative to this transform

    vec3 GlobalPosition();
    quaternion GlobalRotation();
    vec3 GlobalScale();

    vec3 Position();
    void SetPosition(vec3 pos);
    void SetLocation(vec3 pos,vec3 rotation);

    void operator+= (vec3 position);
    void operator-= (vec3 position);
    void Translate(float x,float y,float z);

    void Rotate(float rx,float ry,float rz);
    void Rotate(vec3 r);
    void RotateRadians(vec3 r);
    void Rotate(quaternion r);

    void Scale(float sx,float sy,float sz);
    void Scale(vec3 s);
    void Scale(float s);
    
    static inline int SerializedLength(){return sizeof(vec3)*2+ sizeof(quaternion);}
};


#endif