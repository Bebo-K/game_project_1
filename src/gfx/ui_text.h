#ifndef UI_TEXT_H
#define UI_TEXT_H

#include "../struct/3d_types.h"
#include "../struct/math_types.h"
#include "font_manager.h"


struct Glyph{
    float x,y;
    Texture glyph_texture;
    //float rotation;
    //float color[4];
    //vec2  offset;
};

class UI_Text{
    public:
    int x,y;
    int w,h;
    wchar* string;
    FontID font;
    int    glyph_count;
    Glyph* glyphs;

    
    UI_Text();
    UI_Text(char* str);
    UI_Text(wchar* str);
    UI_Text(wchar* str,FontID font);
    
    void SetString(char* str);
    void SetString(char* str,FontID font);
    void SetString(wchar* str);
    void SetString(wchar* str,FontID font);
    void Draw();
    ~UI_Text();
};



#endif