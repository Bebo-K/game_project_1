#include <game_project_1/gui/menu/developer_layer.hpp>
#include <game_project_1/gui/widget/standard_widgets.hpp>
#include <game_project_1/gui/gui.hpp>
#include <game_project_1/gui/widget/dev_console.hpp>

using namespace UI;
DeveloperLayer *DeveloperLayer::instance = null;

DeveloperLayer::DeveloperLayer() : Menu()
{
    id = Menu::DEVMODE;
    instance = this;
}

DeveloperLayer::~DeveloperLayer()
{
    if (instance == this)
    {
        instance = null;
    }
}

void DeveloperLayer::OnLoad()
{
    console = new DeveloperConsole();
    AddWidget(console);

    console->MoveTo(nullptr, Top, Left, {2, 2});
    DeveloperConsole::Write(L"Hello World! GUI Initialized.");

    FontID footer_font = FontManager::LoadFontFace("SourceSansPro-Regular", 24);
    footer_label = new Widget("footer_label");
    footer_label->layout.width = UI_WIDTH;
    footer_label->layout.height = 72;
    footer_label->texts.Resize(1);
    footer_label->texts.Set(0, new DrawableText(L"bottom text", footer_font));
    AddWidget(footer_label);
    footer_label->MoveTo(nullptr, Bottom, Center_H, {0, 0});
}

void DeveloperLayer::OnUnload() {}

void DeveloperLayer::OnUpdate(Timestep delta)
{
}

bool DeveloperLayer::OnInput(Input::Event event_type)
{
    Controller::Button toggle = Controller::GetToggleConsole();
    if (event_type == Input::Event::ToggleConsole && toggle.IsJustPressed())
    {
        this->active = false;
        this->visible = false;
        return true;
    }
    return false;
}

bool DeveloperLayer::OnSignal(EventSignal signal)
{
}

void DeveloperLayer::SetLabelText(int label_id, wchar *text, ...)
{//FIXME: label_id
    if (instance == null)
        return;

    va_list args;
    va_start(args, text);
    wchar *buffer = new wchar[128];
    vswprintf(buffer, 128, text, args);
    instance->footer_label->texts[label_id]->SetString(buffer);
    delete buffer;
    va_end(args);
}