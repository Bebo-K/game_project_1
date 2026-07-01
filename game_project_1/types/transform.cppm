export module Transform;

import Logger;
import Primitives;
import Map;


//A coordinate space relative to either nothing (global space) or another transform
export class Transform{
    public:
        float x,y,z;
        quaternion rotation;
        vec3 scale;
        Transform* parent;
    
    static inline int SerializedLength(){return sizeof(vec3)*2+ sizeof(quaternion);}//We don't serialize parent relations (should we?)
        
    Transform():x(0),y(0),z(0),rotation(),scale(1.0f,1.0f,1.0f),parent(nullptr){};
    Transform(float _x,float _y,float _z):x(_x),y(_y),z(_z),rotation(),scale(1.0f,1.0f,1.0f),parent(nullptr){}
    Transform(float _x,float _y,float _z,vec3 _rotation):x(_x),y(_y),z(_z),scale(1.0f,1.0f,1.0f),parent(nullptr){rotation =quaternion::of_euler(_rotation.x,_rotation.y,_rotation.z);}
    Transform(float _x,float _y,float _z,quaternion _rotation):x(_x),y(_y),z(_z),rotation(_rotation),scale(1.0f,1.0f,1.0f),parent(nullptr){}
    Transform(float _x,float _y,float _z,vec3 _rotation,vec3 _scale):x(_x),y(_y),z(_z),rotation(),scale(_scale),parent(nullptr){rotation =quaternion::of_euler(_rotation.x,_rotation.y,_rotation.z);}
    Transform(float _x,float _y,float _z,quaternion _rotation,vec3 _scale):x(_x),y(_y),z(_z),rotation(_rotation),scale(_scale),parent(nullptr){}
    Transform(float _x,float _y,float _z,quaternion _rotation,vec3 _scale,Transform* _parent):x(_x),y(_y),z(_z),rotation(_rotation),scale(_scale),parent(_parent){}

    Transform(vec3 pos):x(pos.x),y(pos.y),z(pos.z),rotation(),scale(1.0f,1.0f,1.0f),parent(nullptr){}
    Transform(vec3 pos,vec3 _rotation):x(pos.x),y(pos.y),z(pos.z),scale(1.0f,1.0f,1.0f),parent(nullptr){rotation =quaternion::of_euler(_rotation.x,_rotation.y,_rotation.z);}
    Transform(vec3 pos,quaternion _rotation):x(pos.x),y(pos.y),z(pos.z),rotation(_rotation),scale(1.0f,1.0f,1.0f),parent(nullptr){}
    Transform(vec3 pos,vec3 _rotation,vec3 _scale):x(pos.x),y(pos.y),z(pos.z),scale(_scale),parent(nullptr){rotation =quaternion::of_euler(_rotation.x,_rotation.y,_rotation.z);}
    Transform(vec3 pos,quaternion _rotation,vec3 _scale):x(pos.x),y(pos.y),z(pos.z),rotation(_rotation),scale(_scale),parent(nullptr){}
    Transform(vec3 pos,quaternion _rotation,vec3 _scale,Transform* parent):x(pos.x),y(pos.y),z(pos.z),rotation(_rotation),scale(_scale),parent(parent){}

    Transform(Transform* _parent):x(0),y(0),z(0),rotation(),scale(1.0f,1.0f,1.0f){parent = _parent;}

    Transform(const Transform& other){
        x=other.x;y=other.y;z=other.z;
        rotation=other.rotation;
        scale=other.scale;
        parent=other.parent;
    }
    Transform& operator= (const Transform& other){
        x=other.x;y=other.y;z=other.z;
        rotation=other.rotation;
        scale=other.scale;
        parent=other.parent;
        return *this;
    }
    ~Transform(){
        parent=nullptr;
        x=y=z=0;
        scale={1.0f,1.0f,1.0f};
        rotation=quaternion::identity();
    }
    void Copy(Transform* copy){
        x=copy->x;y=copy->y;z=copy->z;
        rotation=copy->rotation;
        scale=copy->scale;
        parent=copy->parent;
    }

