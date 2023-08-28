#include <game_project_1/types/3d_types.hpp>
#include <math.h>


vec4::vec4(){
    x=y=z=w=0;
}
vec4::vec4(float _x,float _y,float _z,float _w){
    x=_x;
    y=_y;
    z=_z;
    w=_w;
}

vec3::vec3(){
    x=y=z=0;
}

vec3::vec3(float _x,float _y,float _z){
    x=_x;
    y=_y;
    z=_z;
}

vec3::vec3(vec3* copy){
    x=copy->x;
    y=copy->y;
    z=copy->z;
}

void vec3::set(float _x,float _y,float _z){
    x=_x;
    y=_y;
    z=_z;
}
float vec3::dot(vec3 b){
    return x*b.x + y*b.y + z*b.z;
}

vec3 vec3::cross(vec3 b){
    float vx= y*b.z - z*b.y;
    float vy= z*b.x - x*b.z;
    float vz= x*b.y - y*b.x;
    return {vx,vy,vz};
}

float vec3::length(){
    return sqrtf(x*x+y*y+z*z);
}

float vec3::length_sqr(){
    return x*x+y*y+z*z;
}

float vec3::dist(struct vec3 b){
    float dx = b.x - x;
    float dy = b.y - y;
    float dz = b.z - z;
    return sqrtf(dx*dx + dy*dy +dz*dz);
}
float vec3::dist_sqr(struct vec3 b){
    float dx = b.x - x;
    float dy = b.y - y;
    float dz = b.z - z;
    return dx*dx + dy*dy +dz*dz;	
}

void vec3::rotate_x(float theta){
    float rx = -theta*PI_OVER_180;
    float cos = cosf(rx);
    float sin = sinf(rx);
    float _x = x;
    float _y = y*cos - z*sin;
    float _z = y*sin + z*cos;
    x = _x; y = _y; z = _z;
}

void vec3::rotate_y(float theta){
    float ry = theta*PI_OVER_180;
    float cos = cosf(ry);
    float sin = sinf(ry);
    float _x = x*cos + z*sin;
    float _y = y;
    float _z = z*cos - x*sin;
    x = _x; y = _y; z = _z;
}

void vec3::rotate_z(float theta){
    float rz = -theta*PI_OVER_180;
    float cos = cosf(rz);
    float sin = sinf(rz);
    float _x = x*cos - y*sin;
    float _y = x*sin + y*cos;
    float _z = z;
    x = _x; y = _y; z = _z;
}

vec3 vec3::operator +(vec3 v2){return{x+v2.x,y+v2.y,z+v2.z};}
vec3 vec3::operator -(vec3 v2){return{x-v2.x,y-v2.y,z-v2.z};}
vec3 vec3::operator *(float scl){return {x*scl,y*scl,z*scl};}
vec3 vec3::operator *(vec3 scl){return {x*scl.x,y*scl.y,z*scl.z};}
vec3 vec3::of_length(float newlen){
    float len = length();
    if(newlen == 0 || len ==0){return {0,0,0};}
    len = newlen/len;
    return {x*len,y*len,z*len};
}
vec3 vec3::normalized(){
    float len = length();
    if(len ==0){return {0,0,0};}
    return {x/len,y/len,z/len};
}

vec3 vec3::horizontal(){
    return {x,0,z};
}

vec3 vec3::clip(vec3 direction){
    float paralell_component = dot(direction);
    if(paralell_component < 0){
        return vec3(x,y,z) - (direction * paralell_component);
    }
    else return {x,y,z};
}

vec2 vec3::xz(){
    return {x,z};
}

vec3 vec3::zero(){return {0.0f,0.0f,0.0f};}



vec2::vec2(){
    x=y=0;
}

vec2::vec2(vec2* copy){
    x=copy->x;y=copy->y;
}

vec2::vec2(float _x,float _y){
    x=_x;y=_y;
}

float vec2::length(){
    return sqrtf(x*x+y*y);
}

float vec2::length_sqr(){
    return x*x+y*y;
}

void vec2::normalize(){
    float len = sqrtf(x*x+y*y);
    x /= len;
    y /= len;
}

vec2 vec2::normalized(){
    float len = length();
    if(len ==0){return {0,0};}
    return {x/len,y/len};
}

void vec2::rotate(float theta){
    float angle = theta*PI_OVER_180;
    float cos = cosf(angle);
    float sin = sinf(angle);
    float _x = x*cos + y*sin;
    float _y = y*cos - x*sin;
    x = _x; y = _y; 
}

void quaternion::clear(){
    x=y=z=0;
    w=1;
}

void quaternion::set_euler(float x,float y,float z){
    double cz = cosf(x * 0.5);
    double sz = sinf(x * 0.5);
    double cy = cosf(y * 0.5);
    double sy = sinf(y * 0.5);
    double cx = cosf(z * 0.5);
    double sx = sinf(z * 0.5);

    w = cz * cy * cx + sz * sy * sx;
    x = cz * cy * sx - sz * sy * cx;
    y = sz * cy * sx + cz * sy * cx;
    z = sz * cy * cx - cz * sy * sx;
}

