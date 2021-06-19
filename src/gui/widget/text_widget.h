#ifndef TEXT_VIEW_H
#define TEXT_VIEW_H

#include "../widget.h"
#include "../../gfx/ui_text.h"

//Statically shown UI text, e.g. labels
class TextWidget: public Widget{
    public:
    UI_Text text;
    TextWidget();
    TextWidget(char* str);
    TextWidget(char* str,FontID font);
    TextWidget(text_char* str);
    TextWidget(text_char* str,FontID font);
    ~TextWidget();
    void OnDestroy();
    
    void SetString(char* str);
    void SetString(text_char* str,FontID font);
    void OnPaint();
    void OnResize();
};

//Statically shown multi-line text. 
class TextBoxWidget: public Widget{
    public:
    int line_count;
    int max_line_length;
    bool wrap_lines;
    int font_size;
    text_char** lines;
    UI_Text* shown_lines;

    TextBoxWidget(int lines);
    TextBoxWidget(int lines,int line_length);
    ~TextBoxWidget();
    void OnDestroy();
    void OnPaint();

    void AddLine(text_char* newline);
    void SetLine(text_char* line,int line_num);
    void FormatLines();

};

//Player-input text. Chatboxes, naming screens, etc.
class TextEntryWidget:public Widget{
    public:
    bool focused;
    bool player_edited;//
    //bool acceptable;//
    int font_size;
    text_char* line;
    UI_Text* shown_line;
    //TODO: sanitize() callback for length requirements, acceptable characters/words 

    TextEntryWidget(int max_length);
    TextEntryWidget(int max_length, text_char* prompt);
    ~TextEntryWidget();
    void OnDestroy();

    void OnPaint();
};

#endif