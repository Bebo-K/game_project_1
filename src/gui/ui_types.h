#ifndef UI_TYPES_H
#define UI_TYPES_H


typedef int MenuID;
struct MenuType{
    //0 is reserved for "no menu" constant
    static const MenuID MAIN_MENU=1;
    static const MenuID LOADING=2;
    static const MenuID INGAME=3;
    static const MenuID OPTIONS=4;
};

struct UISignal{
    int signal_id;
    int metadata_len;
    byte* metadata;
};













#endif