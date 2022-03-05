#ifndef UI_TEXT_H
#define UI_TEXT_H

#include <game_project_1/types/3d_types.hpp>
#include <game_project_1/types/2d_types.hpp>
#include <game_project_1/types/math_types.hpp>
#include <game_project_1/gfx/font_manager.hpp>


struct Glyph{
    float x,y;
    Texture glyph_texture;
    float rotation;
    vec4  color;
    vec2  offset;//individual letter offset to center it.
    int   codepoint;

    point_i Set(int codepoint);//Sets this letter, texture, and offset. Returns pen offset to next letter.
    void Draw(point_i offset);
};

class DrawableText{
    public:
    int x,y;
    int w,h;
    wchar* string;
    FontID font;
    int    default_font_size;
    int    glyph_count;
    Glyph* glyphs;

    
    DrawableText();
    DrawableText(wchar* str);
    DrawableText(wchar* str,FontID font);
    
    void SetString(wchar* str);
    void BuildString();

    void SetFontSize(int point);

    void Draw();
    ~DrawableText();
};



#endif