#ifndef LOADING_MENU_H
#define LOADING_MENU_H

#include <game_project_1/gui/menu.hpp>

class LoadingMenu: public UI::Menu{
    public:

    UI::Widget* status_text_widget;

    LoadingMenu();
    
    void OnLoad();
    void OnUpdate(Timestep delta);
    void SetStatusMessage(wchar* status);
};






#endif