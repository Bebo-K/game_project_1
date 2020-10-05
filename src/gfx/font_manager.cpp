#include "font_manager.h"
#include "../log.h"
#include "drawable.h"
#include "../os.h"
#include "freetype/ftcolor.h"

FT_Library   ft_library;
FontID       default_font_id,current_font_id;
PointerArray cached_fonts(1);

void FontManager::Init(){
    int err = FT_Init_FreeType(&ft_library);
    if(err != 0){
        logger::warn("Error during Freetype2 library initialization, code: %d \n",err);
    }
    current_font_id=-1;
    default_font_id = LoadFontFace("dat/ui/fonts/Merriweather/Merriweather-Regular.ttf",18);
}

FontID FontManager::LoadFontFace(char* font_name,int font_size){
    FontCache* cache_entry = new FontCache(font_name,font_size);
    return cached_fonts.Add(cache_entry);
}

void FontManager::SetActiveFont(FontID font){current_font_id = font;}

FontManager::FontCache* FontManager::GetActiveFont(){
    FontCache* cache_entry = (FontCache*)cached_fonts.Get(current_font_id);
    if(cache_entry == null){return (FontCache*)cached_fonts.Get(default_font_id);}
    return cache_entry;
}

Texture FontManager::GetGlyph(int code_point){
    FontCache* font = GetActiveFont();

    if(code_point >= precached_codepoints_range_lo && code_point <= precached_codepoints_range_hi){
        return font->glyph_static_textures[code_point-precached_codepoints_range_lo];
    }
    Texture* ret = (Texture*)font->glyph_dynamic_textures.Get(code_point);
    if(ret != null){ return *ret;}
    else return font->AddDynamicGlyph(code_point);
}


FontManager::FontCache::FontCache():glyph_dynamic_textures(8){}

FontManager::FontCache::FontCache(char* font_filename,int font_size):glyph_dynamic_textures(8){
    font_size=font_size;
    atlas_next_glyph[0]=atlas_next_glyph[1]=atlas_next_glyph[2]=0;
    dynamic_atlas_start[0]=dynamic_atlas_start[1]=dynamic_atlas_start[2]=0;

    int err = FT_New_Face(ft_library,font_filename,0,&fontface);
    if (err != 0){
        if (err == FT_Err_Unknown_File_Format){
            logger::warn("Error building Freetype2 font: font type unsupported\n");
        }
        else logger::warn("Error building Freetype2 font: font cound not be read\n",err);
    }
    err = FT_Set_Char_Size(
            fontface,       /* handle to face object           */
            0,              /* char_width in 1/64th of points  */
            font_size*64,   /* char_height in 1/64th of points */
            Window::DPI,   /* horizontal device resolution    */
            Window::DPI ); /* vertical device resolution      */
    if(err != 0){
        logger::warn("Error building Freetype2 font: Unable to set font size. Code:%d\n",err);
    }

    glGenTextures(1,&atlas_gl_id);
    BuildAtlas();
    //For if we need to clear our extra glyphs
    dynamic_atlas_start[0] = atlas_next_glyph[0];
    dynamic_atlas_start[1] = atlas_next_glyph[1];
    dynamic_atlas_start[2] = atlas_next_glyph[2];
}

