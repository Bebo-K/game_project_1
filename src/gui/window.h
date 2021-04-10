#ifndef UI_LAYER_H
#define UI_LAYER_H

#include "../input.h"
#include "../structs/list.h"
#include "layout.h"
#include "widget.h"

class UIWindow{
    public:
    bool    active;
    bool    visible;
    Layout  layout;
    int     type_id;

    List<UIWindow>  sub_windows;
    StringMap   widgets;
    
    UIWindow();
    UIWindow(UIWindow* parent);

    Widget* AddWidget(Widget* w,char* name);
    Widget* AddWidget(Widget* w,char* name,LayoutOffset pos);
    Widget* GetWidget(char* name);
    Widget* RemoveWidget(char* name);
    //Widget* GetWidgetAtPos(int x,int y);
    void Clear();

    void Open();
        virtual void OnOpen(){}
    void Close();
        virtual void OnClose(){};
    void Update(int frames);
        virtual void OnUpdate(int frames){};
    void Paint();
        virtual void OnPaint(){}
    bool HandleInput(Input::EventID event_type);
        virtual bool OnInput(Input::EventID event_type){return false;}
    void HandleResize();
        virtual void OnResize(){}
    bool HandleSignal(int signal_id,int metadata_len, byte* metadata);
        virtual bool OnSignal(int signal_id,int metadata_len, byte* metadata){return false;}
};


#endif