#ifndef UI_MENU_H
#define UI_MENU_H

#include "../input.h"
#include "../struct/data_types.h"
#include "../struct/list.h"
#include "ui_types.h"
#include "widget.h"

namespace UI{
    typedef int MenuID;
    //A collection of UI elements managing a single gameplay element. More than one can be displayed at a time.
    class Menu{
        public:
        const static MenuID MAIN=1,OPTIONS=2,LOADING=3,ERROR_MENU=4,INGAME=5;
        MenuID       id;
        bool         active;
        bool         visible;
        Layout       layout;
        List<Widget> widgets;

        Menu(Layout* parent);
        virtual ~Menu();

        //Open/Close are triggered when a menu transitions from one to another.
        void Open();
        void Close();
        //void TransitionTo(Menu* m2); TODO: special transitions?
        //Load/Unload happen for all menus at once at client load/reload
        void Load();
        void Unload();
        void Update(int frames);
        void Paint();
        bool HandleInput(Input::Event event_type);
        void HandleResize();
        bool HandleSignal(EventSignal signal);

        virtual void OnLoad();
        virtual void OnUnload();
        virtual void OnOpen();
        virtual void OnClose();
        virtual void OnUpdate(int frames);
        virtual void OnPaint();
        virtual bool OnInput(Input::Event event_type);
        virtual void OnResize();
        virtual bool OnSignal(EventSignal signal);

        void AddWidget(Widget* w);
        void RemoveWidget(Widget* w);
    };
}

#endif