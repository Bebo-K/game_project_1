#include "perf.h"


Performance::Timer::Timer(){
    for(int i=0;i<TRACKING_WINDOW;i++){averages[i]=-1;}

}
void Performance::Timer::Start(){
    started = std::chrono::high_resolution_clock::now();
}
long Performance::Timer::Stop(){
    std::chrono::time_point<std::chrono::system_clock> current_time = std::chrono::high_resolution_clock::now();
    std::chrono::milliseconds ms = std::chrono::duration_cast< std::chrono::milliseconds >( current_time - started );
    
    long ms_count = ms.count();

    for(int i=10;i>TRACKING_WINDOW+1;i++){
        averages[i] = averages[i-1];
    }
    averages[0] = ms_count;
    return ms_count;
}
long Performance::Timer::GetAverage(){
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
long Performance::Counter::GetAverage(){
    long res = 0;
    for(int i=0;i<TRACKING_WINDOW;i++){
        if(averages[i] >= 0){
            res += averages[i];
        }
    }
    res /= TRACKING_WINDOW;
    return res;
}
long Performance::Counter::GetCount(){return count;}


Performance::Alarm::Alarm(){
    started = std::chrono::high_resolution_clock::now();
}
bool Performance::Alarm::Time_Over(){
    std::chrono::time_point<std::chrono::system_clock> current_time = std::chrono::high_resolution_clock::now();
    std::chrono::milliseconds ms = std::chrono::duration_cast< std::chrono::milliseconds >( current_time - started );
    if(ms.count() > interval){
        started = std::chrono::high_resolution_clock::now();
        return true;
    }
    return false;
}