#ifndef GUI_H
#define GUI_H

#include "../game/scene.h"
#include "../gfx/renderer.h"
#include "../input.h"
#include "menu.h"

typedef int MenuID;
struct MenuType{
    //0 is reserved for "no menu" constant
    static const MenuID MAIN_MENU=1;
    static const MenuID LOADING=2;
    static const MenuID INGAME=3;
    static const MenuID OPTIONS=4;
};

//Manages the screen real estate for menus,text, and other 2D objects
class GUI{
    public:
    const static int    MENU_STACK_MAX=32;
    UI::Menu*           menu_stack[MENU_STACK_MAX];
    Layout              fullscreen_layout;
    WidgetContainer     debug_widgets;

    GUI();
    void Load();
    void Unload();
    //void Reload();

    void OpenMenu(MenuID menu);
    //void OpenMenu(MenuID menu,MenuContext* context);
    void CloseMenu(MenuID menu);
    bool IsMenuOpen(MenuID menu);

    template <class T> T* GetMenu(MenuID menu);
    
    //For modding later:
    //void RegisterMenu(MenuID menu,Menu* menu);

    //Client events
    void Paint();
    void Update(Scene* scene, int frames);

    //UI interaction events
    bool OnInput(Input::Event event_type);
    void OnSignal(int signal_id,int metadata_len, byte* metadata);
    void OnResize(int screen_w,int screen_h);
};




#endif