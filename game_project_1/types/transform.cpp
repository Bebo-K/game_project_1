#include <game_project_1/types/transform.hpp>
#include <game_project_1/io/log.hpp>


Transform::Transform():x(0),y(0),z(0),rotation(),scale(1.0f,1.0f,1.0f),parent(null){};
Transform::Transform(float _x,float _y,float _z):x(_x),y(_y),z(_z),rotation(),scale(1.0f,1.0f,1.0f),parent(null){}
Transform::Transform(float _x,float _y,float _z,vec3 _rotation):x(_x),y(_y),z(_z),scale(1.0f,1.0f,1.0f),parent(null){rotation =quaternion::of_euler(_rotation.x,_rotation.y,_rotation.z);}
Transform::Transform(float _x,float _y,float _z,quaternion _rotation):x(_x),y(_y),z(_z),rotation(_rotation),scale(1.0f,1.0f,1.0f),parent(null){}
Transform::Transform(float _x,float _y,float _z,vec3 _rotation,vec3 _scale):x(_x),y(_y),z(_z),rotation(),scale(_scale),parent(null){rotation =quaternion::of_euler(_rotation.x,_rotation.y,_rotation.z);}
Transform::Transform(float _x,float _y,float _z,quaternion _rotation,vec3 _scale):x(_x),y(_y),z(_z),rotation(_rotation),scale(_scale),parent(null){}
Transform::Transform(float _x,float _y,float _z,quaternion _rotation,vec3 _scale,Transform* _parent):x(_x),y(_y),z(_z),rotation(_rotation),scale(_scale),parent(_parent){}

Transform::Transform(vec3 pos):x(pos.x),y(pos.y),z(pos.z),rotation(),scale(1.0f,1.0f,1.0f),parent(null){}
Transform::Transform(vec3 pos,vec3 _rotation):x(pos.x),y(pos.y),z(pos.z),scale(1.0f,1.0f,1.0f),parent(null){rotation =quaternion::of_euler(_rotation.x,_rotation.y,_rotation.z);}
Transform::Transform(vec3 pos,quaternion _rotation):x(pos.x),y(pos.y),z(pos.z),rotation(_rotation),scale(1.0f,1.0f,1.0f),parent(null){}
Transform::Transform(vec3 pos,vec3 _rotation,vec3 _scale):x(pos.x),y(pos.y),z(pos.z),scale(_scale),parent(null){rotation =quaternion::of_euler(_rotation.x,_rotation.y,_rotation.z);}
Transform::Transform(vec3 pos,quaternion _rotation,vec3 _scale):x(pos.x),y(pos.y),z(pos.z),rotation(_rotation),scale(_scale),parent(null){}
Transform::Transform(vec3 pos,quaternion _rotation,vec3 _scale,Transform* parent):x(pos.x),y(pos.y),z(pos.z),rotation(_rotation),scale(_scale),parent(parent){}

Transform::Transform(Transform* _parent):x(0),y(0),z(0),rotation(),scale(1.0f,1.0f,1.0f),parent(_parent){}

Transform::Transform(const Transform& other){
    x=other.x;y=other.y;z=other.z;
    rotation=other.rotation;
    scale=other.scale;
    parent=other.parent;
}
Transform& Transform::operator= (const Transform& other){
    x=other.x;y=other.y;z=other.z;
    rotation=other.rotation;
    scale=other.scale;
    parent=other.parent;
    return *this;
}
Transform::~Transform(){
    parent=null;
    x=y=z=0;
    scale={1.0f,1.0f,1.0f};
    rotation=quaternion::identity();
}
void Transform::Copy(Transform* copy){
    x=copy->x;y=copy->y;z=copy->z;
    rotation=copy->rotation;
    scale=copy->scale;
    parent=copy->parent;
}

void Transform::Clear(){
    x=y=z=0;
    scale= {1.0f,1.0f,1.0f};
    rotation = quaternion::identity();
}

//converts this transform to local space of a new parent, setting it's parent while maintaining it's position in global space
void Transform::AttachTo(Transform* new_parent){
    if(parent != null)Unattach();
    vec3 new_pos = new_parent->ToLocalSpace({x,y,z});
    quaternion new_rot = rotation * new_parent->GlobalRotation().inverse();
    vec3 new_scale = scale / new_parent->GlobalScale();
}

//converts this transform to world space, unsetting all parents while maintaining it's position in global space
void Transform::Unattach(){
    if(parent==null)return;
    vec3 new_pos = GlobalPosition();
    quaternion new_rot = GlobalRotation();
    vec3 new_scale = GlobalScale();

    x=new_pos.x;
    y=new_pos.y;
    z=new_pos.z;

    rotation = new_rot;
    scale = new_scale;
}


vec3 Transform::ToGlobalSpace(vec3 local_point){
    vec3 parent_point = ( (local_point + vec3{x,y,z}).rotate(rotation) ) * scale;
    if(parent != null){return parent->ToGlobalSpace(parent_point);}
    return parent_point;
}
vec3 Transform::ToLocalSpace(vec3 world_point){
    vec3 parent_point = (parent != null)?parent->ToLocalSpace(world_point): world_point;
    //reverse operations in reverse order, de-scale, then inverser rotation, then translate backwards
    return (parent_point / scale).rotate(rotation.inverse()) - Position();
}

void Transform::ApplyTo(mat4& space_matrix){
    space_matrix.translate(x,y,z);
    space_matrix.rotate(rotation);
    space_matrix.scale(scale.x,scale.y,scale.z);
    if(parent != null){parent->ApplyTo(space_matrix);}
}

void Transform::ApplyFrom(mat4& view_matrix){
    if(parent != null){parent->ApplyFrom(view_matrix);}
    view_matrix.scale(1.0f/scale.x,1.0f/scale.y,1.0f/scale.z);
    view_matrix.rotate(rotation.inverse());
    view_matrix.translate(-x,-y,-z);
}

vec3 Transform::GlobalPosition(){return ToGlobalSpace({0,0,0});}
quaternion Transform::GlobalRotation(){
    if(parent != null){return parent->GlobalRotation() * rotation;}
    return rotation;
}
vec3 Transform::GlobalScale(){
    if(parent != null){return parent->GlobalScale() * scale;}
    return scale;
}

vec3 Transform::Position(){return {x,y,z};}
void Transform::SetPosition(vec3 pos){x=pos.x; y=pos.y; z=pos.z;}
void Transform::SetLocation(vec3 pos,vec3 rot){x=pos.x; y=pos.y; z=pos.z; rotation = quaternion::of_euler(rot.x,rot.y,rot.z);}

void Transform::operator+= (vec3 pos){x+=pos.x; y+=pos.y; z+=pos.z;}
void Transform::operator-= (vec3 pos){x-=pos.x; y-=pos.y; z-=pos.z;}
void Transform::Translate(float _x,float _y,float _z){x+=_x; y+=_y; z+=_z;}

void Transform::Rotate(float rx,float ry,float rz){rotation.rotate_by(rx,ry,rz);}
void Transform::Rotate(vec3 r){rotation.rotate_by(r.x,r.y,r.z);}
void Transform::RotateRadians(vec3 r){rotation.rotate_by(quaternion::of_euler_radians(r.x,r.y,r.z));}
void Transform::Rotate(quaternion r){rotation.rotate_by(r);}

void Transform::Scale(float sx,float sy,float sz){scale *= {sx,sy,sz};}
void Transform::Scale(vec3 s){scale *= s;}
void Transform::Scale(float s){scale *= s;}
