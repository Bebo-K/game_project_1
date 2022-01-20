#ifndef COMPONENT_TEXT_H
#define COMPONENT_TEXT_H

#include "../widget.h"


namespace UI{


//Statically shown multi-line text. 
class TextBox: public Widget{
    public:
    int line_count;
    int max_line_length;
    bool wrap_lines;
    int font_size;
    wchar** lines;
    UI_Text* shown_lines;

    TextBox(char* name,int lines);
    TextBox(char* name,int lines,int line_length);
    ~TextBox();
    
    void OnPaint(Widget* w);
    void OnResize(Widget* w);

    void AddLine(wchar* newline);
    void SetLine(wchar* line,int line_num);
    void FormatLines();
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