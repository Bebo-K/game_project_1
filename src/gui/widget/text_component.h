#ifndef TEXT_COMPONENT_H
#define TEXT_COMPONENT_H

#include "../widget.h"
#include "../../gfx/ui_text.h"

namespace UI{

//Statically shown UI text, e.g. labels
class TextComponent: public WidgetComponent{
    public:
    UI_Text text;
    TextComponent();
    TextComponent(char* str);
    TextComponent(char* str,FontID font);
    TextComponent(text_char* str);
    TextComponent(text_char* str,FontID font);

    void SetString(char* str);
    void SetString(text_char* str,FontID font);
    void OnPaint(Widget* w);
    void OnResize(Widget* w);
};

//Statically shown multi-line text. 
class TextBoxComponent: public WidgetComponent{
    public:
    int line_count;
    int max_line_length;
    bool wrap_lines;
    int font_size;
    text_char** lines;
    UI_Text* shown_lines;

    TextBoxComponent(int lines);
    TextBoxComponent(int lines,int line_length);
    ~TextBoxComponent();
    
    void OnPaint(Widget* w);
    void OnResize(Widget* w);

    void AddLine(text_char* newline);
    void SetLine(text_char* line,int line_num);
    void FormatLines();
};

//Player-input text. Chatboxes, naming screens, etc.
class TextEntryComponent:public Widget{
    public:
    bool focused;
    bool player_edited;//
    //bool acceptable;//
    int font_size;
    text_char* line;
    UI_Text* shown_line;
    //TODO: sanitize() callback for length requirements, acceptable characters/words 

    TextEntryComponent(int max_length);
    TextEntryComponent(int max_length, text_char* prompt);
    ~TextEntryComponent();

    void OnPaint(Widget* w);
    void OnResize(Widget* w);
};

}

#endif