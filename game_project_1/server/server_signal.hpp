#include <game_project_1/signal.hpp>
#include <game_project_1/server/server.hpp>

namespace ServerSignalID{
    const int SERVER_STARTED = 100;
    const int SERVER_SHUTDOWN = 999;
}


namespace ServerSignalHandler{
    extern Server* server;

    void Init(Server* c);
    void Update(int frames);
    void Signal(EventSignal signal);
    void Free();

};
