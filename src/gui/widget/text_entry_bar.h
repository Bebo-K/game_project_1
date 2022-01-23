#ifndef TEXT_ENTRY_BAR
#define TEXT_ENTRY_BAR

#include "../widget.h"

namespace UI{

class TextEntryKeyboardOverlay{
    public:

    int grid_rows;
    int grid_columns;

    wchar** characters;

    UI_Rect background;
    UI_Rect selector;

    Glyph* letters;
    Sprite finished;

    TextEntryKeyboardOverlay();
    ~TextEntryKeyboardOverlay();
    void Draw();
    void OnInput(Widget* w,Input::Event event_type);
};


class TextEntryBar: public  UI::Widget{
    public:
        TextEntryKeyboardOverlay overlay;




    TextEntryBar(char* label,wchar* text);
    ~TextEntryBar();
    void OnMove();
    void OnDraw();
    void OnPaint();


};

}





#endif