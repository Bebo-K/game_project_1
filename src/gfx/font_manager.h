#ifndef FONT_MANAGER_H
#define FONT_MANAGER_H

#include "texture.h"
#include <ft2build.h>
#include FT_FREETYPE_H


typedef int FontID;

namespace FontManager{
    const static char precached_codepoints_range_lo = 32;
    const static char precached_codepoints_range_hi = 126;
    const static char precached_codepoints_range = 126-32;
    const int ATLAS_SIZE = 1024;
    const int DEVICE_RES_X = 96;
    const int DEVICE_RES_Y = 96;
    const int DEFAULT_FONT_SIZE= 12;

    class FontCache{
        public:
        FT_Face fontface;
        int     font_size;
        GLuint  atlas_gl_id;
        Image*  glyph_atlas;
        int     dynamic_atlas_start[3];
        int     atlas_next_glyph[3];
        Texture glyph_static_textures[FontManager::precached_codepoints_range_hi-FontManager::precached_codepoints_range_lo];
        IDMap   glyph_dynamic_textures;

        FontCache();
        FontCache(char* font_filename,int font_size);
        void BuildAtlas();
        Texture AddDynamicGlyph(int codepoint);
        void ClearDynamicGlyphs();//Only method of glyph cleanup.
    };


    void Init();
    FontID LoadFontFace(char* font_filename,int font_size);
    void SetActiveFont(FontID font_id);
    FontCache* GetActiveFont();
    Texture GetGlyph(int code_point);
    //TODO: Text texture atlas cleanup
}



#endif