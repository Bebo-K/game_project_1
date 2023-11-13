#include <game_project_1/gfx/font_manager.hpp>
#include <game_project_1/gfx/drawable.hpp>
#include <game_project_1/types/list.hpp>
#include <game_project_1/io/asset_manager.hpp>
#include <game_project_1/io/log.hpp>
#include <game_project_1/os.hpp>

FT_Library   ft_library;
FontID       default_font_id,current_font_id;
List<FontManager::FontCache> cached_fonts;


void FontID::operator=(FontID f2){family_id = f2.family_id;size=f2.size;}
bool FontID::operator==(FontID f2){return family_id == f2.family_id && size == f2.size;}


void FontManager::Init(){
    int err = FT_Init_FreeType(&ft_library);
    if(err != 0){
        logger::warn("Error during Freetype2 library initialization, code: %d \n",err);
    }
    current_font_id={-1,18};
    default_font_id = LoadFontFace("Merriweather/Merriweather-Regular",18);
}
void FontManager::Free(){
    for(FontManager::FontCache* font: cached_fonts){
        font->ClearDynamicGlyphs();
        delete font->glyph_atlas;
        free(font);
    }
    cached_fonts.Clear();
    FT_Done_FreeType(ft_library);
}

FontID FontManager::LoadFontFace(char* font_name,int font_size){
    for(FontCache* font: cached_fonts){
        if(cstr::compare(font->font_name,font_name) && font->font_size==font_size){
            font->users++;
            return {cached_fonts.GetIndex(font),font->font_size};
        }
    }
    FontCache* fc = new FontCache(font_name,font_size);
    fc->users=1;
    int newid = cached_fonts.Add(fc);
    return (FontID){newid,font_size};
}

void FontManager::SetActiveFont(FontID font){current_font_id = font;}

FontManager::FontCache* FontManager::GetActiveFont(){
    if(current_font_id.family_id < 0){return cached_fonts[default_font_id.family_id];}
    FontCache* cache_entry = cached_fonts[current_font_id.family_id];
    if(cache_entry == null){return cached_fonts[default_font_id.family_id];}
    return cache_entry;
}

FontManager::FontCache* FontManager::GetFontInfo(FontID id){
    return cached_fonts[id.family_id];
}

//returns either the existing or a new font ID with the newly scaled fontface
FontID FontManager::RescaleFontID(FontID font_id,int new_font_size){
    if(font_id.family_id < 0){
        return {-1,18};
    }
    FontManager::FontCache* font = cached_fonts[font_id.family_id];
    if(font==nullptr || font->font_size == new_font_size)return font_id;

    if(font->users > 1){//Don't alter fontfaces if others might be using it
        font->users -=1;
        return LoadFontFace(font->font_name,new_font_size);
    }

    for(FontCache* cached_font: cached_fonts){//First look to see if there's a scaled font to re-use
        if(cstr::compare(font->font_name,cached_font->font_name) && font->font_size==cached_font->font_size){
            cached_fonts.Remove(font);
            
            
            font->ClearDynamicGlyphs();
            delete font->glyph_atlas;
            //free(font);

            cached_font->users++;
            return {cached_fonts.GetIndex(cached_font),cached_font->font_size};
        }
    }

    //Finally replace this fontface with scaled one and delete old fontface.
    cached_fonts.Remove(font);
    cached_fonts.Set(font_id.family_id,new FontManager::FontCache(font->font_name,new_font_size));

    font->ClearDynamicGlyphs();
    delete font->glyph_atlas;
    //free(font);

    return font_id;
}

Texture FontManager::GetGlyph(int code_point){
    FontCache* font = GetActiveFont();

    if(code_point >= precached_codepoints_range_lo && code_point <= precached_codepoints_range_hi){
        return font->glyph_static_textures[code_point-precached_codepoints_range_lo];
    }
    Texture* ret = font->glyph_dynamic_textures.Get(code_point);
    if(ret != null){ return *ret;}
    else return font->AddDynamicGlyph(code_point);
}


FontManager::FontCache::FontCache():glyph_dynamic_textures(8){
    font_name=nullptr;font_size=8;
}

FontManager::FontCache::FontCache(char* font_uri,int font_point):glyph_dynamic_textures(8){
    users=0;
    font_name=cstr::new_copy(font_uri);
    font_size=font_point;
    atlas_next_glyph[0]=atlas_next_glyph[1]=atlas_next_glyph[2]=0;
    dynamic_atlas_start[0]=dynamic_atlas_start[1]=dynamic_atlas_start[2]=0;

    Stream* font_stream = AssetManager::UI_Font(font_uri);
    byte* font_file = font_stream->readAll();
    int err = FT_New_Memory_Face(ft_library,font_file,font_stream->amount_read,0,&fontface);
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
    
    //free(font_file); Freetype segfault: "you cannot free the backing data for a fontface before calling FT_Done_Face:
    delete font_stream;
}

FontManager::FontCache::~FontCache(){
    FT_Done_Face(fontface);
    glDeleteTextures(1,&atlas_gl_id);
    delete glyph_atlas;
    if(font_name!=nullptr){free(font_name);font_name=nullptr;}
}

TextureRectangle BlitGlyphToAtlas(Image* atlas,FT_Bitmap glyph,int atlas_next[3]){
    TextureRectangle pos;
    int atlas_x = atlas_next[0];
    int atlas_y = atlas_next[1];
    int atlas_h = atlas_next[2];
    if(atlas_x + glyph.width <= FontManager::ATLAS_SIZE 
    && atlas_y + glyph.rows <= FontManager::ATLAS_SIZE){//Fits widthwise our row
        atlas_h = (glyph.rows > (unsigned int)atlas_h)?glyph.rows:atlas_h;
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
        int err = FT_Load_Glyph(fontface,glyph,FT_LOAD_DEFAULT);
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
    
    glyph_dynamic_textures.Add(codepoint,glyph_tex);
    return *glyph_tex;
}

void FontManager::FontCache::ClearDynamicGlyphs(){
    for(Tuple<int,Texture*> t: glyph_dynamic_textures){
        if(t.value != nullptr){free(t.value);}
    }
    glyph_dynamic_textures.Clear();
}

bool FontManager::FontCache::operator==(FontCache* f2){
    return (cstr::compare(font_name,f2->font_name) && font_size == f2->font_size);
}