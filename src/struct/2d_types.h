#ifndef _2D_TYPES_H
#define _2D_TYPES_H


struct point_i{int x,y;};

struct point_f{float x,y;};

struct rect_i{
    int x,y,w,h;

    rect_i();
    rect_i(int ix,int iy,int iw,int ih);
    int area();
    point_i center();
    bool contains(point_i b);
    bool intersects(rect_i b);
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



#endif