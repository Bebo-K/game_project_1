#include "choicebar.h"

#include "../../gfx/font_manager.h"
#include "component_text.h"
#include "component_sprite.h"



ChoiceBar::ChoiceBar(char* name,wchar* label):Widget(name){
    choices=nullptr;
    max_choices=0;

    FontID simple_button_font = FontManager::LoadFontFace("SourceSansPro-Regular",20);
    layout.SetSize(256,64);

    SpriteComponent* sprite = new SpriteComponent("select_bar");

    TextComponent* text = new TextComponent(label,simple_button_font);

    ClickableComponent* clickable = new ClickableComponent();
    clickable->onClickAction = callback;

    SelectableComponent* selectable = new SelectableComponent();
        selectable->onSelectAction = callback;


    components.Add(text);
    components.Add(clickable);
    components.Add(selectable);
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



}