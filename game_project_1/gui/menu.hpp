#ifndef UI_MENU_H
#define UI_MENU_H

#include <game_project_1/input.hpp>
#include <game_project_1/types/primitives.hpp>
#include <game_project_1/types/timestep.hpp>
#include <game_project_1/types/list.hpp>
#include <game_project_1/gui/layout.hpp>
#include <game_project_1/gui/widget.hpp>

namespace UI{
    typedef int MenuID;
    //A collection of UI elements managing a single gameplay element. More than one can be displayed at a time.
    class Menu{
        public:
        const static MenuID DEVMODE=1,MAIN=2,OPTIONS=3,LOADING=4,ERROR_MENU=5,INGAME=6,CHARACTER_CREATE=7;
        MenuID       id;
        bool         active;
        bool         visible;
        List<Widget> widgets;
        Widget*      selected;

        Menu();
        virtual ~Menu();

        //Open/Close are triggered when a menu transitions from one to another.
        void Open();
        void Close();
        //void TransitionTo(Menu* m2); TODO: special transitions?
        //Load/Unload happen for all menus at once at client load/reload
        void Load();
        void Unload();
        void Update(Timestep delta);
        void Paint();
        bool HandleInput(Input::Event event_type);
        bool HandleSelectionInput(Input::Event event_type);
        bool HandleClickInput(Input::Event event_type);
        void HandleResize();
        bool HandleSignal(EventSignal signal);

        virtual void OnLoad();
        virtual void OnUnload();
        virtual void OnOpen();
        virtual void OnClose();
        virtual void OnUpdate(Timestep delta);
        virtual void OnPaint();
        virtual bool OnInput(Input::Event event_type);
        virtual void OnResize();
        virtual bool OnSignal(EventSignal signal);

        void AddWidget(Widget* w);
        void RemoveWidget(Widget* w);
        Widget* GetWidget(char* name);

        void SetSelected(Widget* w);
    };
}

#endif