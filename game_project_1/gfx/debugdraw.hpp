#ifndef DEBUG_DRAW_H
#define DEBUG_DRAW_H

#include <game_project_1/gfx/camera.hpp>
#include <game_project_1/types/3d_types.hpp>
#include <game_project_1/types/data_types.hpp>

namespace DebugDraw{

    void Init();
    void DrawUIRect(int x,int y,int w,int h,color_f color);
    
    void DrawCube(Camera* cam,Transform center_transform,float size,color_f color);
    void Draw3DRect(Camera* cam,Transform center_transform,vec3 size,color_f color);
    void DrawSphere(Camera* cam,Transform center_transform,float height,color_f color);
    void DrawEllipse(Camera* cam,Transform center_transform,float height,float radius,color_f color);
    void DrawCapsule(Camera* cam,Transform center_transform,float height,float radius,color_f color);

};







#endif