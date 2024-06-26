#ifndef CLIENT_SIGNAL_HANDLER_H
#define CLIENT_SIGNAL_HANDLER_H

#include <game_project_1/signal.hpp>
#include <game_project_1/client/client.hpp>
#include <game_project_1/types/timestep.hpp>

namespace ClientSignalType{
    const int CONNECTED_TO_SERVER=102;
    const int DISCONNECTED_FROM_SERVER=103;
    const int LOCAL_SERVER_READY=104;

};


namespace ClientSignalHandler{
    extern Client* client;

    void Init(Client* c);
    void Update(Timestep delta);
    void Signal(EventSignal signal);
    void Free();






};

#endif