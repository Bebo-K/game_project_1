#include "choicebar.h"

#include "../../gfx/font_manager.h"


using namespace UI;

FontID TextChoiceBar::choice_font={-1,18};

bool ChoiceBarSelectedInputCallback(Widget* w, Input::Event event_type){
    TextChoiceBar* choice = (TextChoiceBar*)w;
    if(event_type == Input::Move){
        Controller::Axis axis = Controller::GetAxis(Controller::Move);
        if(axis.IsJustLeft()){
            choice->PrevChoice();
            return true;
        }
        if(axis.IsJustRight()){
            choice->NextChoice();
            return true;
        }
    }
    return false;
}

void ChoiceBarOnClickAction(Widget* w){
    TextChoiceBar* choice = (TextChoiceBar*)w;
    point_i cursor_pos = ScreenToClientSpace(Controller::GetPCCursor());
    rect_i next_button = {choice->sprites[1]->x,choice->sprites[1]->y,choice->sprites[1]->width,choice->sprites[1]->height};
    rect_i prev_button = {choice->sprites[2]->x,choice->sprites[2]->y,choice->sprites[2]->width,choice->sprites[2]->height};

    if(prev_button.contains(cursor_pos)){
        choice->PrevChoice();
    }
    else if(next_button.contains(cursor_pos)){
        choice->NextChoice();
    }
}


void TextChoiceBarHighlightEffect(Widget* w){
    TextChoiceBar* choicebar = (TextChoiceBar*)w;
    vec4 new_color = choicebar->sprites[0]->color;
    new_color.x += 0.2f;
    new_color.y += 0.2f;
    new_color.z += 0.2f;
    new_color.w += 0.2f;
    choicebar->sprites[0]->color =new_color;
}
void TextChoiceBarUnhighlightEffect(Widget* w){
    TextChoiceBar* choicebar = (TextChoiceBar*)w;
    vec4 new_color = choicebar->sprites[0]->color;
    new_color.x -= 0.2f;
    new_color.y -= 0.2f;
    new_color.z -= 0.2f;
    new_color.w -= 0.2f;
    choicebar->sprites[0]->color =new_color;
}

void ChoiceChangeNO_OP(int choice_id, wchar* choice_name){}

TextChoiceBar::TextChoiceBar(char* name,wchar* label):Widget(name){
    choice_change_callback = ChoiceChangeNO_OP;
    choices=nullptr;
    max_choices=0;
    if(choice_font.family_id < 0){choice_font = FontManager::LoadFontFace("SourceSansPro-Regular",20);}
   
    sprites.Resize(3);
        sprites.Set(0,new Sprite("text_select_bar"));
        sprites.Set(1,new Sprite("select_bar_button_next"));
        sprites.Set(2,new Sprite("select_bar_button_next"));
        sprites[2]->x_flip=true;

    layout.width=sprites[0]->width;
    layout.height=sprites[0]->height;

    texts.Resize(2);
        texts.Set(0,new DrawableText(label,choice_font));
        texts.Set(1,new DrawableText(L"",choice_font));

    clickable = new WidgetClickInfo();
        clickable->onClickAction = ChoiceBarOnClickAction;

    selectable = new WidgetSelectInfo();
        selectable->onHighlightEffect= TextChoiceBarHighlightEffect;
        selectable->onStopHighlightEffect=TextChoiceBarUnhighlightEffect;
        selectable->onSelectedInput = ChoiceBarSelectedInputCallback;
}

TextChoiceBar::~TextChoiceBar(){
    if(choices != nullptr){
        for(int i=0;i<max_choices;i++){free(choices[i]);}
        free(choices);
    }
}


void TextChoiceBar::OnMove(){
    sprites[1]->x += 108;
    sprites[1]->y -= 16;
    sprites[2]->x -= 108;
    sprites[2]->y -= 16;

    texts[0]->y += 12;
    texts[1]->y -= 16;
}

void TextChoiceBar::SetChoiceList(wchar** choice_list,int choice_count){
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



void TextChoiceBar::NextChoice(){
    if(max_choices <= 0)return;
    choice_id++;
    if(choice_id >= max_choices){choice_id =0;}
    texts[1]->SetString(choices[choice_id]);
    choice_change_callback(choice_id,choices[choice_id]);
}

void TextChoiceBar::PrevChoice(){
    if(max_choices <= 0)return;
    choice_id--;
    if(choice_id < 0){choice_id =max_choices-1;}
    texts[1]->SetString(choices[choice_id]);
    choice_change_callback(choice_id,choices[choice_id]);
}