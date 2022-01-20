#ifndef ERROR_MENU_H
#define ERROR_MENU_H

#include "../menu.h"

class ErrorMenu: public UI::Menu{
    public:

    UI::Widget* status_text_widget;

    ErrorMenu(UI::Layout* parent);
    
    void OnLoad();
    void OnUpdate(int frames);
    void SetStatusMessage(wchar* status);
};






#endif