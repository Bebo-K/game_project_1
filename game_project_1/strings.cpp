#include <game_project_1/strings.hpp>

//TODO- run all calls of this thru localization table.
char* Translate(const char* msg){
    return cstr::new_copy(msg);
}
wchar* TranslateW(const char* msg){
    return wstr::from_cstr(msg);
}