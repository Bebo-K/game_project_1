#include <game_project_1/types/uuid.hpp>
#include <stdlib.h> 
#include <time.h>
#include <cstdint>

int seeded_rand(int seed){
    srand(seed);
    int result = rand();
    srand(time(NULL));
    return result;
}

int uuid(void* ptr){
    auto i = reinterpret_cast<std::uintptr_t>(ptr);
    return (int)i;
}