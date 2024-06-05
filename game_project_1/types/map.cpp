#include <game_project_1/types/map.hpp>


bool MapUtils::Compare(char* k1, char* k2){return cstr::compare(k1,k2);}
bool MapUtils::IsNull(char* k){return k==nullptr;}
int  MapUtils::NullValue(int k){return 0;}


void MapUtils::Copy(char*& t,char** dest){(*dest)=cstr::new_copy(t);}
void MapUtils::Copy(int& t,int* dest){(*dest)=t;}

void MapUtils::Unset(char** t){DEALLOCATE(*t)}
void MapUtils::Unset(int* t){(*t)=0;}