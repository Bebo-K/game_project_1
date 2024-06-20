#ifndef PANEL_H
#define PANEL_H

#include <game_project_1/gui/widget.hpp>
#include <game_project_1/gui/layout.hpp>
#include <game_project_1/types/list.hpp>

namespace UI{

//A collection of Widgets handled as a unit. Essentially a stripped down version of Menu.
    class Panel{
        public:
        bool         active;
        bool         visible;
        Layout       layout;
        List<Widget> widgets;

        Panel(int w,int h);
        ~Panel();

        void Show();
        void Hide();
        void Update(Timestep delta);
        void Paint();
        bool OnInput(Input::Event event_type);
        bool OnSignal(EventSignal signal);

        void AddWidget(Widget* w);
        void RemoveWidget(Widget* w);
    };

}


#endif