#include "widget.h"
#include "../log.h"

using namespace UI;

Widget::Widget() : layout(), components(){
    name=nullptr;
    active=true;
    visible=true;
    layout.x_pos_scale=Relative;
    layout.y_pos_scale=Relative;
}

Widget::Widget(char* widget_name) : layout(), components(){
    name=cstr::new_copy(widget_name);
    active=true;
    visible=true;
    layout.x_pos_scale=Relative;
    layout.y_pos_scale=Relative;
}

Widget::~Widget(){
    if(name != nullptr){free(name);}
    for(WidgetComponent* c: components){delete c;}
}

WidgetComponent::~WidgetComponent(){}

void Widget::Activate(){
    visible=true;
    active=true;
   for(WidgetComponent* c: components){c->OnActivate(this);}
}

void Widget::Deactivate(){
    visible=false;
    active=false;
    for(WidgetComponent* c: components){c->OnDeactivate(this);}
}

void Widget::Update(int frames){
    if(!active)return;
    for(WidgetComponent* c: components){c->OnUpdate(this,frames);}
}
void Widget::Paint(){
    if(!visible)return;
    for(WidgetComponent* c: components){
        c->OnPaint(this);}
}

bool Widget::HandleInput(Input::Event event_type){
    if(!active)return false;
    for(WidgetComponent* c: components){if(c->OnInput(this,event_type))return true;}
    return false;
}
void Widget::HandleResize(){
    layout.Resize();
    for(WidgetComponent* c: components){c->OnResize(this);}
}

bool Widget::HandleSignal(Signal signal){
    if(!active)return false;
    for(WidgetComponent* c: components){if(c->OnSignal(this,signal))return true;}
    return false;
}


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