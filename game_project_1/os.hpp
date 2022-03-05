#ifndef OS_H
#define OS_H

// Wrapped OS Functions we need to hook for game engine.

namespace OS{
    //Time since program started.
    long time_ms();
    long long time_nano();

    // OS specific threading 
    void StartThread(void (*thread_main)() );
    void StartThread(void (*thread_main)(void*), void* param);
    void SleepThread(int ms);
    void* GetMutexLock(void* object,int timeout);
    bool ReleaseMutexLock(void* lock);

    // External filesystem access
    bool BuildGameFolderPath();
    wchar_t* GetGameFolderPath();
};




namespace Window{
    extern int width;
    extern int height;
    extern int DPI;
};


#endif