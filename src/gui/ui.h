#ifndef MYGAME_UI_H
#define MYGAME_UI_H

#include "../game/scene.h"
#include "../gfx/renderer.h"
#include "../input.h"
#include "ui_layer.h"

class UI{
    public:
    const int MAX_LAYERS =8;

    UI();
    void Load();
    void Unload();

    UILayer debug_layer;
    UILayer interface_layer;
    UILayer pause_layer;
    UILayer menu_layer;

    void Paint();
    void Update(Scene* scene, int frames);

    //UI interaction events
    bool OnInput(Input::EventID event_type);
    void OnSignal(int signal_id,int metadata_len, byte* metadata);
    void OnResize(int screen_w,int screen_h);
};




#endif