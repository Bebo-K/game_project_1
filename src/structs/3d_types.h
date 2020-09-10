#ifndef _3D_TYPES_H
#define _3D_TYPES_H

#define PI 3.1415926535f
#define PI_OVER_180 3.1415926535f/180.0f 
#define FLOAT_IS_ZERO(x) x == 0.0f

struct vec3{
    public:
    float x,y,z;

    vec3();
    vec3(vec3* copy);
    vec3(float x,float y,float z);

    void set(float x,float y,float z);
    float dot(struct vec3 b);
    vec3 cross(struct vec3 b);
    float length();
    float length_sqr();
    float dist(struct vec3 b);
    float dist_sqr(struct vec3 b);
    void rotate_x(float theta);
    void rotate_y(float theta);
    void rotate_z(float theta);
    vec3 operator +(vec3 v2);
    vec3 operator -(vec3 v2);
    vec3 operator *(float scl);
    vec3 of_length(float newlen);
    vec3 normalized();
    vec3 xz();
};

struct vec2{
    float x,y;

    vec2();
    vec2(vec2* copy);
    vec2(float x,float y);

    float length();
    float length_sqr();
    //float dot();
    void normalize();
    void rotate(float theta);
};

struct mat4;

struct quaternion{
    float x,y,z,w;

    void clear();
    void set_euler(float x,float y,float z);
    mat4 to_matrix();
    void rotate_by(quaternion q2);
    void rotate_by(float x,float y,float z);
    void normalize();
    float dot(quaternion q2);
    quaternion operator + (quaternion q2);
    quaternion operator - (quaternion q2);
    quaternion operator * (float weight);
    //vec3 get_euler();
};

struct mat4{

    float m[16];


    void identity();
    void transpose();
    void ortho(float width,float height,float near,float far);
    void frustum(float l,float r,float b,float t,float n,float f);
    void perspective(float width,float height,float near,float far,float fov);
    void transform(float x,float y,float z,vec3 rotation, vec3 scale);
    void transform(float x,float y,float z,quaternion rotation, vec3 scale);

    void set(mat4* m2);
    mat4 copy();
    void multiply_vec3(vec3* vec);

    void multiply_by(mat4* mat);

    void translate(vec3 vec);
    void translate(float x, float y, float z);

    void scale(vec3 vec);
    void scale(float x,float y,float z);

    void rotate(vec3 vec);
    void rotate(float x, float y, float z);
    void rotate(quaternion q);

    void rotate_x(float theta);
    void rotate_y(float theta);
    void rotate_z(float theta);
};

struct mat3{
    float m[9];

    void set(mat4* m);
    void invert();
    void transpose();
    float determinant();
};


struct Transform{
    float x,y,z;
    quaternion rotation;
    vec3 scale;

    void Clear();
};


#endif