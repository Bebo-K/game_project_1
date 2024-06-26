#ifndef FONT_MANAGER_H
#define FONT_MANAGER_H

#include <game_project_1/types/map.hpp>
#include <game_project_1/gfx/texture.hpp>
#include <ft2build.h>
#include FT_FREETYPE_H


struct FontID{
    int family_id;//TODO; Right now fonts of the same family don't share a family ID, it's just a UUID.
    int size;

    void operator=(FontID f2);
    bool operator==(FontID f2);
};

namespace FontManager{
    const static char precached_codepoints_range_lo = 32;
    const static char precached_codepoints_range_hi = 126;
    const static char precached_codepoints_range = 126-32;
    const int ATLAS_SIZE = 1024;
    const int DEVICE_RES_X = 96;
    const int DEVICE_RES_Y = 96;
    const int DEFAULT_FONT_SIZE= 12;

    class FontCache{
        private:
        int     dynamic_atlas_start[3];
        int     atlas_next_glyph[3];

        public:
        int     users;
        char*   font_name;
        int     font_size;
        FT_Face fontface;
        GLuint  atlas_gl_id;
        Image*  glyph_atlas;
        Texture glyph_static_textures[FontManager::precached_codepoints_range_hi-FontManager::precached_codepoints_range_lo];
        Map<int,Texture>   glyph_dynamic_textures;

        FontCache();
        FontCache(char* font_filename,int font_size);
        ~FontCache();
        void BuildAtlas();
        Texture AddDynamicGlyph(int codepoint);
        void ClearDynamicGlyphs();//Only method of glyph cleanup.
        bool operator==(FontCache* f2);
    };


    void Init();
    void Free();
    FontID LoadFontFace(char* font_filename,int font_size);
    void SetActiveFont(FontID font_id);
    FontCache* GetActiveFont();
    FontCache* GetFontInfo(FontID font_id);
    FontID RescaleFontID(FontID font_id,int new_font_size); 
    Texture GetGlyph(int code_point);
    //TODO: Text texture atlas cleanup
}



#endif