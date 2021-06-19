#include "loading_menu.h"


LoadingMenu::LoadingMenu(Layout* parent){
    layout.offset.parent=parent;
    if(parent != null){
        layout.W = parent->W;
        layout.H = parent->H;
    }
    else{
        layout.W = 512;
        layout.H = 256;
    }

}