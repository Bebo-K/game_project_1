export module UUID;

import <stdlib>;
import <time>;
import <cstdint>;

export int seeded_rand(int seed){
    srand(seed);
    int result = rand();
    srand(time(NULL));
    return result;
}

export int uuid(void* ptr){
    auto i = reinterpret_cast<std::uintptr_t>(ptr);
    return (int)i;
}