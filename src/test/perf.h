#ifndef _PERFORMANCE_H
#define _PERFORMANCE_H



typedef unsigned long long nanosec;
typedef unsigned long millisec;
namespace Performance
{
    
    const int TRACKING_WINDOW = 10;//In seconds
    class Timer{//Timer with near-nanosecond precision
        public:
        nanosec averages[TRACKING_WINDOW];
        nanosec started;
        Timer();
        void Start();
        nanosec Stop();
        nanosec GetAverage();
    };
    class Alarm{//Alarm with ms precision
        private:
            millisec started;
            millisec last_trigger;
        public: 
        millisec interval;

        Alarm();
        void Reset();
        bool Time_Over();
    };
    class Counter{//Counter with running averages.
        public:
        int averages[TRACKING_WINDOW];
        int count;
        Counter();
        void Increment();
        void Reset();
        float GetAverage();
        int GetCount();
    };



    //TODO: blit this data to screen in some way
    extern Performance::Counter frames;
    extern Performance::Counter draws;
    extern Performance::Counter ticks;
    extern int polls_last_second;
    extern int draws_last_second;
    extern int updates_last_second;
    extern int ticks_last_second;
} 




#endif