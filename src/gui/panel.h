#ifndef PANEL_H
#define PANEL_H

#include "widget.h"
#include "ui_types.h"
#include "../struct/list.h"

namespace UI{

//A collection of Widgets handled as a unit. Essentially a stripped down version of Menu.
    class Panel{
        public:
        bool         active;
        bool         visible;
        Layout       layout;
        List<Widget> widgets;

        Panel(Layout* parent);
        ~Panel();

        void Show();
        void Hide();
        void Update(int frames);
        void Paint();
        bool OnInput(Input::Event event_type);
        void OnResize();
        bool OnSignal(Signal signal);

        void AddWidget(Widget* w);
        void RemoveWidget(Widget* w);
    };

}


#endif