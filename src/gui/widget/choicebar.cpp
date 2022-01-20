#include "choicebar.h"

#include "../../gfx/font_manager.h"


using namespace UI;

bool ChoiceBarSelectedInputCallback(Widget* w, Input::Event event_type){
    ChoiceBar* choice = (ChoiceBar*)w;
    if(event_type == Input::Move){
        Controller::Axis axis = Controller::GetAxis(Controller::Move);
        if(axis.IsLeft()){
            choice->PrevChoice();
            return true;
        }
        if(axis.IsRight()){
            choice->NextChoice();
            return true;
        }
    }
    return false;
}

void ChoiceBarOnClickAction(Widget* w){
    ChoiceBar* choice = (ChoiceBar*)w;
    point_i cursor_pos = Controller::GetPCCursor();
    rect_i prev_button = {choice->sprites[1]->x,choice->sprites[1]->y,choice->sprites[1]->width,choice->sprites[1]->height};
    rect_i next_button = {choice->sprites[2]->x,choice->sprites[2]->y,choice->sprites[2]->width,choice->sprites[2]->height};

    if(prev_button.contains(cursor_pos)){
        choice->PrevChoice();
    }
    else if(next_button.contains(cursor_pos)){
        choice->NextChoice();
    }
}

ChoiceBar::ChoiceBar(char* name,wchar* label):Widget(name){
    choices=nullptr;
    max_choices=0;

    FontID choice_font = FontManager::LoadFontFace("SourceSansPro-Regular",20);
    layout.SetSize(256,64);

    sprites.Resize(3);
        sprites.Set(0,new Sprite(TextureManager::Get("select_bar")));
        sprites.Set(1,new Sprite(TextureManager::Get("next_button")));
        sprites.Set(2,new Sprite(TextureManager::Get("next_button")));
        sprites[2]->x_flip=true;

        //TODO: position sprites
    

    texts.Resize(2);
        texts.Set(0,new UI_Text(label,choice_font));
        texts.Set(0,new UI_Text(L"",choice_font));

    clickable = new WidgetClickInfo();
        clickable->onClickAction = ChoiceBarOnClickAction;

    selectable = new WidgetSelectInfo();
        selectable->onSelectedInput = ChoiceBarSelectedInputCallback;
}

ChoiceBar::~ChoiceBar(){
    if(choices != nullptr){
        for(int i=0;i<max_choices;i++){free(choices[i]);}
        free(choices);
    }
}

void ChoiceBar::SetChoiceList(wchar** choice_list,int choice_count){
    max_choices=choice_count;
    choices = (wchar**)calloc(max_choices,sizeof(wchar*));
    for(int i=0;i<max_choices;i++){
        choices[i] = wstr::new_copy(choice_list[i]);
    }
    if(max_choices > 0){
        choice_id=0;
        texts[1]->SetString(choices[choice_id]);
    }
}



void ChoiceBar::NextChoice(){
    if(max_choices <= 0)return;
    choice_id++;
    if(choice_id >= max_choices){choice_id =0;}
    texts[1]->SetString(choices[choice_id]);
}

void ChoiceBar::PrevChoice(){
    if(max_choices <= 0)return;
    choice_id--;
    if(choice_id < 0){choice_id =max_choices;}
    texts[1]->SetString(choices[choice_id]);
}