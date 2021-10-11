#include "perf.h"
#include "../os.h"

//TODO: blit this data to screen in some way

Performance::Counter Performance::frames;
Performance::Counter Performance::draws;
Performance::Counter Performance::ticks;
int Performance::polls_last_second;
int Performance::draws_last_second;
int Performance::updates_last_second;
int Performance::ticks_last_second;

Performance::Timer::Timer(){
    for(int i=0;i<TRACKING_WINDOW;i++){averages[i]=-1;}
}

void Performance::Timer::Start(){
    started = time_nano();
}

nanosec Performance::Timer::Stop(){
    nanosec system_time = time_nano();
    nanosec ns = system_time - started;
    for(int i=10;i>TRACKING_WINDOW+1;i++){averages[i] = averages[i-1];}
    averages[0] = ns;
    return ns;
}

nanosec Performance::Timer::GetAverage(){
    long res = 0;
    for(int i=0;i<TRACKING_WINDOW;i++){
        if(averages[i] >= 0){
            res += averages[i];
        }
    }
    res /= TRACKING_WINDOW;
    return res;
}

Performance::Counter::Counter(){
    for(int i=0;i<TRACKING_WINDOW;i++){averages[i]=-1;}
}

void Performance::Counter::Increment(){count++;}
void Performance::Counter::Reset(){
    for(int i=10;i>TRACKING_WINDOW+1;i++){
        averages[i] = averages[i-1];
    }
    averages[0] = count;
    count =0;
}
float Performance::Counter::GetAverage(){
    long res = 0;
    for(int i=0;i<TRACKING_WINDOW;i++){
        if(averages[i] >= 0){
            res += averages[i];
        }
    }
    res /= (float)TRACKING_WINDOW;
    return res;
}
int Performance::Counter::GetCount(){return count;}


Performance::Alarm::Alarm(){
    started = time_ms();
}
bool Performance::Alarm::Time_Over(){
    millisec ms = time_ms() - started;
    if(ms > interval){
        started = time_ms();
        return true;
    }
    return false;
}