TextureRectangle BlitGlyphToAtlas(Image* atlas,FT_Bitmap glyph,int atlas_next[3]){
    TextureRectangle pos;
    unsigned int atlas_x = (unsigned int)atlas_next[0];
    unsigned int atlas_y = (unsigned int)atlas_next[1];
    unsigned int atlas_h = (unsigned int)atlas_next[2];
    if(atlas_x + glyph.width <= FontManager::ATLAS_SIZE 
    && atlas_y + glyph.rows <= FontManager::ATLAS_SIZE){//Fits widthwise our row
        atlas_h = (glyph.rows > atlas_h)?glyph.rows:atlas_h;
    }
    else if(atlas_y + atlas_h + glyph.rows < FontManager::ATLAS_SIZE){//fits below our row
        atlas_x = 0;
        atlas_y += atlas_h;
        atlas_h = glyph.rows;
    }	
    unsigned char alpha;
    for(unsigned int i=0;i<glyph.rows;i++){
        for(unsigned int j=0; j<glyph.width;j++){
            alpha=glyph.buffer[glyph.width*i + j];
            if(alpha > 16){//edge softening. Do I want this?
                if(j==0 || glyph.buffer[glyph.width*i + j -1] < 16){alpha /= 2;}
                else if(j==glyph.width-1 || glyph.buffer[glyph.width*i + j +1] < 16){alpha /= 2;}
            }
            atlas->image_data[(atlas->width*(atlas_y+i) + (atlas_x+j))*4] = 255;//glyph.buffer[(glyph.width*i + j)];
            atlas->image_data[(atlas->width*(atlas_y+i) + (atlas_x+j))*4+1] = 255;//glyph.buffer[(glyph.width*i + j)];
            atlas->image_data[(atlas->width*(atlas_y+i) + (atlas_x+j))*4+2] = 255;//glyph.buffer[(glyph.width*i + j)];
            atlas->image_data[(atlas->width*(atlas_y+i) + (atlas_x+j))*4+3] = alpha;
        }
    }
    
    pos.x = ((float)atlas_x)/FontManager::ATLAS_SIZE;
    pos.y = ((float)atlas_y)/FontManager::ATLAS_SIZE;
    pos.w = ((float)glyph.width)/FontManager::ATLAS_SIZE;
    pos.h = ((float)glyph.rows)/FontManager::ATLAS_SIZE;

    atlas_x += glyph.width;

    atlas_next[0] = atlas_x;
    atlas_next[1] = atlas_y;
    atlas_next[2] = atlas_h;

    return pos;
}

void SubmitAtlas(Image* atlas,int guint){
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D,guint);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_BASE_LEVEL, 0);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, 0);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D,0,GL_RGBA,atlas->width,atlas->height,0,
            GL_RGBA,GL_UNSIGNED_BYTE,atlas->image_data);
}

void FontManager::FontCache::BuildAtlas(){
    glyph_atlas = AlgorithmicImage::Gradient(FontManager::ATLAS_SIZE);// Image(FontManager::ATLAS_SIZE,FontManager::ATLAS_SIZE);

    for(int i=0;i < precached_codepoints_range;i++){
        int codepoint = precached_codepoints_range_lo+i;
        int glyph = FT_Get_Char_Index(fontface,codepoint);
        int err = FT_Load_Glyph(fontface,glyph,FT_LOAD_COLOR);
        if(err != 0){logger::warn("Unable to load glyph. Code:%d\n",err);}        
        err = FT_Render_Glyph(fontface->glyph,FT_RENDER_MODE_NORMAL);
        if(err != 0){logger::warn("Unable to render glyph. Code:%d\n",err);}
        glyph_static_textures[i].atlas_id=atlas_gl_id;
        glyph_static_textures[i].height_px= fontface->glyph->bitmap.rows;
        glyph_static_textures[i].width_px= fontface->glyph->bitmap.width;
        glyph_static_textures[i].tex_coords = BlitGlyphToAtlas(glyph_atlas,fontface->glyph->bitmap,atlas_next_glyph);
    }
    SubmitAtlas(glyph_atlas,atlas_gl_id);
}

Texture FontManager::FontCache::AddDynamicGlyph(int codepoint){
    int err = FT_Load_Char(fontface,codepoint,FT_LOAD_DEFAULT);
    if(err != 0){logger::warn("Unable to load glyph. Code:%d\n",err);}        
    err = FT_Render_Glyph(fontface->glyph,FT_RENDER_MODE_NORMAL);
    if(err != 0){logger::warn("Unable to render glyph. Code:%d\n",err);}

    Texture* glyph_tex = new Texture();

    glyph_tex->atlas_id=atlas_gl_id;
    glyph_tex->height_px= fontface->glyph->bitmap.rows;
    glyph_tex->width_px= fontface->glyph->bitmap.width;
    glyph_tex->tex_coords = BlitGlyphToAtlas(glyph_atlas,fontface->glyph->bitmap,atlas_next_glyph);
    SubmitAtlas(glyph_atlas,atlas_gl_id);
    
    glyph_dynamic_textures.Add(codepoint,(byte*)glyph_tex);
    return *glyph_tex;
}

void FontManager::FontCache::ClearDynamicGlyphs(){
    for(int i=0;i<glyph_dynamic_textures.slots;i++){
        if(glyph_dynamic_textures.slot_is_filled.Get(i)==false)continue;
        Texture* glyph_tex = (Texture*)glyph_dynamic_textures.values[i];
        free(glyph_tex);
    }   
    glyph_dynamic_textures.Clear();
}