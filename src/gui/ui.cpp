#include "ui.h"
#include "../os.h"
#include "../config.h"
#include "../log.h"
#include <math.h>
#include "../test/perf.h"

#include "widgets/text_widget.h"
#include "widgets/dev_console.h"

#include "menus/main_menu.h"
#include "menus/loading_menu.h"
#include "menus/ingame_menu.h"

UI::UI():debug_widgets(1){
    current_screen = nullptr;
}

DeveloperConsole* game_console;

void UI::Load(){
    logger::info("loading ui...\n");
    game_console = new DeveloperConsole();
    game_console->layout.offset.parent = &fullscreen_layout;
    game_console->active = false;
    debug_widgets.Add("console",(byte*)game_console);
    DeveloperConsole::Write("hello world!");
}
    
    
UIWindow* BuildMenu(int window_id){
    switch(window_id){
        case UI::MAIN_MENU: return new MainMenu(); break;
        case UI::LOADING: return new LoadingMenu(); break;
        case UI::INGAME: return new IngameMenu(); break;
        default: break;
    }
    return nullptr;
}

UIWindow* UI::OpenWindow(int window_id){
    if(current_screen != nullptr){CloseWindow();}
    current_screen = BuildMenu(window_id);
    current_screen->active=true;
    current_screen->visible=true;
    return current_screen;
}

void UI::CloseWindow(){
    free(this->current_screen);
    current_screen=nullptr;
}

void UI::Paint(){
    if(current_screen && current_screen->visible){
        current_screen->Paint();
    }
    for(int i=0;i<debug_widgets.Max();i++){
        Widget* pWidget = (Widget*)debug_widgets.At(i);
        if(pWidget != null){pWidget->Paint();}
    }
}

void UI::Update(Scene* scene, int frames){
    if(current_screen && current_screen->active){
        current_screen->Update(frames);
    }
    
    for(int i=0;i<debug_widgets.Max();i++){
        Widget* pWidget = (Widget*)debug_widgets.At(i);
        if(pWidget != null){pWidget->OnUpdate(frames);}
    }
}

void UI::Unload(){
    logger::info("unloading ui...\n");
    delete game_console;
}

bool  UI::OnInput(Input::EventID event_type){
    bool handled = false;
    for(int i=0;i<debug_widgets.Max();i++){
        Widget* pWidget = (Widget*)debug_widgets.At(i);
        if(pWidget != null && pWidget->OnInput(event_type)){handled = true;}
    }
    if(current_screen && current_screen->active){
        handled = current_screen->HandleInput(event_type);
    }
    return handled;
}
void UI::OnSignal(int signal_id,int metadata_len, byte* metadata){
    bool handled = false;
    for(int i=0;i<debug_widgets.Max();i++){
        Widget* pWidget = (Widget*)debug_widgets.At(i);
        if(pWidget != null && pWidget->OnSignal(signal_id,metadata_len,metadata)){handled = true;}
    }
    if(current_screen && current_screen->active){
        handled = current_screen->HandleSignal(signal_id,metadata_len,metadata);
    }
}

void UI::OnResize(int screen_w,int screen_h){
    fullscreen_layout.X=0;
    fullscreen_layout.Y=0;
    fullscreen_layout.W=screen_w;
    fullscreen_layout.H=screen_h;
    for(int i=0;i<debug_widgets.Max();i++){
        Widget* pWidget = (Widget*)debug_widgets.At(i);
        if(pWidget != null){pWidget->OnResize();}
    }
    if(current_screen){
        current_screen->HandleResize();
    }
}