void quaternion::rotate_by(quaternion q2){
    float qx,qy,qz,qw;
    qx = x*q2.x - y*q2.y - z*q2.z - w*q2.w;
    qy = x*q2.x + y*q2.y - z*q2.z + w*q2.w;
    qz = x*q2.x + y*q2.y + z*q2.z - w*q2.w;
    qw = x*q2.x - y*q2.y + z*q2.z + w*q2.w;

    x=qx;y=qy;z=qz;w=qw;
}
void quaternion::rotate_by(float x,float y,float z){
    quaternion q2;
    q2.set_euler(x,y,z);
    rotate_by(q2);
}

void quaternion::normalize(){
    float length = sqrtf(x*x+y*y+z*z+w*w);
    x /= length;
    y /= length;
    z /= length;
    w /= length;
}

float quaternion::dot(quaternion q2){
    return x*q2.x + y*q2.y + z*q2.z + w*q2.w;
}

quaternion quaternion::operator + (quaternion q2){
    return {x+q2.x,y+q2.y,z+q2.z,w+q2.w};
}

quaternion quaternion::operator - (quaternion q2){
    return {x-q2.x,y-q2.y,z-q2.z,w-q2.w};
}

quaternion quaternion::operator * (float weight){
    return {x*weight,y*weight,z*weight,w*weight};
}

mat4 quaternion::to_matrix(){
    mat4 ret;
    ret.identity();
    ret.m[0]=1-2*y*y-2*z*z;ret.m[1]=  2*x*y-2*w*z;ret.m[2]=   2*x*z+2*w*y;
    ret.m[4]=  2*x*y+2*w*z;ret.m[5]=1-2*x*x-2*z*z;ret.m[6]=   2*y*z-2*w*x;
    ret.m[8]=  2*x*z-2*w*y;ret.m[9]=  2*y*z+2*w*x;ret.m[10]=1-2*x*x-2*y*y;

    return ret;
}

void mat4::set(mat4* m2){
    for(int i=0;i<16;i++){m[i]=m2->m[i];}
}

mat4 mat4::copy(){
    mat4 new_m = {0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0};
    for(int i=0;i<16;i++){new_m.m[i]=m[i];}
    return new_m;
}

void mat4::multiply_by(mat4* mat){
    float new_m[] = {0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0};
    //Column major on the left = Row Major on the right. In other words, this is a column major muliplication of mat*this, so its this*mat row major.
    float* m2 = mat->m;
    for(int i=0;i<4;i++){
        for(int j=0;j<4;j++){
            new_m[i*4 + j]=( 
                (m[i*4]  	*m2[j]) +
                (m[i*4+1]	*m2[j+4]) +
                (m[i*4+2]	*m2[j+8]) +
                (m[i*4+3]	*m2[j+12])
            );
        }
    }
    for(int i=0;i<16;i++){m[i]=new_m[i];}
}

void mat4::translate(vec3 vec){translate(vec.x,vec.y,vec.z);}
void mat4::translate(float x,float y,float z){
    mat4 m2 = {
        1.0,0,0,x,
        0,1.0,0,y,
        0,0,1.0,z,
        0,0,0,1.0
    };
    multiply_by(&m2);
}

void mat4::scale(vec3 vec){scale(vec.x,vec.y,vec.z);}
void mat4::scale(float x, float y, float z){
    mat4 m2 ={{
        x,0,0,0,
        0,y,0,0,
        0,0,z,0,
        0,0,0,1
    }};
    multiply_by(&m2);
}

void mat4::rotate(vec3 vec){rotate(vec.x,vec.y,vec.z);}
void mat4::rotate(float x, float y, float z){
    rotate_x(x);
    rotate_y(y);
    rotate_z(z);
}
void mat4::rotate(quaternion q){
    mat4 rotation_matrix = q.to_matrix();
    multiply_by(&rotation_matrix);
}

void mat4::rotate_x(float theta){
    if(FLOAT_IS_ZERO(theta))return;
    float rx = -theta*PI_OVER_180;
    float cos = cosf(rx);
    float sin = sinf(rx);
    mat4 rotate {{
        1,      0,	    0,      0,
        0,      cos,    -sin,   0,
        0,      sin,    cos,    0,
        0,      0,      0,      1
    }};
    multiply_by(&rotate);
}
void mat4::rotate_y(float theta){
    if(FLOAT_IS_ZERO(theta))return;
    float rx = -theta*PI_OVER_180;
    float cos = cosf(rx);
    float sin = sinf(rx);
    mat4 rotate ={{
        cos,    0,      sin,    0,
        0,      1,      0,      0,
        -sin,   0,      cos,    0,
        0,      0,      0,      1
    }};
    multiply_by(&rotate);
}
void mat4::rotate_z(float theta){
    if(FLOAT_IS_ZERO(theta))return;
    float rx = -theta*PI_OVER_180;
    float cos = cosf(rx);
    float sin = sinf(rx);
    mat4 rotate ={{
        cos,    -sin,	0,      0,
        sin,    cos,    0,      0,
        0,      0,      1,      0,
        0,      0,      0,      1
    }};
    multiply_by(&rotate);
}

