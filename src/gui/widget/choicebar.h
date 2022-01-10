#ifndef CHOICEBAR_H
#define CHOICEBAR_H

#include "../widget.h"
#include "component_clickable.h"
#include "component_selectable.h"

class ChoiceBar: public  UI::Widget{

    public:
        int choice_id;
        wchar** choices;
        int max_choices;


    ChoiceBar(char* label,wchar* text);
    ~ChoiceBar();

    void SetChoiceList(wchar** choice_list,int choice_count); 
    SelectableComponent* GetSelectionInfo();
    ClickableComponent* GetClickInfo();

};









#endif