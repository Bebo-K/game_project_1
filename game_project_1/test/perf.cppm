export module Performance;

import OS;

export typedef unsigned long long nanosec;
export typedef unsigned long millisec;

namespace Performance
{
    const int TRACKING_WINDOW = 10;//In seconds
    export class Timer{//Timer with near-nanosecond precision
    public:
        nanosec averages[TRACKING_WINDOW];
        nanosec started;
        Timer():started(0){for(int i=0;i<TRACKING_WINDOW;i++){averages[i]=-1;}}
        
        void Start(){started = OS::time_nano();}
        nanosec Stop(){
            nanosec system_time = OS::time_nano();
            nanosec ns = system_time - started;
            for(int i=10;i>TRACKING_WINDOW+1;i++){averages[i] = averages[i-1];}
            averages[0] = ns;
            return ns;
        }
        nanosec GetAverage(){
            long res = 0;
            for(int i=0;i<TRACKING_WINDOW;i++){
                if(averages[i] > 0){
                    res += averages[i];
                }
            }
            res /= TRACKING_WINDOW;
            return res;
        }
    };

    export class Counter{//Counter with running averages.
    public:
        int averages[TRACKING_WINDOW];
        int count;
        Counter():count(0){
            for(int i=0;i<TRACKING_WINDOW;i++){averages[i]=-1;}
        }
        
        void Increment(){count++;}
        void Reset(){
            for(int i=TRACKING_WINDOW-1; i>0; i++){
                averages[i] = averages[i-1];
            }
            averages[0] = count;
            count =0;
        }
        float GetAverage(){
            long res = 0;
            for(int i=0;i<TRACKING_WINDOW;i++){
                if(averages[i] > 0){
                    res += averages[i];
                }
            }
            res /= (float)TRACKING_WINDOW;
            return res;
        }
        int GetCount(){return count;}
    };

    export class Alarm{//Alarm with ms precision
    private:
        millisec started;
        millisec last_trigger;
    public:
        millisec interval;
        Alarm(millisec after):last_trigger(0),interval(after){started = OS::time_ms();}

        bool Time_Over(){
            millisec ms = OS::time_ms() - started;
            if(ms > interval){
                started = OS::time_ms();
                return true;
            }
            return false;
        }
    };
    
    //TODO: blit this data to screen in some way
    export Counter frames;
    export Counter draws;
    export Counter ticks;
    export int polls_last_second;
    export int draws_last_second;
    export int updates_last_second;
    export int ticks_last_second;
} 