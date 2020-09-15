#include "text.h"
#include "../log.h"


float glyph_vert_data[] ={1,1,0,  1,0,0,  0,0,0,   0,0,0,  0,1,0,  1,1,0};
float glyph_texcoord_data[] = {1,0,  1,1, 0,1,   0,1,  0,0,  1,0};
VBO glyph_vertices;
VBO glyph_texcoords;

const float TEXT_SCALE = 0.5f;


text_string TextString::from_cstr(char* str){
    if(str==null)return null;
    int i;
    for(i=0;str[i]!=0;i++);
    text_string ret = (text_string)new int[i+1];
    ret[i]=0;
    for(i=0;str[i]!=0;i++){ret[i] = (int)str[i];}
    return ret;
}

void BuildGlyphPrimitive(){
    glyph_vertices.Create(glyph_vert_data,GL_FLOAT,3,18);
    glyph_texcoords.Create(glyph_texcoord_data,GL_FLOAT,2,12);
    int err = glGetError();
    if(err != 0){
        logger::warn("Error building text glyph primitive, code: %d \n",err);
    }
}

SimpleText::SimpleText(char* str){
    layer=32;
    scale={1,1,1};
    if(!glyph_vertices.Valid()){BuildGlyphPrimitive();}
    SetString(TextString::from_cstr(str),-1);
}
SimpleText::SimpleText(text_string str){
    layer=32;
    scale={1,1,1};
    if(!glyph_vertices.Valid()){BuildGlyphPrimitive();}
    SetString(str,-1);
}
SimpleText::SimpleText(text_string str,int font_id){
    layer=32;
    scale={1,1,1};
    if(!glyph_vertices.Valid()){BuildGlyphPrimitive();}
    SetString(str,font_id);
}

void SimpleText::SetString(text_string str,int font_id){
    if(str ==null)return;
    if(font_id >= 0){FontManager::SetActiveFont(font_id);}
    string=str;
    FT_Face fontface = FontManager::GetActiveFont()->fontface;

    int strlen=0;
    for(int i=0;str[i] != 0;i++){strlen++;}
    glyph_count = strlen;
    glyphs = (Glyph*)calloc(glyph_count,sizeof(Glyph));

    float pen_x,pen_y;

    for(int i=0;str[i] != 0;i++){
        int err = FT_Load_Char(fontface,str[i],FT_LOAD_DEFAULT);
        if(err != 0){logger::warn("Unable to load glyph. Code:%d\n",err);}        

        glyphs[i].x = pen_x + fontface->glyph->bitmap_left;
        glyphs[i].y = pen_y + fontface->glyph->bitmap_top;
        glyphs[i].glyph_texture = FontManager::GetGlyph(str[i]);

        pen_x += fontface->glyph->advance.x/64.0f;//advance is 1/64th of a pixel
        pen_y += fontface->glyph->advance.y/64.0f;
    }
}

SimpleText::~SimpleText(){
    free(glyphs);
}

void SimpleText::Update(int frames){}
void SimpleText::Draw(Camera* cam,mat4* view, mat4* projection){
    cam->SetShader("ui_default");
    glDisable(GL_DEPTH_TEST);
    glEnableVertexAttribArray(cam->shader->ATTRIB_VERTEX);
    glEnableVertexAttribArray(cam->shader->ATTRIB_TEXCOORD);
    glyph_vertices.Bind(0);
    glyph_texcoords.Bind(1);

    glUniformMatrix4fv(cam->shader->PROJECTION_MATRIX,1,true,(GLfloat*)projection);
    view->translate(x,y,0);
    //view->scale(TEXT_SCALE,TEXT_SCALE,1);

    mat4 base_view;
    glActiveTexture(GL_TEXTURE0);
    for(int i=0;i<glyph_count;i++){
        base_view.set(view);


        base_view.translate(glyphs[i].x,glyphs[i].y,0);
        base_view.scale(glyphs[i].glyph_texture.width_px,glyphs[i].glyph_texture.height_px,1);
        
        glUniformMatrix4fv(cam->shader->MODELVIEW_MATRIX,1,true,(GLfloat*)&base_view);
        
        //glUniform4fv(cam->shader->COLOR,1,(GLfloat*)glyphs[i].color);
        glBindTexture(GL_TEXTURE_2D,glyphs[i].glyph_texture.atlas_id);

        glUniform1i(cam->shader->TEXTURE_0,0);
        glUniform4fv(cam->shader->TEXTURE_LOCATION,1,(GLfloat*)&glyphs[i].glyph_texture.tex_coords);

        glDrawArrays(GL_TRIANGLES,0,6);
    }
;

    glDisableVertexAttribArray(cam->shader->ATTRIB_VERTEX);
    glDisableVertexAttribArray(cam->shader->ATTRIB_TEXCOORD);
    
    glEnable(GL_DEPTH_TEST);

}
