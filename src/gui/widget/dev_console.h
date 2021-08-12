#ifndef DEV_CONSOLE_H
#define DEV_CONSOLE_H

#include "../widget.h"
#include "../../gfx/ui_text.h"
#include "../../gfx/ui_shape.h"
#include "../../gfx/font_manager.h"
#include "../../gfx/ui_text.h"


class DeveloperConsole: public Widget{
    private:
    static const int CACHED_LINE_COUNT = 64;
    static const int SHOWN_LINE_COUNT = 10;
    static const int MAX_LINE_LENGTH = 256;

    static DeveloperConsole* instance;
    static FontID console_font;

    int last_line_indx;
    text_char line_buffers[CACHED_LINE_COUNT*MAX_LINE_LENGTH];
    text_char entry_buffer[MAX_LINE_LENGTH];
    
    public:
    int  start_timer;//hack to get the character that spawned the console from being immediately printed.
    int font_size =12;
    int line_wrap = 80;
    UI_Text shown_lines[SHOWN_LINE_COUNT];
    UI_Text entry_line;
    UI_Rect background_rect;
    UI_Rect entry_rect;

    DeveloperConsole();
    ~DeveloperConsole();

    void OnPaint();
    void OnUpdate(int frames);
    bool OnInput(Input::Event event_type);
    void OnResize();
    void OnDestroy();

    void UpdateShownLines();
    
    static void Write(char* str);
};



#endif