#ifndef GUI_H
#define GUI_H

#include <game_project_1/input.hpp>
#include <game_project_1/types/list.hpp>
#include <game_project_1/types/timestep.hpp>
#include <game_project_1/client/client_scene.hpp>
#include <game_project_1/gui/layout.hpp>
#include <game_project_1/gui/widget/dev_console.hpp>

#include <game_project_1/gui/menu.hpp>
#include <game_project_1/gui/menu/main_menu.hpp>
#include <game_project_1/gui/menu/options_menu.hpp>
#include <game_project_1/gui/menu/loading_menu.hpp>
#include <game_project_1/gui/menu/ingame_menu.hpp>
#include <game_project_1/gui/menu/error_menu.hpp>
#include <game_project_1/gui/menu/character_create_menu.hpp>

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
    void Update(ClientScene* scene, Timestep delta);

    //UI interaction events
    bool OnInput(Input::Event event_type);
    void OnSignal(EventSignal signal);
    void OnResize(int screen_w,int screen_h);

    //Debug
    void DebugLog(wchar* str);
};




#endif