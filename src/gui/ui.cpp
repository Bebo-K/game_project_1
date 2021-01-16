#include "ui.h"
#include "../os.h"
#include "../config.h"
#include "../log.h"
#include <math.h>
#include "../test/perf.h"

#include "widgets/text_widget.h"
#include "widgets/dev_console.h"

UI::UI(){}

DeveloperConsole* game_console;

void UI::Load(){
    logger::info("loading ui...\n");
    game_console = new DeveloperConsole();
    debug_layer.active=true;
    debug_layer.AddWidget(game_console,"console");
    DeveloperConsole::Write("hello world!");
}

void UI::Paint(){
    interface_layer.Paint();
    menu_layer.Paint();
    pause_layer.Paint();
    debug_layer.Paint();
}

void UI::Update(Scene* scene, int frames){
    interface_layer.Update(frames);
    menu_layer.Update(frames);
    pause_layer.Update(frames);
    debug_layer.Update(frames);
}

void UI::Unload(){
    logger::info("unloading ui...\n");
    delete game_console;
}

bool  UI::OnInput(Input::EventID event_type){
    bool handled = false;
    handled = debug_layer.OnInput(event_type);
    if(!handled){handled = interface_layer.OnInput(event_type);}
    if(!handled){handled = pause_layer.OnInput(event_type);}
    if(!handled){handled = menu_layer.OnInput(event_type);}
    return handled;
}
void UI::OnSignal(int signal_id,int metadata_len, byte* metadata){
    bool handled = false;
    handled = debug_layer.OnSignal(signal_id,metadata_len,metadata);
    if(!handled){handled = interface_layer.OnSignal(signal_id,metadata_len,metadata);}
    if(!handled){handled = pause_layer.OnSignal(signal_id,metadata_len,metadata);}
    if(!handled){handled = menu_layer.OnSignal(signal_id,metadata_len,metadata);}
}
void UI::OnResize(int screen_w,int screen_h){
    debug_layer.Resize(screen_w, screen_h);
    interface_layer.Resize(screen_w, screen_h);
    pause_layer.Resize(screen_w, screen_h);
    menu_layer.Resize(screen_w, screen_h);
}
