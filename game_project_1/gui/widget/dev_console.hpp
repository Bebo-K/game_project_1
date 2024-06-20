#ifndef DEV_CONSOLE_H
#define DEV_CONSOLE_H

#include <game_project_1/types/timestep.hpp>
#include <game_project_1/gui/widget.hpp>
#include <game_project_1/gfx/ui_shape.hpp>
#include <game_project_1/gfx/font_manager.hpp>
#include <game_project_1/gfx/text.hpp>


class DeveloperConsole: public UI::Widget{
    private:
    static const int CACHED_LINE_COUNT = 64;
    static const int SHOWN_LINE_COUNT = 10;
    static const int MAX_LINE_LENGTH = 256;

    static DeveloperConsole* instance;

    int last_line_indx;
    wchar line_buffers[CACHED_LINE_COUNT*MAX_LINE_LENGTH];
    wchar entry_buffer[MAX_LINE_LENGTH];
    
    public:
    FontID console_font;
    int font_size =12;
    int line_wrap = 80;
    bool text_selected=false;

    DeveloperConsole();
    ~DeveloperConsole();

    void Load();
    void OnPaint();
    void OnUpdate(Timestep delta);
    bool OnInput(Input::Event event_type);
    void OnMove();
    void OnDestroy();

    void UpdateShownLines();
    
    static void Write(wchar* str);
};



#endif