#include <game_project_1/gui/menu/ingame_menu.hpp>
#include <game_project_1/input.hpp>

using namespace UI;

IngameMenu::IngameMenu(): Menu(){id = Menu::INGAME;current_dialogue_id=0;}

void IngameMenu::OnLoad(){

}

bool IngameMenu::OnInput(Input::Event event_type){
    if(current_dialogue_id > 0){
        if(
            (event_type == Input::Event::A && Controller::GetButton(Controller::A).IsJustReleased()) ||
            (event_type == Input::Event::B && Controller::GetButton(Controller::B).IsJustReleased()) ){
            AdvanceDialogue();
        }
            return true;
    }
    return false;
}

//TODO: the real system for this
wchar* GetTranslatedDialogueByID(int dialogue_id){
    return L"Test dialogue";
}

void IngameMenu::StartDialogueBox(int dialogue_id){
    if(current_dialogue_id > 0){return;}
    wchar* dialogue_string = GetTranslatedDialogueByID(dialogue_id);
    current_dialogue_id=dialogue_id;
    //TODO- add widget and text one frame at a time, frame advance, all that jazz
    logger::infoW(dialogue_string);
}

void IngameMenu::AdvanceDialogue(){
    current_dialogue_id= 0;//TODO
    logger::info("\n");
}