#ifndef UI_TEXT_H
#define UI_TEXT_H

#include "../structs/3d_types.h"
#include "../structs/math_types.h"
#include "font_manager.h"


struct Glyph{
    float x,y;
    Texture glyph_texture;
    //float rotation;
    //float color[4];
    //vec2  offset;
};

class UIText{
    public:
    int x,y;
    text_char* string;
    int    glyph_count;
    Glyph* glyphs;

    
    UIText();
    UIText(char* str);
    UIText(text_char* str);
    UIText(text_char* str,FontID font);
    void SetString(char* str);
    void SetString(text_char* str,FontID font);
    ~UIText();

    void Update(int frames);
    void Draw();
};
/*
class TextBoxText: public Drawable{
    public:
    text_string string;


    TextBoxText(text_string str);
    ~TextBoxText();

    void Update(int frames);
    void Draw(Camera* cam,mat4* view, mat4* projection);
};
*/



#endif