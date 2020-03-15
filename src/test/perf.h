#ifndef _PERFORMANCE_H
#define _PERFORMANCE_H

#include <chrono>



namespace Performance
{
    const int TRACKING_WINDOW = 10;
    class Timer{
        public:
        long averages[TRACKING_WINDOW];
        std::chrono::time_point<std::chrono::system_clock> started;
        Timer();
        void Start();
        long Stop();
        long GetAverage();
    };
    class Counter{
        public:
        long averages[TRACKING_WINDOW];
        long count;
        Counter();
        void Increment();
        void Reset();
        long GetAverage();
        long GetCount();
    };
    class Alarm{
        private:
            std::chrono::time_point<std::chrono::system_clock> started;
            long long last_trigger;
        public: 
        long interval;

        Alarm();
        void Reset();
        bool Time_Over();
    };
} // namespace performance





#endif