#include <game_project_1/types/primitives.hpp>
#include <math.h>

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

float vec2::angle(){
    float len = sqrtf(x*x+y*y);
    float theta = atan2f(x/len,y/len)/PI_OVER_180;
    return (theta > 0)? theta: theta+360.0f;
}
vec2 vec2::operator*(float v){
    return {x*v,y*v};
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

vec3 vec3::rotate(quaternion angle){
    angle.normalize();
    quaternion v = {x,y,z,0};
    quaternion result = (angle * v) * angle.inverse();
    return {result.x,result.y,result.z};
}

vec3 vec3::operator +(vec3 v2){return{x+v2.x,y+v2.y,z+v2.z};}
vec3 vec3::operator -(vec3 v2){return{x-v2.x,y-v2.y,z-v2.z};}
vec3 vec3::operator *(float scl){return {x*scl,y*scl,z*scl};}
vec3& vec3::operator *=(float scl){x*=scl;y*=scl;z*=scl;return *this;}
vec3& vec3::operator *=(vec3 s){x*=s.x;y*=s.y;z*=s.z;return *this;}
vec3 vec3::operator *(vec3 scl){return {x*scl.x,y*scl.y,z*scl.z};}
vec3 vec3::operator /(vec3 scl){return {x/scl.x,y/scl.y,z/scl.z};}
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


//returns angle from +z axis of the horizontal component of this vector
float vec3::xz_angle(){
    float len = sqrtf(x*x+z*z);
    float theta = atan2f(z/len,x/len)/PI_OVER_180;
    return (theta > 0)? theta: theta+360.0f;
}

vec3 vec3::zero(){return {0.0f,0.0f,0.0f};}

vec4::vec4(){
    x=y=z=w=0;
}
vec4::vec4(float _x,float _y,float _z,float _w){
    x=_x;
    y=_y;
    z=_z;
    w=_w;
}

quaternion::quaternion(){x=y=z=0;w=1;}
quaternion::quaternion(float rx,float ry,float rz){x=y=z=0;w=1;set_euler(rx,ry,rz);}
quaternion::quaternion(float qx,float qy,float qz,float qw){x=qx;y=qy;z=qz;w=qw;}

void quaternion::clear(){
    x=y=z=0;
    w=1;
}


void quaternion::set_euler(float rx,float ry,float rz){
    set_euler_radians(PI_OVER_180*rx,PI_OVER_180*ry,PI_OVER_180*rz);
}
void quaternion::set_euler_radians(float rx,float ry,float rz){
    double cx = cosf(rx * 0.5);
    double sx = sinf(rx * 0.5);
    double cy = cosf(ry * 0.5);
    double sy = sinf(ry * 0.5);
    double cz = cosf(rz * 0.5);
    double sz = sinf(rz * 0.5);

    w = cx * cy * cz + sx * sy * sz;
    x = sx * cy * cz - cx * sy * sz;
    y = cx * sy * cz + sx * cy * sz;
    z = cx * cy * sz - sx * sy * cz;
}

quaternion quaternion::of_euler(float rx,float ry,float rz){
    return of_euler_radians(PI_OVER_180*rx,PI_OVER_180*ry,PI_OVER_180*rz);
}
quaternion quaternion::of_euler_radians(float rx,float ry,float rz){
    double cx = cosf(rx * 0.5);
    double sx = sinf(rx * 0.5);
    double cy = cosf(ry * 0.5);
    double sy = sinf(ry * 0.5);
    double cz = cosf(rz * 0.5);
    double sz = sinf(rz * 0.5);
    /*
    return{
        (float)(cx * cy * cz + sx * sy * sz),
        (float)(sx * cy * cz - cx * sy * sz),
        (float)(cx * sy * cz + sx * cy * sz),
        (float)(cx * cy * sz - sx * sy * cz)
    };
    */
   
    return{
        (float)(sx * cy * cz - cx * sy * sz),
        (float)(cx * sy * cz + sx * cy * sz),
        (float)(cx * cy * sz - sx * sy * cz),
        (float)(cx * cy * cz + sx * sy * sz)
    };
}

quaternion quaternion::identity(){
    return {0,0,0,1.0f};
}

quaternion quaternion::from_to(vec3 from,vec3 to){
    vec3 nfrom = from.normalized();
    vec3 nto = to.normalized();

    vec3 cross = nfrom.cross(nto);
    float dot = nfrom.dot(nto);//cos theta between vectors

    if(dot < -0.9999f){
        //180 degree turn
        vec3 ortho = {1,0,0};
        if(fabs(nfrom.x) > fabs(nfrom.z)){
            ortho = {0,0,1};
        }
        vec3 axis = nfrom.cross(ortho).normalized();
        return {axis.x,axis.y,axis.z,0};
    }
    if(dot > 0.9999f){
        //no rotation
        return quaternion::identity();
    }   

    float theta = acosf(dot);
    float w = cosf(theta/2.0f);
    float s = sinf(theta/2.0f);

    return (quaternion{cross.x*s,cross.y*s,cross.z*s,w}).normalized();
}

void quaternion::rotate_by(quaternion q2){
    quaternion rotated = (*this)*q2;
    x=rotated.x;
    y=rotated.y;
    z=rotated.z;
    w=rotated.w;
    /* where'd I get this math?
    float qx,qy,qz,qw;
    qx = x*q2.x - y*q2.y - z*q2.z - w*q2.w;
    qy = x*q2.x + y*q2.y - z*q2.z + w*q2.w;
    qz = x*q2.x + y*q2.y + z*q2.z - w*q2.w;
    qw = x*q2.x - y*q2.y + z*q2.z + w*q2.w;

    x=qx;y=qy;z=qz;w=qw;
    */
}


//gives the product a*b where a = this and b = q2
quaternion quaternion::operator* (quaternion q2){
    return {
        w*q2.x + x*q2.w  + y*q2.z - z*q2.y,
        w*q2.y - x*q2.z  + y*q2.w + z*q2.x,
        w*q2.z + x*q2.y  - y*q2.x + z*q2.w,
        w*q2.w - x*q2.x  - y*q2.y - z*q2.z
    };
}

//gives the quaternion q2 that (this)*q2 = identity
quaternion quaternion::inverse(){
    return {-x,-y,-z,w};
}
/*
vec3 quaternion::get_euler(){
    
}
*/

void quaternion::rotate_by(float rx,float ry,float rz){
    quaternion q2;
    q2.set_euler(rx,ry,rz);
    rotate_by(q2);
}

void quaternion::normalize(){
    float length = sqrtf(x*x+y*y+z*z+w*w);
    x /= length;
    y /= length;
    z /= length;
    w /= length;
}

quaternion quaternion::normalized(){
    float length = sqrtf(x*x+y*y+z*z+w*w);
    return {x/length,y/length,z/length,w/length};
}

float quaternion::dot(quaternion q2){
    return x*q2.x + y*q2.y + z*q2.z + w*q2.w;
}

float quaternion::theta_difference(quaternion q2){
    return theta_difference_radians(q2)*PI_OVER_180;
}
float quaternion::theta_difference_radians(quaternion q2){
    quaternion diff = inverse()*q2;
    float diff_vec = (vec3{diff.x,diff.y,diff.z}).length();
    return abs(atan2(diff_vec,w));
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


void mat4::operator=(mat4& m2){
    for(int i=0;i<16;i++){m[i]=m2.m[i];}
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

mat4 mat4::ortho(float width,float height,float near,float far){
    float l = (-width/2.0); float r = width/2.0;
    float t = height/2.0; float b = -height/2.0;
    float n = near; float f = far;

    mat4 ortho;
    for(int i=0;i<16;i++){ortho.m[i]=0.0f;}
    ortho.m[0] =  2.0/(r-l);          
    ortho.m[3] = -(r+l)/(r-l);        
    ortho.m[5] =  2.0/(t-b);          
    ortho.m[7] = -(t+b)/(t-b);        
    ortho.m[10] =-2.0/(f-n);          
    ortho.m[11] =-(f+n)/(f-n);        
    ortho.m[15] = 1.0;
    return ortho;
}

mat4 mat4::get_identity(){
    mat4 identity;
    identity.identity();
    return identity;
}

mat4 mat4::frustum(float l,float r,float b,float t,float n,float f){
    // Row Major Frustum Matrix
    //	X	0 	A 	0 	
    //  0 	Y 	B 	0 
    //  0 	0 	C 	D 	
    //  0 	0 	-1 	0

    mat4 frustum;
    for(int i=0;i<16;i++){frustum.m[i]=0.0f;}
        
    frustum.m[0] = (2*n)/(r-l);	    //X
    frustum.m[2] =  (r+l)/(r-l);    //A
    
    frustum.m[5] = (2*n)/(t-b); 	 //Y
    frustum.m[6] = (t+b)/(t-b);      //B
    
    frustum.m[10] = -((f+n)/(f-n));	 //C
    frustum.m[11] = -((2*f*n)/(f-n));//D

    frustum.m[14] = -1;
    frustum.m[15] = 0;
    return frustum;
}

mat4 mat4::perspective(float width,float height,float near,float far,float fov){
    float nearsize = tanf((fov/2)*PI_OVER_180)*near;
    float aspect = height/width;
    return frustum(-nearsize,nearsize,-nearsize*aspect,nearsize*aspect,near,far);
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



point_i point_i::operator+(point_i b){return {x+b.x,y+b.y};}
point_i point_i::operator-(point_i b){return {x-b.x,y-b.y};}
bool point_i::operator ==(point_i b){return x==b.x && y==b.y;}

point_i point_f::to_point_i(){return {(int)x,(int)y};}

rect_i::rect_i(){x=y=w=h=0;}
rect_i::rect_i(int ix, int iy,int iw, int ih){x=ix;y=iy;w=iw;h=ih;}
int rect_i::area(){return w*h;}
point_i rect_i::center(){return {x+(w/2),y+(h/2)};}
bool rect_i::contains(point_i b){
    return  (b.x >= x) && (b.x <= x + w) &&
            (b.y >= y) && (b.y <= y + h);
}
bool rect_i::intersects(rect_i b){
    return  (b.x + b.w <= x) && (x + w <= b.x) &&  (x <= b.x + b.w) && (b.x <= x + w) &&
            (b.y + b.h <= y) && (y + h <= b.y) &&  (y <= b.y + b.h) && (b.y <= y + h);
}

rect_f::rect_f(){x=y=w=h=0;}
rect_f::rect_f(float fx, float fy,float fw, float fh){x=fx;y=fy;w=fw;h=fh;}
float rect_f::area(){return w*h;}
rect_i rect_f::to_integers(){return {(int)x,(int)y,(int)w,(int)h};}
rect_f rect_f::ratio_of(rect_f b){return {x/b.w,y/b.h,w/b.w,h/b.h};}
point_f rect_f::center(){return {x+(w/2),y+(h/2)};}
bool rect_f::contains(point_f b){
    return  (b.x >= x) && (b.x <= x + w) &&
            (b.y >= y) && (b.y <= y + h);
}
bool rect_f::intersects(rect_f b){
    return  (b.x + b.w <= x) && (x + w <= b.x) &&  (x <= b.x + b.w) && (b.x <= x + w) &&
            (b.y + b.h <= y) && (y + h <= b.y) &&  (y <= b.y + b.h) && (b.y <= y + h);
}


int bitmask::bit(int place){return (1<<place);}
bitmask::bitmask(int a){val =a;}
bitmask bitmask::of_bits(int a){return bitmask(1<<a);}
bitmask bitmask::of_bits(int a, int b){return bitmask((1<<a)|(1<<b));}
bitmask bitmask::of_bits(int a, int b, int c){return bitmask((1<<a)|(1<<b)|(1<<c));}
//...
bitmask bitmask::of_bits(int a[],int l){
    bitmask mask(0);
    for(int i=0;i<l;i++){mask.set(a[i]);}
    return mask;
}
void bitmask::set(int place){val |= bit(place);}
void bitmask::clear(int place){val &= ~bit(place);}
void bitmask::clearAll(){val=0;}
void bitmask::toggle_bit(int place){val ^= bit(place);}
bool bitmask::get_bit(int place){return (val & bit(place)) != 0;}
void bitmask::and_with(bitmask& b2){val &= b2.val;}
void bitmask::or_with(bitmask& b2){val |= b2.val;}
bitmask bitmask::invert(bitmask& b2){return {~b2.val};}

color::color(){r=255;g=0;b=255;a=255;}
color::color(byte R, byte G, byte B, byte A){
    r=R;b=B;g=G;a=A;
}
color::color(int color_code){
    r = (color_code>>24) | 255;
    g = (color_code>>16) | 255;
    b = (color_code>>8) | 255;
    a = (color_code) | 255;
}
color::color(color_f colorf){
    r=colorf.r*255.0f;
    g=colorf.g*255.0f;
    b=colorf.b*255.0f;
    a=colorf.a*255.0f;
}

int color::as_code(){
    int color_code=0;
    color_code |= r;
    color_code = color_code << 8;
    color_code |= g;
    color_code = color_code << 8;
    color_code |= b;
    color_code = color_code << 8;
    color_code |= a;

    return color_code;
}
color_f::color_f(){r=1.0f;g=0.0f;1.0f;a=1.0f;}
color_f::color_f(float R,float G,float B,float A){r=R;g=G;b=B;a=A;}
color_f::color_f(color c){
    r = c.r/255.0f;
    g = c.g/255.0f;
    b = c.b/255.0f;
    a = c.a/255.0f;
}

color_f color_f::mult(color_f c2){
    return {r*c2.r, g*c2.g, b*c2.b, a*c2.a};
}