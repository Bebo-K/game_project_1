#ifndef GUI_H
#define GUI_H

#include "../game/scene.h"
#include "../input.h"
#include "../struct/list.h"
#include "../struct/pool.h"
#include "layout.h"
#include "widget.h"
#include "widget/dev_console.h"
#include "menu.h"
#include "menu/main_menu.h"
#include "menu/options_menu.h"
#include "menu/loading_menu.h"
#include "menu/ingame_menu.h"
#include "menu/error_menu.h"
#include "menu/character_create_menu.h"

//Manages the screen real estate for menus,text, and other 2D objects
class GUI{
    private:
    static class GUI*   instance;
    public:

    List<UI::Menu>      menus;
    DeveloperConsole    developer_console;

    //List<Widget>      world_anchored_elements; TODO: things like player nameplates

    MainMenu*            main_menu;
    OptionsMenu*         options_menu;
    LoadingMenu*         loading_menu;
    ErrorMenu*           error_menu;
    IngameMenu*          ingame_menu;
    CharacterCreateMenu* character_create_menu;


    GUI();
    void Load();
    void Unload();
    void Reload();
    void CloseAll();

    static GUI* GetGUI();
    UI::Menu* GetMenu(int ID);
    
    //Client events
    void Paint();
    void Update(Scene* scene, int frames);

    //UI interaction events
    bool OnInput(Input::Event event_type);
    void OnSignal(EventSignal signal);
    void OnResize(int screen_w,int screen_h);

    //Debug
    void DebugLog(wchar* str);
};




#endif