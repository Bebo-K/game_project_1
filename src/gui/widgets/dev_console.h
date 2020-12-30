#ifndef DEV_CONSOLE_H
#define DEV_CONSOLE_H

#include "../../gfx/ui_text.h"
#include "../../gfx/ui_shape.h"
#include "../widget.h"



class DeveloperConsole:public Widget{
    static const int CACHED_LINE_COUNT = 64;
    static const int SHOWN_LINE_COUNT = 10;
    static const int MAX_LINE_LENGTH = 256;

    static DeveloperConsole* instance;
    static FontID console_font;

    int last_line_indx;
    text_char line_buffers[CACHED_LINE_COUNT*MAX_LINE_LENGTH];
    text_char entry_buffer[MAX_LINE_LENGTH];
    
    public:
    bool active = true;
    int font_size =12;
    int line_wrap = 80;
    UIText shown_lines[SHOWN_LINE_COUNT];
    UIText entry_line;
    UI_Rect background_rect;
    UI_Rect entry_rect;

    DeveloperConsole();
    ~DeveloperConsole();

    void Paint();
    void OnUpdate(int frames);
    bool OnInput(InputEvent event_type);
    void OnResize();
    bool OnSignal(int signal_id,int metadata_len, byte* metadata);
    

    static void Write(char* str);
    void UpdateShownLines();
};



#endif