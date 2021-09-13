#ifndef GUI_H
#define GUI_H

#include "../game/scene.h"
#include "../input.h"
#include "ui_types.h"
#include "widget.h"
#include "menu.h"
#include "menu/main_menu.h"
#include "menu/options_menu.h"
#include "menu/loading_menu.h"
#include "menu/ingame_menu.h"


//Manages the screen real estate for menus,text, and other 2D objects
class GUI{
    private:
    static class GUI*   instance;
    public:
    UI::Layout          fullscreen_layout;
    List<UI::Widget>    debug_widgets;

    //List<Widget>      world_anchored_elements; TODO: things like player nameplates

    MainMenu*            main_menu;
    OptionsMenu*         options_menu;
    LoadingMenu*         loading_menu;
    IngameMenu*          ingame_menu;

    List<Menu>           menus;

    GUI();
    void Load();
    void Unload();
    void Reload();

    static GUI* GetGUI();
    Menu* GetMenu(int ID);
    
    //Client events
    void Paint();
    void Update(Scene* scene, int frames);

    //UI interaction events
    bool OnInput(Input::Event event_type);
    void OnSignal(UI::Signal signal);
    void OnResize(int screen_w,int screen_h);
};




#endif