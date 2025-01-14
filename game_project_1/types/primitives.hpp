#ifndef PRIMITIVES_H
#define PRIMITIVES_H

#ifndef null
#define null nullptr
#endif

#define PI 3.1415926535f
#define PI_OVER_180 (3.1415926535f/180.0f)
#define FLOAT_IS_ZERO(x) (x == 0.0f)

#define SET_BIT(val,place)   ((val) |=  (1<<(place)))
#define CLEAR_BIT(val,place) ((val) &= ~(1<<(place)))
#define TOGGLE_BIT(val,place)  ((val) ^=  (1<<(place)))
#define GET_BIT(val,place) ((val & (1 << place)) != 0)

#define DEALLOCATE(val) if(val!=nullptr){free(val);val=nullptr;}
#define SAFE_DELETE(val) if(val!=nullptr){delete val;val=nullptr;}

typedef unsigned int uint32;
typedef unsigned char byte;

union value{
    int i;
    float f;
    wchar_t* wstr;
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
    vec2 normalized();
    void rotate(float theta);
    float angle();
};

struct quaternion;
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
    vec3 rotate(quaternion angle);
    vec3 operator +(vec3 v2);
    vec3 operator -(vec3 v2);
    vec3 operator *(float scl);
    vec3 operator *(vec3 scl);
    vec3 operator /(vec3 scl);
    vec3 of_length(float newlen);
    vec3 normalized();
    vec3 horizontal();
    vec2 xz();
    float xz_angle();
    vec3 clip(vec3 direction);

    static vec3 zero();
};

struct vec4{
    float x,y,z,w;

    vec4();
    vec4(float vx,float vy,float vz,float vw);
};

struct mat4;

struct quaternion{
    float x,y,z,w;

    void clear();
    void set_euler(float x,float y,float z);
    void set_euler_radians(float x,float y,float z);
    static quaternion of_euler(float x,float y,float z);
    static quaternion of_euler_radians(float x,float y,float z);
    static quaternion identity();
    mat4 to_matrix();
    void rotate_by(quaternion q2);
    void rotate_by(float x,float y,float z);
    void normalize();
    quaternion normalized();
    float dot(quaternion q2);
    float theta_difference(quaternion q2);
    float theta_difference_radians(quaternion q2);
    quaternion operator + (quaternion q2);
    quaternion operator - (quaternion q2);
    quaternion operator * (float weight);
    quaternion operator * (quaternion q2);
    quaternion inverse();
    //vec3 get_euler();
};

struct mat4{
    float m[16];

    static mat4  ortho(float width,float height,float near,float far);
    static mat4  frustum(float l,float r,float b,float t,float n,float f);
    static mat4  perspective(float width,float height,float near,float far,float fov);

    void identity();
    void transpose();
    void transform(float x,float y,float z,vec3 rotation, vec3 scale);
    void transform(float x,float y,float z,quaternion rotation, vec3 scale);

    mat4 copy();
    void operator= (mat4& m2);
    
    void set(mat4* m2);
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

struct point_i{
    int x,y;

    point_i operator +(point_i b);
    point_i operator -(point_i b);
    bool operator ==(point_i b);
};

struct rect_i{
    int x,y,w,h;

    rect_i();
    rect_i(int ix,int iy,int iw,int ih);
    int area();
    point_i center();
    bool contains(point_i b);
    bool intersects(rect_i b);
};

struct point_f{
    float x,y;
    point_i to_point_i();
};

struct rect_f{
    float x,y,w,h;

    rect_f();
    rect_f(float fx,float fy,float fw,float fh);
    float area();

    rect_i to_integers();
    rect_f ratio_of(rect_f);
    point_f center();
    bool contains(point_f b);
    bool intersects(rect_f b);
};

struct color_f;
struct color{
    byte r,g,b,a;
    color();
    color(byte R, byte G, byte B, byte A);
    color(int color_code);
    color(color_f c);
    int as_code();
};

struct color_f{
    float r,g,b,a;
    color_f();
    color_f(float R,float G,float B,float A);
    color_f(color c);
    color_f mult(color_f c2);
};

struct bitmask{
    static int bit(int place);
    const static int all= -1;
    const static int none = 0;
    int val;
    bitmask(int a);
    void and_with(bitmask& b2);
    void or_with(bitmask& b2);
    void set(int place);
    void clear(int place);
    void clearAll();
    void toggle_bit(int place);
    bool get_bit(int place);

    static bitmask invert(bitmask& b2);
    static bitmask of_bits(int a);
    static bitmask of_bits(int a, int b);
    static bitmask of_bits(int a, int b, int c);
    static bitmask of_bits(int a[],int l);
};

#endif

