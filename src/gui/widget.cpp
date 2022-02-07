#include "widget.h"
#include "../log.h"
#include "gui.h"

using namespace UI;

Widget::Widget() : layout(){
    name=nullptr;
    active=true;
    visible=true;
    selectable = nullptr;
}

Widget::Widget(char* widget_name) : layout(), sprites(),rects(),texts(){
    name=cstr::new_copy(widget_name);
    active=true;
    visible=true;
    selectable = nullptr;
    clickable = nullptr;
}

Widget::~Widget(){
    if(name != nullptr){free(name);}
    if(selectable == nullptr){delete selectable;selectable=nullptr;}
    if(clickable == nullptr){delete clickable;clickable=nullptr;}

    for(int i=0;i<sprites.length;i++){
        if(sprites[i] != nullptr){delete sprites[i];sprites.Set(i,nullptr);}
    }
    for(int i=0;i<rects.length;i++){
        if(rects[i] != nullptr){delete rects[i];rects.Set(i,nullptr);}
    }
    for(int i=0;i<texts.length;i++){
        if(texts[i] != nullptr){delete texts[i];texts.Set(i,nullptr);}
    }
}

void Widget::Activate(){
    visible=true;
    active=true;
}

void Widget::Deactivate(){
    visible=false;
    active=false;
}

void Widget::Update(int frames){
    if(!active)return;
}
void Widget::Paint(){
    if(!visible)return;
    for(UI_Rect* rect: rects){
        if(rect == nullptr)continue;
        rect->Draw();
    }
    for(Sprite* sprite: sprites){
        if(sprite == nullptr)continue;
        sprite->Draw();
    }
    for(DrawableText* text: texts){
        if(text == nullptr)continue;
        text->Draw();
    }
    OnPaint();
}

bool Widget::HandleInput(Input::Event event_type){
    if(!active)return false;
    return OnInput(event_type);
}


void Widget::HandleResize(){
    /* TODO: UI font rescaling when you feel scrappy
    for(UI_Text* text: texts){//Rescale fonts
        if(text == nullptr || text->string == nullptr || wstr::len(text->string) == 0)continue;
        text->font = FontManager::RescaleFontID(text->font,(int)(text->default_font_size * GUI::scale_factor));
        text->BuildString();
    }
    */
    OnResize();
}


void Widget::MoveTo(Widget* parent,VerticalOrigin vmode,HorizontalOrigin hmode, point_i offset){
    if(parent != null){layout.MoveTo(&parent->layout,vmode,hmode,offset);}
    
    else{
        Layout fullscreen_layout(UI_WIDTH,UI_HEIGHT);
        layout.MoveTo(&fullscreen_layout,vmode,hmode,offset);
    }
    
    for(Sprite* s:sprites){
        s->x = layout.center.x - (s->width/2);
        s->y = layout.center.y - (s->height/2);
    }
    for(UI_Rect* r:rects){
        r->rect = layout.Rect();
    }
    for(DrawableText* t:texts){
        t->x = layout.center.x - (t->w/2);
        t->y = layout.center.y - (t->h/2);
    }
    OnMove();
}

bool Widget::HandleSignal(EventSignal signal){
    if(!active)return false;
    return OnSignal(signal);
}

bool Widget::IsSelectable(){
    return (selectable != nullptr);
}

void Widget::OnUpdate(){}
bool Widget::OnInput(Input::Event event_type){return false;}
void Widget::OnMove(){}
void Widget::OnPaint(){}
void Widget::OnResize(){}
bool Widget::OnSignal(EventSignal signal){return false;}


void Action_NO_OP(Widget* w){}
void Effect_NO_OP(Widget* w){}
bool Input_NO_OP(Widget* w, Input::Event event_type){return false;}

WidgetSelectInfo::WidgetSelectInfo(){
    locked=false;
    next_up=nullptr;
    next_down=nullptr;
    next_left=nullptr;
    next_right=nullptr;
    onHighlightEffect=Effect_NO_OP;
    onStopHighlightEffect=Effect_NO_OP;
    onSelectedInput=Input_NO_OP;
}

WidgetSelectInfo::~WidgetSelectInfo(){}


