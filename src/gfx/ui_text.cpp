#include "ui_text.h"
#include "../log.h"
#include "../os.h"
#include "drawable.h"


//Renders on one big triangle instead of 2 in a quad, to prevent diagonal seams.
float glyph_vert_data[] =       {0,2,0,  0,0,0,  2,0,0};//   0,0,0,  0,1,0,  1,1,0};
float glyph_texcoord_data[] =   {0,-1,    0,1,    2,1};//   0,1,  0,0,  1,0};
VBO glyph_vertices;
VBO glyph_texcoords;

//const float TEXT_SCALE = 0.5f;


void BuildGlyphPrimitive(){
    glyph_vertices.Create(glyph_vert_data,GL_FLOAT,3,9);
    glyph_texcoords.Create(glyph_texcoord_data,GL_FLOAT,2,6);
    int err = glGetError();
    if(err != 0){
        logger::warn("Error building text glyph primitive, code: %d \n",err);
    }
}



UI_Text::UI_Text(){
    if(!glyph_vertices.Valid()){BuildGlyphPrimitive();}
    glyphs=nullptr;
    string=nullptr;
    glyph_count=0;
    x=y=0;
}
UI_Text::UI_Text(char* str){
    if(!glyph_vertices.Valid()){BuildGlyphPrimitive();}
    glyphs=nullptr;
    x=y=0;
    SetString(TextString::from_cstr(str),-1);
}
UI_Text::UI_Text(text_char* str){
    if(!glyph_vertices.Valid()){BuildGlyphPrimitive();}
    glyphs=nullptr;
    x=y=0;
    SetString(str,-1);
}
UI_Text::UI_Text(text_char* str,FontID font_id){
    if(!glyph_vertices.Valid()){BuildGlyphPrimitive();}
    glyphs=nullptr;
    x=y=0;
    SetString(str,font_id);
}

void UI_Text::SetString(char* str){
    SetString(TextString::from_cstr(str),-1);
}
void UI_Text::SetString(char* str,FontID font_id){
    SetString(TextString::from_cstr(str),font_id);
}
void UI_Text::SetString(text_char* str){
    SetString(str,-1);
}
void UI_Text::SetString(text_char* str,FontID font_id){
    if(str ==null)return;
    string=str;
    if(font_id >= 0){FontManager::SetActiveFont(font_id);}
    FT_Face fontface = FontManager::GetActiveFont()->fontface;

    int strlen=0;
    for(int i=0;str[i] != 0;i++){strlen++;}
    glyph_count = strlen;
    if(glyphs != nullptr){free(glyphs);glyphs=nullptr;}
    glyphs = (Glyph*)calloc(glyph_count,sizeof(Glyph));

    int pen_x=0,pen_y=0;

    for(int i=0;str[i] != 0;i++){
        int err = FT_Load_Char(fontface,str[i],FT_LOAD_DEFAULT);
        if(err != 0){logger::warn("Unable to load glyph. Code:%d\n",err);}        

        glyphs[i].x = (float)(pen_x + (fontface->glyph->metrics.horiBearingX/64));
        glyphs[i].y = (float)(pen_y - (fontface->glyph->metrics.height- fontface->glyph->metrics.horiBearingY)/64);
        glyphs[i].glyph_texture = FontManager::GetGlyph(str[i]);

        pen_x += fontface->glyph->metrics.horiAdvance/64;//advance is 1/64th of a pixel
        if(str[i] != ' '){pen_x-=1;}else pen_x += 1;//don't know why but I promise this is a thing
        pen_y += fontface->glyph->advance.y/64;
    }
}

UI_Text::~UI_Text(){
    free(glyphs);
}

void UI_Text::Draw(){
    Shader* text_shader = ShaderManager::UseShader("ui_text");
    glDisable(GL_DEPTH_TEST);
    glEnableVertexAttribArray(text_shader->ATTRIB_VERTEX);
    glEnableVertexAttribArray(text_shader->ATTRIB_TEXCOORD);
    glyph_vertices.Bind(0);
    glyph_texcoords.Bind(1);

    glActiveTexture(GL_TEXTURE0);
    for(int i=0;i<glyph_count;i++){
        glUniform2f(text_shader->IMAGE_POS,x+glyphs[i].x,y+glyphs[i].y);
        glUniform2f(text_shader->IMAGE_SIZE,(float)glyphs[i].glyph_texture.width_px,(float)glyphs[i].glyph_texture.height_px);
        glUniform2f(text_shader->WINDOW_SIZE,(float)Window::width,(float)Window::height);
        //glUniform4fv(text_shader->COLOR,1,(GLfloat*)glyphs[i].color);
        glBindTexture(GL_TEXTURE_2D,glyphs[i].glyph_texture.atlas_id);

        glUniform1i(text_shader->TEXTURE_0,0);
        glUniform4fv(text_shader->TEXTURE_LOCATION,1,(GLfloat*)&glyphs[i].glyph_texture.tex_coords);

        glDrawArrays(GL_TRIANGLES,0,3);
    }

    glDisableVertexAttribArray(text_shader->ATTRIB_VERTEX);
    glDisableVertexAttribArray(text_shader->ATTRIB_TEXCOORD);
    
    glEnable(GL_DEPTH_TEST);
}


