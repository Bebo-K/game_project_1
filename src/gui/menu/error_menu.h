#ifndef ERROR_MENU_H
#define ERROR_MENU_H

#include "../menu.h"
#include "../widget/text_component.h"

class ErrorMenu: public UI::Menu{
    public:

    UI::TextComponent* h_status_text;

    ErrorMenu(UI::Layout* parent);
    
    void OnLoad();
    void OnUpdate(int frames);
    void SetStatusMessage(char* status);
};






#endif