#ifndef UI_MENU_H
#define UI_MENU_H

#include "../input.h"
#include "../struct/list.h"
#include "layout.h"
#include "widget.h"

namespace UI{
    //A collection of UI elements managing a single gameplay element. More than one can be displayed at a time.
    class Menu{
        public:
        bool        active;
        bool        visible;
        Layout      layout;
        int         id;

        Menu();
        virtual ~Menu();
        void Open(Layout* menu_area);
        void Close();
        void Update(int frames);
        void Paint();
        bool HandleInput(Input::Event event_type);
        void HandleResize();
        bool HandleSignal(int signal_id,int metadata_len, byte* metadata);

        virtual void OnOpen();
        virtual void OnClose();
        virtual void OnUpdate(int frames);
        virtual void OnPaint();
        virtual bool OnInput(Input::Event event_type);
        virtual void OnResize();
        virtual bool OnSignal(int signal_id,int metadata_len, byte* metadata);
    };
}

#endif