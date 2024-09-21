#ifndef DEV_LAYER_HPP
#define DEV_LAYER_HPP

#include <game_project_1/gui/menu.hpp>
#include <game_project_1/gui/widget.hpp>


class DeveloperLayer: public UI::Menu {
    public:

    static DeveloperLayer* instance;

    DeveloperLayer();
    ~DeveloperLayer();
    UI::Widget* console;
    UI::Widget* footer_label;

    void OnLoad();
    void OnUnload();
    void OnUpdate(Timestep delta);
    bool OnInput(Input::Event event_type);
    bool OnSignal(EventSignal signal);

    static void SetLabelText(int label_id,wchar* text,...);

};



#endif