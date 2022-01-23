#include "text_entry_bar.h"


/*
   wchar** characters[][] = {
        "1234567890()",
        "QWERTYUIOP-=",
        "ASDFGHJKL_+!",
        " ZXCVBNM.   "
    };

*/

using namespace UI;

bool TextEntryBarSelectedInputCallback(Widget* w, Input::Event event_type){
    TextEntryBar* entry_bar = (TextEntryBar*)w;
    if(event_type == Input::A){
        if(Controller::GetButton(Controller::A).IsJustPressed()){
            entry_bar->selectable->locked=true;
        }
    }
    return false;
}

void TextEntryBarOnClickAction(Widget* w){
    TextEntryBar* entry_bar = (TextEntryBar*)w;

    point_i cursor_pos = ScreenToClientSpace(Controller::GetPCCursor());
    rect_i entry_button = {entry_bar->sprites[1]->x,entry_bar->sprites[1]->y,entry_bar->sprites[1]->width,entry_bar->sprites[1]->height};

    if(entry_button.contains(cursor_pos)){
        entry_bar->selectable->locked=true;
    }
}

void TextEntryBarHighlightEffect(Widget* w){
    TextEntryBar* entry_bar = (TextEntryBar*)w;
    vec4 new_color = entry_bar->sprites[0]->color;
    new_color.x += 0.2f;
    new_color.y += 0.2f;
    new_color.z += 0.2f;
    new_color.w += 0.2f;
    entry_bar->sprites[0]->color =new_color;
}
void TextEntryBarUnhighlightEffect(Widget* w){
    TextEntryBar* entry_bar = (TextEntryBar*)w;
    vec4 new_color = entry_bar->sprites[0]->color;
    new_color.x -= 0.2f;
    new_color.y -= 0.2f;
    new_color.z -= 0.2f;
    new_color.w -= 0.2f;
    entry_bar->sprites[0]->color =new_color;
}


TextEntryKeyboardOverlay::TextEntryKeyboardOverlay(): background(), selector(),finished("name_plaque"){
    grid_rows = 4;
    grid_columns=12;

    wchar** characters = (wchar**)calloc(4,sizeof(wchar*));
        characters[0] = wstr::new_copy(L"1234567890()");
        characters[1] = wstr::new_copy(L"QWERTYUIOP-=");
        characters[2] = wstr::new_copy(L"ASDFGHJKL_+!");
        characters[3] = wstr::new_copy(L" ZXCVBNM.   ");

    letters = (Glyph*)calloc(grid_rows*grid_columns,sizeof(Glyph));
    for(int i=0;i<grid_columns;i++){
        for(int j=0;j<grid_rows;j++){
            letters[j*grid_columns + i].x =  characters[j][i];
        }
    }
}
TextEntryKeyboardOverlay::~TextEntryKeyboardOverlay(){
    for(int i=0;i<grid_rows;i++){
        free(characters[i]);
    }
    free(characters);
    free(letters);
    characters=nullptr;
}
void TextEntryKeyboardOverlay::Draw(){
    background.Draw();
    selector.Draw();
    for(int i=0;i<grid_columns;i++){
        for(int j=0;j<grid_rows;j++){
            //Draw()
            //letters[j*grid_columns + i].x =  characters[j][i];
        }
    }
    finished.Draw();
}
void TextEntryKeyboardOverlay::OnInput(Widget* w, Input::Event event_type){


}

TextEntryBar::TextEntryBar(char* label,wchar* text){
    sprites.Resize(3);
        sprites.Set(0,new Sprite("text_bar"));
        sprites.Set(1,new Sprite("text_bar_entry_button"));

    texts.Resize(1);
        texts.Set(0,new UI_Text(text,FontManager::LoadFontFace("SourceSansPro-Regular",20)));

    clickable = new WidgetClickInfo();
        clickable->onClickAction = TextEntryBarOnClickAction;

    selectable = new WidgetSelectInfo();
        selectable->onHighlightEffect= TextEntryBarHighlightEffect;
        selectable->onStopHighlightEffect=TextEntryBarUnhighlightEffect;
        selectable->onSelectedInput = TextEntryBarSelectedInputCallback;

}
TextEntryBar::~TextEntryBar(){}
void TextEntryBar::OnMove(){
    sprites[1]->x += 108;
}
void TextEntryBar::OnPaint(){
    if(selectable->locked){
        overlay.Draw();
    }

}