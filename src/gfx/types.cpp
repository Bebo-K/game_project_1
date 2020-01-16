#include "types.h"
#include <math.h>


vec3::vec3(){
    x=y=z=0.0;
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

float vec3::length(){
    return sqrtf(x*x+y*y+z*z);
}

float vec3::length_sqr(){
    return x*x+y*y+z*z;
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

void mat4::set(mat4* m2){
    for(int i=0;i<16;i++){m[i]=m2->m[i];}
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

void mat4::scale(vec3 vec){translate(vec.x,vec.y,vec.z);}
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

void mat4::multiply_vec3(vec3* vec){
    float _x = m[0]*vec->x + m[1]*vec->y + m[2]* vec->z + m[3];
    float _y = m[4]*vec->x + m[5]*vec->y + m[6]* vec->z + m[7];
    float _z = m[8]*vec->x + m[9]*vec->y + m[10]*vec->z + m[11];
    
    vec->set(_x,_y,_z);
}
