#include <game_project_1/gui/menu/character_create_menu.hpp>
#include <game_project_1/gui/widget/standard_widgets.hpp>
#include <game_project_1/gui/widget/choicebar.hpp>
#include <game_project_1/gui/widget/text_entry_bar.hpp>
#include <game_project_1/gui/gui.hpp>
#include <game_project_1/game/game_constants.hpp>
#include <game_project_1/client/client_net_handler.hpp>

using namespace UI;

CharacterCreateMenu* charmenu = nullptr;

void StartCharacterCallback(){
    if(charmenu->player_name != nullptr && wstr::len(charmenu->player_name)!= 0){
        ClientNetHandler::CreatePlayer(charmenu->player_name,charmenu->player_appearance,charmenu->class_id);
    }
}

void RaceChangeCallback(int choice_id, wchar* choice_name){
    charmenu->player_appearance.race = choice_id;
}

void ClassChangeCallback(int choice_id,wchar* choice_name){
    charmenu->class_id=choice_id;
}
              
CharacterCreateMenu::CharacterCreateMenu() : Menu(){id = Menu::CHARACTER_CREATE;}

void CharacterCreateMenu::OnLoad(){
    charmenu = this;
    player_name=nullptr;
    class_id=0;
    player_appearance.race=0;
    player_appearance.style1=0;
    player_appearance.style2=0;
    player_appearance.style3=0;
    player_appearance.color[0]=255;
    player_appearance.color[1]=255;
    player_appearance.color[2]=255;
    player_appearance.color[3]=255;


    AddWidget(new MenuBackground());
    
    FontID loading_font = FontManager::LoadFontFace("SourceSansPro-Regular",32);
    Widget* header_text = new Widget("status_text");
        header_text->layout.width=1024;
        header_text->layout.height=64;
        header_text->texts.Resize(1);
        header_text->texts.Set(0,new DrawableText(L"Build a character:",loading_font));
        header_text->MoveTo(nullptr,Top,Center_H,{0,28});
    AddWidget(header_text);

    TextChoiceBar* race_choice = new TextChoiceBar("race_choice",L"Race");
        race_choice->SetChoiceList(GameConstants::race_names,GameConstants::race_count);
        race_choice->choice_change_callback = RaceChangeCallback;

    TextChoiceBar* class_choice = new TextChoiceBar("class_choice",L"Class");
        class_choice->SetChoiceList(GameConstants::class_names,GameConstants::class_count);
        class_choice->choice_change_callback = ClassChangeCallback;

    TextEntryBar* name_entry = new TextEntryBar("name_entry",L"Enter name:",&player_name);
/*
    ChoiceBar* style_1_choice = new ChoiceBar("style_1_choice",L"Style 1");
        style_1_choice->SetChoiceCount(4);
        style_1_choice->choice_change_callback = Style1ChangeCallback;

    ChoiceBar* style_2_choice = new TextChoiceBar("style_2_choice",L"Style 2");
        style_2_choice->SetChoiceCount(4);
        style_2_choice->choice_change_callback = Style2ChangeCallback;

    ChoiceBar* style_3_choice = new TextChoiceBar("style_3_choice",L"Style 3");
        style_3_choice->SetChoiceCount(4);
        style_3_choice->choice_change_callback = Style3ChangeCallback;

    ColorPicker* color_1_picker = new ColorPicker("color_1_picker");
        color_1_picker->color_change_callback = Color1ChangeCallback;
    */

    //TextBar character_name = new TextBar();

    SimpleButton* create_button = new SimpleButton("join_button",L"JOIN",256,80,{0.3,0.15,1.0,0.8},StartCharacterCallback);

    race_choice->MoveTo(header_text,Below,Left,{0,48});
    class_choice->MoveTo(race_choice,Below,Left,{0,32});
    name_entry->MoveTo(class_choice,Below,Center_H,{0,48});
    create_button->MoveTo(name_entry,Below,Center_H,{0,80});

    race_choice->selectable->next_down = class_choice;
    class_choice->selectable->next_up = race_choice;
    class_choice->selectable->next_down = name_entry;
    name_entry->selectable->next_up = class_choice;
    name_entry->selectable->next_down = create_button;
    create_button->selectable->next_up = name_entry;

    AddWidget(race_choice);
    AddWidget(class_choice);
    AddWidget(name_entry);
    AddWidget(create_button);

    SetSelected(race_choice);
}

void CharacterCreateMenu::OnUnload(){
    charmenu=nullptr;
}
void  CharacterCreateMenu::OnUpdate(int frames){};
void  CharacterCreateMenu::OnPaint(){}

bool  CharacterCreateMenu::OnInput(Input::Event event_type){return false;}
void  CharacterCreateMenu::OnResize(){}
bool  CharacterCreateMenu::OnSignal(EventSignal signal){return false;}