#include <game_project_1/gui/widget/text_entry_bar.hpp>


using namespace UI;

bool TextEntryBarSelectedInputCallback(Widget* w, Input::Event event_type){
    TextEntryBar* entry_bar = (TextEntryBar*)w;
    if(entry_bar->overlay.active){
        return entry_bar->overlay.OnInput(entry_bar,event_type);
    }
    if(event_type == Input::A && Controller::GetButton(Controller::A).IsJustPressed()){
            entry_bar->selectable->locked=true;
            entry_bar->overlay.active=true;
            Input::SetKeyboardTypingOnly(true);
            return true;
    }
    return false;
}

void TextEntryBarOnClickAction(Widget* w){
    TextEntryBar* entry_bar = (TextEntryBar*)w;

    point_i cursor_pos = ScreenToClientSpace(Controller::GetPCCursor());
    rect_i entry_button = {entry_bar->sprites[0]->x,entry_bar->sprites[0]->y,entry_bar->sprites[0]->width,entry_bar->sprites[0]->height};

    if(entry_button.contains(cursor_pos)){
        entry_bar->selectable->locked=true;
        entry_bar->overlay.active=true;
        Input::SetKeyboardTypingOnly(true);
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


wchar* EN_US_TEXTENTRY_LAYER1 = L" zxcvbnm.   asdfghjkl_+!qwertyuiop-=1234567890()";
wchar* EN_US_TEXTENTRY_LAYER2 = L" ZXCVBNM.   ASDFGHJKL_+!QWERTYUIOP-=1234567890()";

TextEntryKeyboardOverlay::TextEntryKeyboardOverlay():
layout(), background(),entry_line(), selector(),back("text_grid_back"),shift1("text_grid_shift1"),end("text_grid_ok"),entry(){
    grid_size=60;

    grid_layers=2;
    current_layer=0;
    grid_rows = 4;
    grid_columns=12;

    selected = {0,1};
    back_pos = {10,0};
    shift1_pos = {0,0};
    end_pos = {11,0};

    background.color= {0.1f,0.1f,0.2f,0.5f};
    entry_line.color= {0.3f,0.3f,0.6f,0.7f};
    selector.color= {1.0f,1.0f,1.0f,0.3f};

    layers = (wchar**)calloc(grid_layers,sizeof(wchar*));
        layers[0] = wstr::new_copy(EN_US_TEXTENTRY_LAYER1);
        layers[1] = wstr::new_copy(EN_US_TEXTENTRY_LAYER2);
        //Looks like
        //"1234567890()"
        //"QWERTYUIOP-="
        //"ASDFGHJKL_+!"
        //"^ZXCVBNM.<E "

    letters = (Glyph*)calloc(grid_rows*grid_columns,sizeof(Glyph));

    for(int i=0;i<grid_rows;i++){
        for(int j=0;j<grid_columns;j++){
            letters[i*grid_columns + j].codepoint=' ';
            letters[i*grid_columns + j].x =  j*grid_size;// + (grid_size-fontface.size)/4.0f;
            letters[i*grid_columns + j].y =  i*grid_size;// + (grid_size-fontface.size)/4.0f;
        }
    }

    fontface = FontManager::LoadFontFace("SourceSansPro-Regular",48);
    entry.font=fontface;
    entry.default_font_size=48;

    LoadLayer();
    OnMove();
}

TextEntryKeyboardOverlay::~TextEntryKeyboardOverlay(){
    free(letters);
    letters=nullptr;

    for(int i=0;i<grid_layers;i++){
        free(layers[i]);
    }
    free(layers);
    layers=nullptr;
}


void TextEntryKeyboardOverlay::LoadLayer(){
    FontManager::SetActiveFont(fontface);
    for(int i=0;i<grid_rows;i++){
        for(int j=0;j<grid_columns;j++){
            wchar character = CharAt(j,i);
            if(character == ' ')continue;
            letters[i*grid_columns + j].Set(character);
            letters[i*grid_columns + j].offset.x += (grid_size - letters[i*grid_columns + j].glyph_texture.width_px)/4;
            letters[i*grid_columns + j].offset.y += (grid_size - letters[i*grid_columns + j].glyph_texture.height_px)/4;
        }
    }
}

void TextEntryKeyboardOverlay::OnSelect(Widget* widget){
    TextEntryBar* text_entry = (TextEntryBar*)widget;
    if(selected == back_pos){
        RemoveChar();
    }
    else if(selected == shift1_pos){
        current_layer++;
        if(current_layer >= grid_layers){current_layer=0;}
        LoadLayer();
    }
    else if(selected == end_pos){
        if(wstr::len(entry.string)> 0){//Validate
            text_entry->selectable->locked=false;
            this->active=false;
            Input::SetKeyboardTypingOnly(false);
            text_entry->UpdateHook();
        }
    }
    else{
        AddSelectedChar();
    }
}

void TextEntryKeyboardOverlay::AddSelectedChar(){
    if(wstr::len(entry.string) < MAX_ENTRY_LENGTH){
        wchar* appended = wstr::append(entry.string,CharAt(selected.x,selected.y));
        entry.SetString(appended);
        free(appended);
    }
    else{
        wchar* replace_last = wstr::new_copy(entry.string);
        replace_last[wstr::len(replace_last)-1]=CharAt(selected.x,selected.y);
        entry.SetString(replace_last);
        free(replace_last);
    }
}

void TextEntryKeyboardOverlay::RemoveChar(){
    if(wstr::len(entry.string) > 1){
        wchar* removed = wstr::substr(entry.string,0,wstr::len(entry.string)-1);
        entry.SetString(removed);
        free(removed);
    }
    else if(wstr::len(entry.string) == 1){
        entry.SetString(nullptr);
    }
}

void TextEntryKeyboardOverlay::OnMove(){
    layout.center = {132 + (UI::UI_WIDTH/2),-16 + (UI::UI_HEIGHT/2)};
    layout.width = 768;
    layout.height= 280;

    background.rect=layout.Rect();
    entry_line.rect=background.rect;
    entry_line.rect.y += background.rect.h + 16;
    entry_line.rect.h = grid_size + 4;

    entry.x = entry_line.rect.x + 2;
    entry.y = entry_line.rect.y + 2;

    point_i origin = layout.LowerLeft() + (point_i){32,32};
    entry_line.center={(float)layout.center.x,(float)layout.center.y +212};

    back.x = origin.x+(back_pos.x*grid_size)+(grid_size/2)-(back.width/2);
    back.y = origin.y+(back_pos.y*grid_size)+(grid_size/2)-(back.height/2);

    shift1.x = origin.x+(shift1_pos.x*grid_size)+(grid_size/2)-(shift1.width/2);
    shift1.y = origin.y+(shift1_pos.y*grid_size)+(grid_size/2)-(shift1.height/2);

    end.x = origin.x+(end_pos.x*grid_size)+(grid_size/2)-(end.width/2);
    end.y = origin.y+(end_pos.y*grid_size)+(grid_size/2)-(end.height/2);
    UpdateSelection();
}

void TextEntryKeyboardOverlay::UpdateSelection(){
    point_i origin = layout.LowerLeft() + (point_i){32,32};
    selector.rect = {   origin.x+ selected.x*grid_size,
                        origin.y+ selected.y*grid_size,grid_size,grid_size};
}

void TextEntryKeyboardOverlay::Draw(){
    if(!active)return;
    point_i origin = layout.LowerLeft() + (point_i){32,32};
    background.Draw();
    selector.Draw();
    entry_line.Draw();
    for(int i=0;i<grid_rows;i++){
        for(int j=0;j<grid_columns;j++){
            letters[i*grid_columns + j].Draw(origin);
        }
    }
    shift1.Draw();
    back.Draw();
    end.Draw();
    entry.Draw();
}

wchar TextEntryKeyboardOverlay::CharAt(int x,int y){
    return layers[current_layer][y*grid_columns + x];    
}

point_i TextEntryKeyboardOverlay::CharLocation(int codepoint){
    point_i ret = {-1,-1};
    for(int i=0;i<grid_rows;i++){
        for(int j=0;j<grid_columns;j++){
            for(int l=0;l<grid_layers;l++){
                if(codepoint == layers[l][i*grid_columns+j]){
                    if(current_layer != l){current_layer = l; LoadLayer();}
                    ret = {j,i};
                }
            }
        }
    }
    return ret;
}

bool TextEntryKeyboardOverlay::OnInput(Widget* w, Input::Event event_type){
    if(!active)return false;
    TextEntryBar* text_entry = (TextEntryBar*)w;
    bool handled =false;
    switch(event_type){
        case Input::Event::PC_Text:{
            wchar* text = Controller::GetTextInput();
            int text_len = wstr::len(text);
            for(int i=0;i<text_len;i++){
                point_i codepoint_loc = CharLocation(text[i]);
                if(codepoint_loc.x >= 0){
                    selected = codepoint_loc;
                    UpdateSelection();
                    AddSelectedChar();
                }
                else if(text[i] == '\b'){
                    RemoveChar();
                }
            }
            handled= true;
            break;
        }
        case Input::Event::PC_Cursor:{
            point_i letter_origin = layout.LowerLeft() + (point_i){32,32};
            point_i cursor_letter_index = UI::ScreenToClientSpace(Controller::GetPCCursor()) - letter_origin;
            cursor_letter_index.x /= grid_size;
            cursor_letter_index.y /= grid_size;

            if(cursor_letter_index.x >= 0 && cursor_letter_index.x < grid_columns &&
                cursor_letter_index.y >= 0 && cursor_letter_index.y < grid_rows){

                selected = cursor_letter_index;
                UpdateSelection();
            }
            handled= true;
            break;
        }
        case Input::Event::PC_LClick:{
            point_i letter_origin = layout.LowerLeft() + (point_i){32,32};
            point_i cursor_letter_index = UI::ScreenToClientSpace(Controller::GetPCCursor()) - letter_origin;
            cursor_letter_index.x /= grid_size;
            cursor_letter_index.y /= grid_size;

            if(Controller::GetPCLeftMouse().IsJustPressed() &&
                cursor_letter_index.x >= 0 && cursor_letter_index.x < grid_columns &&
                cursor_letter_index.y >= 0 && cursor_letter_index.y < grid_rows){

                OnSelect(text_entry);
            }
            handled= true;
            break;
        }
        case Input::Event::PC_Escape:{
            text_entry->selectable->locked=false;
            text_entry->UpdateHook();
            Input::SetKeyboardTypingOnly(false);
            this->active=false;
            handled= true;
            break;
        }
        case Input::Event::PC_Return:{
            if(!(selected == end_pos)){selected = end_pos;}
            else{OnSelect(text_entry);}
            break;
        }
        case Input::Move:{
            Controller::Axis axis = Controller::GetAxis(Controller::Move);
            if(axis.IsJustUp()){
                if(selected.y < grid_rows-1){
                    selected.y++;
                    UpdateSelection();
                }
            }
            if(axis.IsJustDown()){
                if(selected.y >0){
                    selected.y--;
                    UpdateSelection();
                }
            }
            if(axis.IsJustLeft()){
                if(selected.x >0){
                    selected.x--;
                    UpdateSelection();
                }
            }
            if(axis.IsJustRight()){
                if(selected.x < grid_columns-1){
                    selected.x++;
                    UpdateSelection();
                }
            }
            handled= true;
            break;
        }
        case Input::A:{
            if(Controller::GetButton(Controller::A).IsJustPressed()){
                OnSelect(text_entry);
            }
            handled= true;
            break;
        }
        case Input::B:{
            if(Controller::GetButton(Controller::B).IsJustPressed()){
                RemoveChar();
            }
            handled= true;
            break;
        }
        case Input::Menu:{
            if(Controller::GetButton(Controller::Menu).IsJustPressed()){
                selected = end_pos;
                UpdateSelection();
            }
            handled= true;
            break;
        }
        default:{break;}
    }
    return handled;
}

TextEntryBar::TextEntryBar(char* label,wchar* text,wchar** hook):overlay(){
    overlay.active=false;
    string_hook=hook;
    if(string_hook != nullptr){(*string_hook)=nullptr;}
    sprites.Resize(3);
        sprites.Set(0,new Sprite("text_bar"));
        sprites.Set(1,new Sprite("text_bar_entry_button"));

    texts.Resize(1);
        texts.Set(0,new DrawableText(text,FontManager::LoadFontFace("SourceSansPro-Regular",20)));

    clickable = new WidgetClickInfo();
        clickable->onClickAction = TextEntryBarOnClickAction;
        
    layout.width=sprites[0]->width;
    layout.height=sprites[0]->height;

    selectable = new WidgetSelectInfo();
        selectable->onHighlightEffect= TextEntryBarHighlightEffect;
        selectable->onStopHighlightEffect=TextEntryBarUnhighlightEffect;
        selectable->onSelectedInput = TextEntryBarSelectedInputCallback;
}
TextEntryBar::~TextEntryBar(){}
void TextEntryBar::OnMove(){
    sprites[1]->x += 108;
    overlay.layout.center = {UI::UI_WIDTH/2,UI::UI_HEIGHT/2};
    overlay.layout.width = UI::UI_WIDTH;
    overlay.layout.height = UI::UI_HEIGHT;
    overlay.OnMove();
}
void TextEntryBar::OnPaint(){
    if(selectable->locked){
        overlay.Draw();
    }

}


void TextEntryBar::UpdateHook(){
    if(string_hook == nullptr)return;

    wchar* string = (*string_hook);
    if(string != nullptr){free(string);}

    (*string_hook) = wstr::new_copy(overlay.entry.string);
    texts[0]->SetString(overlay.entry.string);
}