#ifndef UI_LAYER_H
#define UI_LAYER_H

#include "../input.h"
#include "../struct/list.h"
#include "layout.h"
#include "widget.h"

namespace UI{
    //What does this represent?
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
    };
}

#endif