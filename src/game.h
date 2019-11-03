
#include <chrono>
using namespace std::chrono;

class Game{

    time_point<system_clock> last_frame;
    time_point<system_clock> last_render;
    double frame_interval = 1000.0; 
    double render_interval = 2000.0;



    public:
    
    bool running;

    void start();
    void poll();
    void paint();
    void update(int ms);
    


};


