#ifndef GUI_H
#define GUI_H

#include "../game/scene.h"
#include "../gfx/renderer.h"
#include "../input.h"
#include "menu.h"
#include "ui_types.h"


//Manages the screen real estate for menus,text, and other 2D objects
class GUI{
    private:
    UI::Menu*           menus[4];
    public:
    UI::Layout          fullscreen_layout;
    List<Widget>        debug_widgets;

    
    //MainMenu            main_menu;
    //OptionsMenu         options_menu;
    //LoadingMenu         loading_menu;
    //IngameMenu          ingame_menu;

    GUI();
    void Load();
    void Unload();
    void Reload();
    
    //For modding later:
    //void RegisterMenu(MenuID menu,Menu* menu);

    //Client events
    void Paint();
    void Update(Scene* scene, int frames);

    //UI interaction events
    bool OnInput(Input::Event event_type);
    void OnSignal(UISignal signal);
    void OnResize(int screen_w,int screen_h);
};




#endif