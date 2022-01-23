#include "ui_text.h"
#include "../log.h"
#include "../os.h"
#include "drawable.h"

#include "../gui/layout.h"
#include "../gui/layout.h"

//Renders on one big triangle instead of 2 in a quad, to prevent diagonal seams.
float glyph_vert_data[] =       {0,2,0,  0,0,0,  2,0,0};//   0,0,0,  0,1,0,  1,1,0};
float glyph_texcoord_data[] =   {0,-1,    0,1,    2,1};//   0,1,  0,0,  1,0};
VBO glyph_vertices;
VBO glyph_texcoords;


char default_font_name[] = "SourceSansPro-Regular";
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
    font=-1;
    default_font_size=16;
    x=y=w=h=0;
    font=-1;
}
UI_Text::UI_Text(wchar* str){
    if(!glyph_vertices.Valid()){BuildGlyphPrimitive();}
    glyphs=nullptr;
    string=nullptr;
    glyph_count=0;
    font=-1;
    default_font_size=16;
    x=y=w=h=0;
    SetString(str);
}

UI_Text::UI_Text(wchar* str,FontID font_id){
    if(!glyph_vertices.Valid()){BuildGlyphPrimitive();}
    glyphs=nullptr;
    string=nullptr;
    glyph_count=0;
    font=font_id;
    default_font_size=FontManager::GetFontInfo(font_id)->font_size;
    x=y=w=h=0;
    SetString(str);
}

void UI_Text::SetString(wchar* str){
    if(string != null){
        free(string);
        string=null;
    }
    if(str ==null || wstr::len(str) == 0)return;
    string=wstr::new_copy(str);
    if(font < 0){font = FontManager::LoadFontFace(default_font_name,16);}
    BuildString();
}

void UI_Text::BuildString(){
    if(string ==null || wstr::len(string) == 0)return;
    if(glyphs != nullptr){
        free(glyphs);
        glyphs=nullptr;
        glyph_count = 0;
    }

    FontManager::SetActiveFont(font);
    FT_Face fontface = FontManager::GetActiveFont()->fontface;

    glyph_count = wstr::len(string);
    glyphs = (Glyph*)calloc(glyph_count,sizeof(Glyph));

    int pen_x=0,pen_y=0;
    int max_height=0;

    for(int i=0;string[i] != 0;i++){
        int err = FT_Load_Char(fontface,string[i],FT_LOAD_DEFAULT);
        if(err != 0){logger::warn("Unable to load glyph. Code:%d\n",err);}        

        if(max_height < fontface->glyph->metrics.height){max_height = fontface->glyph->metrics.height;}

        glyphs[i].x = (float)(pen_x + (fontface->glyph->metrics.horiBearingX/64));
        
        glyphs[i].y = (float)(pen_y - (fontface->glyph->metrics.height- fontface->glyph->metrics.horiBearingY)/64);
        glyphs[i].glyph_texture = FontManager::GetGlyph(string[i]);
        glyphs[i].color[0]=1.0f;
        glyphs[i].color[1]=1.0f;
        glyphs[i].color[2]=1.0f;
        glyphs[i].color[3]=1.0f;

        pen_x += fontface->glyph->metrics.horiAdvance/64;//advance is 1/64th of a pixel
        if(string[i] != ' '){pen_x-=1;}else pen_x += 1;//don't know why but I promise this is a thing
        pen_y += fontface->glyph->advance.y/64;
    }
    w = pen_x;
    h = max_height/64;
}


void UI_Text::SetFontSize(int point){
    font = FontManager::RescaleFontID(font,point);
    default_font_size = point;
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

    mat4 modelview;

    glActiveTexture(GL_TEXTURE0);
    Glyph* glyph=nullptr;
    for(int i=0;i<glyph_count;i++){
        glyph = &glyphs[i];

        modelview.identity();
        modelview.translate(glyph->offset.x,glyph->offset.y,0);
        modelview.rotate_z(glyph->rotation);

        glUniform2f(text_shader->IMAGE_POS,x+glyphs[i].x,y+glyphs[i].y);
        glUniform2f(text_shader->IMAGE_SIZE,(float)glyph->glyph_texture.width_px,(float)glyph->glyph_texture.height_px);
        //glUniform2f(text_shader->WINDOW_SIZE,(float)Window::width,(float)Window::height);
        glUniform2f(text_shader->WINDOW_SIZE,(float)UI::UI_WIDTH,(float)UI::UI_HEIGHT);
        glUniform4fv(text_shader->COLOR,1,(GLfloat*)glyph->color);
        glUniformMatrix4fv(text_shader->MODELVIEW_MATRIX,1,true,(GLfloat*)&modelview);
        glBindTexture(GL_TEXTURE_2D,glyphs[i].glyph_texture.atlas_id);

        glUniform1i(text_shader->TEXTURE_0,0);
        glUniform4fv(text_shader->TEXTURE_LOCATION,1,(GLfloat*)&glyphs[i].glyph_texture.tex_coords);

        glDrawArrays(GL_TRIANGLES,0,3);
    }

    glDisableVertexAttribArray(text_shader->ATTRIB_VERTEX);
    glDisableVertexAttribArray(text_shader->ATTRIB_TEXCOORD);
    
    glEnable(GL_DEPTH_TEST);
}


