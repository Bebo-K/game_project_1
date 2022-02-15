#include "data_types.h"


void* i_to_p(int ptr){
    byte* ret = null;
    ret += ptr;
    return ret;
}