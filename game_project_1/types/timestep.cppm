export module Timestep;

import Config;


export struct Timestep{
    int   frames;
    float seconds;
};

export struct Timer{
    int frames_left;

    Timer():frames_left(0){}
    Timer(int frames):frames_left(frames){}

    bool Countdown(Timestep delta){
        if(frames_left <= 0){return false;}
        frames_left -= delta.frames;
        if(frames_left <= 0){frames_left=0;return true;}
        return false;
    }
    bool Expired(){
        return frames_left <= 0;
    }

    void Set(float seconds){
        frames_left = (int)(seconds / config::tick_interval);
    }
    void Set(int frames){
        frames_left = frames;
    }
};