WidgetClickInfo::WidgetClickInfo(){
    state=PressedState::Not_Pressed;
    onHoverEffect=Effect_NO_OP;
    onStopHoverEffect=Effect_NO_OP;
    onClickEffect=Effect_NO_OP;
    onClickReleaseEffect=Effect_NO_OP;
    onClickAction=Action_NO_OP;
}

WidgetClickInfo::~WidgetClickInfo(){}


/*************************
 *   Widget Containers   *
 *************************/
/*
WidgetContainer::WidgetContainer(){
    slots=1;
    keys = (char**)calloc(1,sizeof(char*));
    values = (Widget**)calloc(1,sizeof(Widget*));
}
WidgetContainer::~WidgetContainer(){
    Clear();
    free(keys);keys=nullptr;
    free(values);values=nullptr;
}
void WidgetContainer::Clear(){
    for(int i=0;i<slots;i++){
        if(keys[i] != nullptr){free(keys[i]);keys[i]=nullptr;}
        if(values[i] != nullptr){delete (values[i]); values[i] = nullptr;}
    }
    free(keys);keys=nullptr;
    free(values);values=nullptr;
}


Widget* WidgetContainer::Add(Widget* w,char* name,Layout::Offset pos){
    w->layout.offset=pos;
    return Add(w,name);
}
Widget* WidgetContainer::Add(Widget* w, char* name){
    if(w==nullptr || name == nullptr)return nullptr;
    for(int i=0;i<slots;i++){
        if(values[i]==w)return nullptr;
        if(keys[i]==nullptr)continue;
        if(cstr::compare(keys[i],name))return nullptr;//TODO: auto-rename instead of fail
    }
    bool added=false;
    for(int i=0;i< slots;i++){
        if(keys[i]==nullptr){
            keys[i] = cstr::new_copy(name);
            values[i] = w;
            added=true;
        }
    }
    
    if(!added){
        char** double_keys = (char**)calloc(slots*2,sizeof(char*));
        Widget** double_values = (Widget**)calloc(slots*2,sizeof(Widget*));
            memcpy(double_keys,keys,sizeof(char*)*slots);
            memcpy(double_values,values,sizeof(Widget*)*slots);
            free(keys);keys=double_keys;
            free(values);values=double_values;
        
        keys[slots] = cstr::new_copy(name);
        values[slots] = w;
        slots *= 2;
    }


    w->SetName(name);
    w->layout.offset.parent=parent_layout;
    w->layout.Resize();
    return w;
}
Widget* WidgetContainer::Get(char* name){
    for(int i=0;i<slots;i++){
        if(keys[i] == nullptr)continue;
        if(cstr::compare(name,keys[i])){
            return values[i];
        }
    }
    return null;
}
//does not delete removed widget
Widget* WidgetContainer::Remove(char* name){
    Widget* ret = nullptr;
    for(int i=0;i<slots;i++){
        if(keys[i] == nullptr)continue;
        if(cstr::compare(name,keys[i])){
            free(keys[i]); keys[i]=nullptr;
            ret = values[i]; values[i] = nullptr;
            return ret;
        }
    }
    return ret;
}
void WidgetContainer::Destroy(char* name){
    for(int i=0;i<slots;i++){
        if(keys[i] == nullptr)continue;
        if(cstr::compare(name,keys[i])){
            free(keys[i]); keys[i]=nullptr;
            if(values[i] != nullptr){delete(values[i]); values[i] = nullptr;}
        }
    }
}
//Widget* Find(char* pathname);
//Widget* GetAtPosition(int x,int y);


WidgetContainerIterator WidgetContainer::begin(){return {this,next(-1)};}
WidgetContainerIterator WidgetContainer::end(){return {this,slots};}
int WidgetContainer::next(int index){
    int next=index;
    while( ++next < slots && keys[next] == nullptr);
    return next;
}

Widget* WidgetContainerIterator::operator*(){
    if(index < 0){
        return null;
    }
    return (Widget*)parent->values[index];
}

WidgetContainerIterator WidgetContainerIterator::operator++(){
    index = parent->next(index);
    return (*this);
}

bool WidgetContainerIterator::operator==(WidgetContainerIterator& l2){
    return index ==l2.index;
}

bool WidgetContainerIterator::operator!=(WidgetContainerIterator& l2){
    return !(index ==l2.index);
}

*/