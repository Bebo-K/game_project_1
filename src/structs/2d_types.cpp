#include "2d_types.h"



rect_i::rect_i(){x=y=w=h=0;}
rect_i::rect_i(int ix, int iy,int iw, int ih){x=ix;y=iy;w=iw;h=ih;}
int rect_i::area(){return w*h;}
point_i rect_i::center(){return {x+(w/2),y+(h/2)};}
bool rect_i::intersects(point_i b){
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
bool rect_f::intersects(point_f b){
    return  (b.x >= x) && (b.x <= x + w) &&
            (b.y >= y) && (b.y <= y + h);
}
bool rect_f::intersects(rect_f b){
    return  (b.x + b.w <= x) && (x + w <= b.x) &&  (x <= b.x + b.w) && (b.x <= x + w) &&
            (b.y + b.h <= y) && (y + h <= b.y) &&  (y <= b.y + b.h) && (b.y <= y + h);
}