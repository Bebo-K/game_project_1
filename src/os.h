#ifndef OS_H
#define OS_H

// Wrapped OS Functions we need to hook for game engine.
long time_ms();
long long time_nano();

namespace Window{
    extern int width;
    extern int height;


};




#endif