#include "widget.h"
#include "../log.h"

Widget::Widget():sub_widgets(&this->layout){
    name=nullptr;
}

//Destroy is called from the destructor.
void Widget::Destroy(){
    OnDestroy();
    for(Widget* sw: sub_widgets){sw->Destroy();}
    if(name != null){free(name);name=null;}
}

void Widget::SetName(char* new_name){
    if(cstr::contains(new_name,".")){
        name = cstr::substr_before(new_name,'.');
        logger::warn("Widget name '%s' contains a '.' character, truncating to '%s' to preserve name heirarchy.\n",new_name,name);
    }
    else{
        name = cstr::new_copy(new_name);
    }
}

void Widget::ParentTo(Widget* parent,char* my_name){
    parent->sub_widgets.Add(this,my_name);
}


void Widget::Activate(){
    visible=true;
    active=true;
    OnActivate();
    for(Widget* sw: sub_widgets){sw->Activate();}
}

void Widget::Deactivate(){
    visible=false;
    active=false;
    OnDeactivate();
    for(Widget* sw: sub_widgets){sw->Deactivate();}
}

void Widget::Update(int frames){
    if(!active)return;
    OnUpdate(frames);
    for(Widget* sw: sub_widgets){sw->Update(frames);}
}
void Widget::Paint(){
    if(!visible)return;
    OnPaint();
    for(Widget* sw: sub_widgets){sw->Paint();}
}

bool Widget::HandleInput(Input::EventID event_type){
    if(!active)return false;
    bool handled = OnInput(event_type);
    if(!handled){
        for(Widget* sw: sub_widgets){
            handled = sw->HandleInput(event_type);
            if(handled)break;
        }
    } 
    return handled;
}
void Widget::HandleResize(){
    layout.Resize();
    OnResize();
    for(Widget* sw: sub_widgets){sw->HandleResize();}
}


bool Widget::HandleSignal(int signal_id,int metadata_len, byte* metadata){
    if(!active)return false;
    bool handled = OnSignal(signal_id,metadata_len,metadata);
    if(!handled){
        for(Widget* sw: sub_widgets){
            handled = sw->OnSignal(signal_id,metadata_len,metadata);
            if(handled)break;
        }
    } 
    return handled;
}


/*************************
 *   Widget Containers   *
 *************************/

WidgetContainer::WidgetContainer(Layout* parent){
    parent_layout = parent;
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


Widget* WidgetContainer::Add(Widget* w,char* name,LayoutOffset pos){
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