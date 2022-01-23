#ifndef COMPONENT_TEXT_H
#define COMPONENT_TEXT_H

#include "../widget.h"


namespace UI{


//Statically shown multi-line text. 
class TextBox: public Widget{
    public:
    FontID font;
    int font_size;
    int line_count;
    int max_line_length;
    wchar** line_buffers;
    UI_Text* shown_lines;
    bool wrap_lines;

    TextBox(char* name,FontID font,int line_count,int line_length,int width,int height);
    ~TextBox();
    
    void OnPaint();
    void OnMove();

    //void AddLine(wchar* newline);
    //void SetLine(wchar* line,int line_num);
    //void FormatLines();
};

//Player-input text. Chatboxes, naming screens, etc.
class TextEntryField:public Widget{
    public:
    bool focused;
    bool player_edited;//
    //bool acceptable;//
    int font_size;
    wchar* line;
    UI_Text* shown_line;
    //TODO: sanitize() callback for length requirements, acceptable characters/words 

    TextEntryField(char* name,int max_length);
    TextEntryField(char* name,int max_length, wchar* prompt);
    ~TextEntryField();

    void OnPaint(Widget* w);
    void OnResize(Widget* w);
};

}

#endif