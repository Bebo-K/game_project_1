#ifndef CLIENT_H
#define CLIENT_H

#include "game/scene.h"
#include "gui/ui.h"

class Client{

    public:

    Scene scene;
    UI ui;

    Client();


    void Paint();
    void Update(int ms);


};

#endif