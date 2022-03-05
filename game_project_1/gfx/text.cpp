#include <game_project_1/gfx/text.hpp>
#include <game_project_1/gfx/ui_quad.hpp>
#include <game_project_1/gfx/drawable.hpp>
#include <game_project_1/gui/layout.hpp>
#include <game_project_1/log.hpp>
#include <game_project_1/os.hpp>

char default_font_name[] = "SourceSansPro-Regular";
//const float TEXT_SCALE = 0.5f;


DrawableText::DrawableText(){
    glyphs=nullptr;
    string=nullptr;
    glyph_count=0;
    font={-1,18};
    default_font_size=16;
    x=y=w=h=0;
}
DrawableText::DrawableText(wchar* str){
    glyphs=nullptr;
    string=nullptr;
    glyph_count=0;
    font={-1,18};
    default_font_size=16;
    x=y=w=h=0;
    SetString(str);
}
DrawableText::DrawableText(wchar* str,FontID font_id){
    glyphs=nullptr;
    string=nullptr;
    glyph_count=0;
    font=font_id;
    default_font_size=FontManager::GetFontInfo(font_id)->font_size;
    x=y=w=h=0;
    SetString(str);
}

void DrawableText::SetString(wchar* str){
    if(string != null){
        free(string);
        string=null;
    }
    string=wstr::new_copy(str);
    if(font.family_id < 0){font = FontManager::LoadFontFace(default_font_name,16);}
    BuildString();
}

void DrawableText::BuildString(){
    if(glyphs != nullptr){
        free(glyphs);
        glyphs=nullptr;
        glyph_count = 0;
    }
    if(string ==null || wstr::len(string) == 0)return;

    FontManager::SetActiveFont(font);
    FT_Face fontface = FontManager::GetActiveFont()->fontface;

    glyph_count = wstr::len(string);
    glyphs = (Glyph*)calloc(glyph_count,sizeof(Glyph));

    int pen_x=0,pen_y=0;
    int max_height=0;

    for(int i=0;string[i] != 0;i++){
        point_i advance = glyphs[i].Set(string[i]);
        glyphs[i].x=pen_x;
        glyphs[i].y=pen_y;
        if(max_height < fontface->glyph->metrics.height){max_height = fontface->glyph->metrics.height;}
        pen_x += advance.x;
        pen_y += advance.y;
    }
    w = pen_x;
    h = max_height/64;
}

void DrawableText::SetFontSize(int point){
    font = FontManager::RescaleFontID(font,point);
    default_font_size = point;
}

DrawableText::~DrawableText(){
    free(glyphs);
}

void DrawableText::Draw(){
    for(int i=0;i<glyph_count;i++){glyphs[i].Draw({x,y});}
}

point_i Glyph::Set(int code_point){
    codepoint=code_point;
    FT_Face fontface = FontManager::GetActiveFont()->fontface;
    int err = FT_Load_Char(fontface,code_point,FT_LOAD_DEFAULT);
    if(err != 0){logger::warn("Unable to load glyph. Code:%d\n",err);}        

    glyph_texture = FontManager::GetGlyph(codepoint);
    offset.x = (fontface->glyph->metrics.horiBearingX/64);
    offset.y = - (fontface->glyph->metrics.height - fontface->glyph->metrics.horiBearingY)/64;
    color={1.0f,1.0f,1.0f,1.0f};
    point_i advance = {(fontface->glyph->metrics.horiAdvance/64),(fontface->glyph->advance.y/64)};//advance is 1/64th of a pixel
    //if(codepoint != ' '){advance.x--;}else{advance.x++;}//don't know why but I promise this is a thing
    return advance;
}

void Glyph::Draw(point_i draw_offset){
    Shader* text_shader = ShaderManager::UseShader("ui_2d_quad");

    BindUIQuad();

    mat4 modelview;

    modelview.identity();
    //modelview.translate(-offset.x,-offset.y,0);
    modelview.rotate_z(rotation);
    //modelview.translate(offset.x,offset.y,0);

    glUniform2f(text_shader->IMAGE_POS,x + offset.x + draw_offset.x, y + offset.y +draw_offset.y);
    glUniform2f(text_shader->IMAGE_SIZE,(float)glyph_texture.width_px,(float)glyph_texture.height_px);
    glUniform2f(text_shader->WINDOW_SIZE,(float)UI::UI_WIDTH,(float)UI::UI_HEIGHT);
    glUniform4fv(text_shader->COLOR,1,(GLfloat*)&color);
    glUniformMatrix4fv(text_shader->MODELVIEW_MATRIX,1,true,(GLfloat*)&modelview);
    glBindTexture(GL_TEXTURE_2D,glyph_texture.atlas_id);

    glUniform1i(text_shader->TEXTURE_0,0);
    glUniform4fv(text_shader->TEXTURE_LOCATION,1,(GLfloat*)&glyph_texture.tex_coords);

    glDrawArrays(GL_TRIANGLES,0,3);

    int err = glGetError(); 
    if(err != 0){logger::warn("UI_Rect.Draw() -> GL Error: %d \n",err);}
    
    //text_shader->OnFinishUse();
}
