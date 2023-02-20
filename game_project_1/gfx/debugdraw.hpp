#ifndef DEBUG_DRAW_H
#define DEBUG_DRAW_H

#include <game_project_1/types/3d_types.hpp>

namespace DebugDraw{

    void Init();
    void DrawRect(int x,int y,int w,int h,color_f color);
    void Draw3DRect(int x,int y,int z,int w,int h,int d,color_f color);


};







#endif