void mat4::identity(){
    for(int i=0;i<16;i++){m[i]=0.0f;}
    m[0] = 1.0;
    m[5] = 1.0;
    m[10] = 1.0;
    m[15] = 1.0;
}

void mat4::ortho(float width,float height,float near,float far){
    float l = (-width/2.0); float r = width/2.0;
    float t = height/2.0; float b = -height/2.0;
    float n = near; float f = far;

    for(int i=0;i<16;i++){m[i]=0.0f;}
    m[0] =  2.0/(r-l);          
    m[3] = -(r+l)/(r-l);        
    m[5] =  2.0/(t-b);          
    m[7] = -(t+b)/(t-b);        
    m[10] =-2.0/(f-n);          
    m[11] =-(f+n)/(f-n);        
    m[15] = 1.0;
}

void mat4::frustum(float l,float r,float b,float t,float n,float f){
    // Row Major Frustum Matrix
    //	X	0 	A 	0 	
    //  0 	Y 	B 	0 
    //  0 	0 	C 	D 	
    //  0 	0 	-1 	0
        
    m[0] = (2*n)/(r-l);	    //X
    m[2] =  (r+l)/(r-l);    //A
    
    m[5] = (2*n)/(t-b); 	 //Y
    m[6] = (t+b)/(t-b);      //B
    
    m[10] = -((f+n)/(f-n));	 //C
    m[11] = -((2*f*n)/(f-n));//D

    m[14] = -1;
    m[15] = 0;
}

void mat4::perspective(float width,float height,float near,float far,float fov){
    float nearsize = tanf((fov/2)*PI_OVER_180)*near;
    float aspect = height/width;
    frustum(-nearsize,nearsize,-nearsize*aspect,nearsize*aspect,near,far);
}

void mat4::transform(float x,float y,float z,vec3 rotation, vec3 scl){
    translate(x,y,z);
    rotate(rotation);
    scale(scl);
}

void mat4::transform(float x,float y,float z,quaternion rotation, vec3 scl){
    translate(x,y,z);
    rotate(rotation);
    scale(scl);
}

void mat4::transpose(){
    float swap;
    swap=m[1]; m[1]=m[4]; m[4]=swap;
    swap=m[2]; m[2]=m[8]; m[8]=swap;
    swap=m[3]; m[3]=m[12]; m[12]=swap;
    swap=m[6]; m[6]=m[9]; m[9]=swap;
    swap=m[7]; m[7]=m[13]; m[13]=swap;
    swap=m[11]; m[11]=m[14]; m[14]=swap;
}

void mat4::multiply_vec3(vec3* vec){
    float _x = m[0]*vec->x + m[1]*vec->y + m[2]* vec->z + m[3];
    float _y = m[4]*vec->x + m[5]*vec->y + m[6]* vec->z + m[7];
    float _z = m[8]*vec->x + m[9]*vec->y + m[10]*vec->z + m[11];
    
    vec->set(_x,_y,_z);
}

void mat3::set(mat4* mat){
    m[0] = mat->m[0]; m[1] = mat->m[1]; m[2] = mat->m[2];
    m[3] = mat->m[4]; m[4] = mat->m[5]; m[5] = mat->m[6];
    m[6] = mat->m[8]; m[7] = mat->m[9]; m[8] = mat->m[10];
}

void mat3:: transpose(){
    float swap;
    swap=m[1]; m[1]=m[3]; m[3]=swap;
    swap=m[2]; m[2]=m[6]; m[6]=swap;
    swap=m[5]; m[5]=m[7]; m[7]=swap;
}
float mat3::determinant(){
return   m[0] * (m[4] * m[8] - m[7] * m[5])
        -m[3] * (m[1] * m[8] - m[7] * m[2])
        +m[6] * (m[1] * m[5] - m[4] * m[2]);
}

void mat3::invert(){
    float det = determinant();
    float c[] = {m[0],m[1],m[2],m[3],m[4],m[5],m[6],m[7],m[8]};
    m[0] =  (c[4] * c[8] - c[7] * c[5])/ det;
    m[1] = -(c[1] * c[8] - c[7] * c[2])/ det;
    m[2] =  (c[1] * c[5] - c[4] * c[2])/ det;
    m[3] = -(c[3] * c[8] - c[6] * c[5])/ det;
    m[4] =  (c[0] * c[8] - c[6] * c[2])/ det;
    m[5] = -(c[0] * c[5] - c[3] * c[2])/ det;
    m[6] =  (c[3] * c[7] - c[6] * c[4])/ det;
    m[7] = -(c[0] * c[7] - c[6] * c[1])/ det;
    m[8] =  (c[0] * c[4] - c[3] * c[1])/ det;
}


vec3 Transform::Position(){return {x,y,z};}
void Transform::Clear(){
    x=y=z=0;
    rotation.x=rotation.y=rotation.z=0;
    scale.x=scale.y=scale.z=1.0f;
}



Location::Location(){position={0,0,0};rotation={0,0,0};}
Location::Location(vec3 pos,vec3 rot){position=pos;rotation=rot;}
//Transform Location::ToTransform(){}