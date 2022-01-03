#include "../signal.h"
#include "../server/server.h"

namespace ServerSignalID{
    const int SERVER_STARTED = 100;
    const int SERVER_SHUTDOWN = 999;

    const int PLAYER_CONNECTED = 105;//val1 = (wstr*)player name, val2 = player/net target id
    const int PLAYER_FAIL_CONNECT = 106;//val1 = (wstr*)player name, val2 = (wstr*)reason
    const int PLAYER_DISCONNECTED = 107;//val1 = player/net target id, val2 = (wstr*)reason


}


namespace ServerSignalHandler{
    extern Server* server;

    void Init(Server* c);
    void Update(int frames);
    void Signal(EventSignal signal);
    void Free();






};
