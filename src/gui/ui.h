#ifndef MYGAME_UI_H
#define MYGAME_UI_H

#include "../game/scene.h"
#include "../gfx/renderer.h"
#include "../input.h"
#include "window.h"

class UI{
    public:
    static const int MAIN_MENU=1;
    static const int LOADING=2;
    static const int INGAME=3;
    //static const int OPTIONS=4;
    const static int LAYER_STACK_MAX=4;

    UI();
    void Load();
    void ClearWindows();
    //void AddWindow(UIWindow* p_window);
    //void RemoveWindow();
    //void ActivateWindow();
    //void DeactivateWindow();
    //void ShowWindow(); 
    //void HideWindow(); 
    UIWindow* OpenWindow(int window_id);
    void CloseWindow();

    void Unload();
    Layout      fullscreen_layout;
    StringMap   debug_widgets;

    UIWindow*   current_screen;
    //int       screen_stack_max=8;

    void Paint();
    void Update(Scene* scene, int frames);

    //UI interaction events
    bool OnInput(Input::EventID event_type);
    void OnSignal(int signal_id,int metadata_len, byte* metadata);
    void OnResize(int screen_w,int screen_h);
};




#endif