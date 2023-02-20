#ifndef SIMPLE_BUTTON_WIDGET_H
#define SIMPLE_BUTTON_WIDGET_H

#include <game_project_1/gui/widget.hpp>

namespace UI{    

class SimpleButton:public Widget{
    public:
    void (*select_callback)();

    SimpleButton(char* name, wchar* label, float w, float h, color_f color, void (*callback)());
    ~SimpleButton();

};

class MenuBackground: public Widget{
    public:
    MenuBackground();
    ~MenuBackground();
};

}

#endif