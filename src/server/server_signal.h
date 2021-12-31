#include "../signal.h"


namespace ServerSignalID{
    const int SERVER_STARTED = 100;
    const int SERVER_SHUTDOWN = 999;

    const int PLAYER_CONNECTED = 105;//val1 = (wstr*)player name, val2 = player/net target id
    const int PLAYER_DISCONNECTED = 106;//val1 = player/net target id



}
