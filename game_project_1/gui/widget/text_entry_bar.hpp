#ifndef TEXT_ENTRY_BAR
#define TEXT_ENTRY_BAR

#include <game_project_1/gui/widget.hpp>

namespace UI{

class TextEntryKeyboardOverlay{
    public:
    const static int MAX_ENTRY_LENGTH=24;
    Layout layout;
    UI_Rect background;
    UI_Rect entry_line;
    UI_Rect selector;

    int grid_size;
    int grid_layers;
    int current_layer;

    int grid_rows;
    int grid_columns;

    bool active;
    point_i selected;
    
    Sprite back;
    point_i back_pos;

    Sprite shift1;
    point_i shift1_pos;

    Sprite end;
    point_i end_pos;

    Glyph* letters;
    wchar** layers;
    DrawableText entry;

    FontID fontface;


    TextEntryKeyboardOverlay();
    ~TextEntryKeyboardOverlay();
    void Draw();
    void OnMove();
    bool OnInput(Widget* w,Input::Event event_type);
    point_i CharLocation(int codepoint);
    wchar CharAt(int x,int y);
    void LoadLayer();
    void OnSelect(Widget* text_entry);
    void UpdateSelection();
    void AddSelectedChar();
    void RemoveChar();
};


class TextEntryBar: public  UI::Widget{
    public:
        TextEntryKeyboardOverlay overlay;
        wchar** string_hook;




    TextEntryBar(char* label,wchar* text,wchar** hook);
    ~TextEntryBar();
    void OnMove();
    void OnDraw();
    void OnPaint();
    void UpdateHook();


};

}





#endif