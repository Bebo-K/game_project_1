#ifndef TEXT_H
#define TEXT_H

#include "../structs/3d_types.h"
#include "../structs/math_types.h"
#include "drawable.h"
#include "font_manager.h"



typedef unsigned int* text_string;

namespace TextString{
    text_string from_cstr(char* str);

}


struct Glyph{
    float x,y;
    Texture glyph_texture;
    //float rotation;
    //float color[4];
    //vec2  offset;
};

class SimpleText: public Drawable{
    public:
    text_string string;
    int    glyph_count;
    Glyph* glyphs;

    SimpleText(char* str);
    SimpleText(text_string str);
    SimpleText(text_string str,int font_size);
    void SetString(text_string str, int font_size);
    ~SimpleText();

    void Update(int frames);
    void Draw(Camera* cam,mat4* view, mat4* projection);
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