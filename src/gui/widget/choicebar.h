#ifndef CHOICEBAR_H
#define CHOICEBAR_H

#include "../widget.h"

class TextChoiceBar: public  UI::Widget{
    private:
        static FontID choice_font;
    public:
        int choice_id;
        wchar** choices;
        int max_choices;
        
        void (*choice_change_callback)(int new_choice,wchar* choice_name);





    TextChoiceBar(char* label,wchar* text);
    ~TextChoiceBar();
    void OnMove();

    void SetChoiceList(wchar** choice_list,int choice_count); 
    void NextChoice();
    void PrevChoice();
};









#endif