#ifndef DEBUG_DRAW_H
#define DEBUG_DRAW_H

#include <game_project_1/gfx/camera.hpp>
#include <game_project_1/types/primitives.hpp>
#include <game_project_1/types/math_types.hpp>
#include <game_project_1/types/transform.hpp>

namespace DebugDraw{

    void Init();
    void DrawUIRect(int x,int y,int w,int h,color_f color);
    
    void DrawLine(Camera* cam,vec3 start,vec3 end,color_f color);
    void DrawCube(Camera* cam,vec3 center,quaternion rotation,float size,color_f color);
    void Draw3DRect(Camera* cam,vec3 center,quaternion rotation,vec3 size,color_f color);
    void DrawSphere(Camera* cam,vec3 center,float radius,color_f color);
    void DrawEllipse(Camera* cam,vec3 center,float height,float radius,color_f color);
    void DrawCylinder(Camera* cam,vec3 center,float height,float radius,color_f color);
    //void DrawArc(Camera* cam,vec3 center,float height,float radius,float arc_len, float y_rot, color_f color);
    //void DrawCapsule(Camera* cam,vec3 center,quaternion rotation,float height,float radius,color_f color);

};







#endif