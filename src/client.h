#ifndef CLIENT_H
#define CLIENT_H

#include "gui/ui.h"
#include "game/scene.h"

class Client{

    public:

    Scene scene;
    UI ui;

    Client();
    void Load();
    void Paint();
    void Update(int ms);


};

#endif