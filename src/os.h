#ifndef OS_H
#define OS_H

// Wrapped OS Functions we need to hook for game engine.
long time_ms();
long long time_nano();

void start_thread(void (*thread_main)() );

bool build_game_folder_path();
wchar_t* get_games_folder_path();


namespace Window{
    extern int width;
    extern int height;
    extern int DPI;
};


#endif