    void Clear(){
        x=y=z=0;
        scale= {1.0f,1.0f,1.0f};
        rotation = quaternion::identity();
    }

    //converts this transform to local space of a new parent, setting it's parent while maintaining it's position in global space
    void AttachTo(Transform* new_parent){
        if(parent != nullptr)Unattach();
        vec3 old_pos = {x,y,z};
        vec3 new_pos = new_parent->ToLocalSpace(old_pos);
        quaternion new_rot = rotation * new_parent->GlobalRotation().inverse();
        vec3 new_scale = scale / new_parent->GlobalScale();
        x = new_pos.x;y = new_pos.y;z = new_pos.z;
        rotation = new_rot;
        scale = new_scale;
    }

    //converts this transform to world space, unsetting all parents while maintaining it's position in global space
    void Unattach(){
        if(parent==nullptr)return;
        vec3 new_pos = GlobalPosition();
        quaternion new_rot = GlobalRotation();
        vec3 new_scale = GlobalScale();

        x=new_pos.x;
        y=new_pos.y;
        z=new_pos.z;

        rotation = new_rot;
        scale = new_scale;
    }


    vec3 ToGlobalSpace(vec3& local_point){
        vec3 parent_point = (local_point*scale).rotate(rotation)+vec3{x,y,z}; 
        if(parent != nullptr){return parent->ToGlobalSpace(parent_point);}
        return parent_point;
    }
    vec3 ToLocalSpace(vec3& world_point){
        vec3 parent_point = (parent != nullptr)?parent->ToLocalSpace(world_point): world_point;
        //reverse operations in reverse order, de-translate, then inverse rotation, then de-scale
        return (parent_point-vec3{x,y,z}).rotate(rotation.inverse()) / scale;
    }

    void ApplyTo(mat4& space_matrix){
        if(parent != nullptr){parent->ApplyTo(space_matrix);}
        space_matrix.translate(x,y,z);
        space_matrix.rotate(rotation);
        space_matrix.scale(scale.x,scale.y,scale.z);
    }

    void ApplyFrom(mat4& view_matrix){
        view_matrix.scale(1.0f/scale.x,1.0f/scale.y,1.0f/scale.z);
        view_matrix.rotate(rotation.inverse());
        view_matrix.translate(-x,-y,-z);
        if(parent != nullptr){parent->ApplyFrom(view_matrix);}
    }

    vec3 GlobalPosition() {
        vec3 origin = {0,0,0};
        return ToGlobalSpace(origin);
    }
    quaternion GlobalRotation(){
        if(parent != nullptr){return parent->GlobalRotation() * rotation;}
        return rotation;
    }
    vec3 GlobalScale(){
        if(parent != nullptr){return parent->GlobalScale() * scale;}
        return scale;
    }

    vec3 Position(){return {x,y,z};}
    void SetPosition(vec3 pos){x=pos.x; y=pos.y; z=pos.z;}
    void SetLocation(vec3 pos,vec3 rot){x=pos.x; y=pos.y; z=pos.z; rotation = quaternion::of_euler(rot.x,rot.y,rot.z);}

    void operator+= (vec3 pos){x+=pos.x; y+=pos.y; z+=pos.z;}
    void operator-= (vec3 pos){x-=pos.x; y-=pos.y; z-=pos.z;}
    void Translate(float _x,float _y,float _z){x+=_x; y+=_y; z+=_z;}

    void Rotate(float rx,float ry,float rz){rotation.rotate_by(rx,ry,rz);}
    void Rotate(vec3 r){rotation.rotate_by(r.x,r.y,r.z);}
    void RotateRadians(vec3 r){rotation.rotate_by(quaternion::of_euler_radians(r.x,r.y,r.z));}
    void Rotate(quaternion r){rotation.rotate_by(r);}

    void Scale(float sx,float sy,float sz){scale *= {sx,sy,sz};}
    void Scale(vec3 s){scale *= s;}
    void Scale(float s){scale *= s;}
    
};



