#ifndef UI_LAYER_H
#define UI_LAYER_H

#include "../input.h"
#include "../struct/list.h"
#include "layout.h"
#include "widget.h"

class UIWindow{
    public:
    bool    active;
    bool    visible;
    Layout  layout;
    int     type_id;

    List<UIWindow>  sub_windows;
    WidgetContainer widgets;
    
    UIWindow();
    UIWindow(UIWindow* parent);
    
    void Clear();

    void Open();
        virtual void OnOpen(){}
    void Close();
        virtual void OnClose(){};
    void Update(int frames);
        virtual void OnUpdate(int frames){};
    void Paint();
        virtual void OnPaint(){}
    bool HandleInput(Input::Event event_type);
        virtual bool OnInput(Input::Event event_type){return false;}
    void HandleResize();
        virtual void OnResize(){}
    bool HandleSignal(int signal_id,int metadata_len, byte* metadata);
        virtual bool OnSignal(int signal_id,int metadata_len, byte* metadata){return false;}
};


#endif