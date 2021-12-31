#ifndef SERVER_NETWORK_SYNC_H
#define SERVER_NETWORK_SYNC_H

#include "../net/network.h"

namespace ServerNetworkSynchronizer{

    void Init(ServerNetwork* net);

    void Update(int frames);


}



#endif