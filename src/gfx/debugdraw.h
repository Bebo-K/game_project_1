#ifndef DEBUG_DRAW_H
#define DEBUG_DRAW_H

#include "../struct/3d_types.h"

namespace DebugDraw{

    void Init(){};
    void DrawRect(int x,int y,int w,int h,vec4 color);
    void Draw3DRect(int x,int y,int z,int w,int h,int d,vec4 color);


};







#endif