#ifndef INGAME_MENU_H
#define INGAME_MENU_H

#include <game_project_1/gui/menu.hpp>


class IngameMenu: public UI::Menu{
    public:
    int current_dialogue_id;

    IngameMenu();
    
    void OnLoad();
    bool OnInput(Input::Event event_type);

    void StartDialogueBox(int dialogue_id);
    void AdvanceDialogue();
};






#endif