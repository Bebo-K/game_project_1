#include <game_project_1/types/data_types.hpp>


void* i_to_p(int ptr){
    byte* ret = null;
    ret += ptr;
    return ret;
}