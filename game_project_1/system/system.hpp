#ifndef GAME_SYSTEM_H
#define GAME_SYSTEM_H

#include <game_project_1/game/scene.hpp>
//Classes in the /system folder should subclass one of these two objects

class ClientSystem{
    public:

    float Delta(int frame_count);
    int DeltaMS(int frame_count);
    //virtual void UpdateClient(ClientScene* scene,int frames);

};

class ServerSystem{
    public:

    float Delta(int frame_count);
    int DeltaMS(int frame_count);
    //virtual void UpdateClient(ClientScene* scene,int frames);

};




#endif