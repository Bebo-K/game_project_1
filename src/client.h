#ifndef CLIENT_H
#define CLIENT_H

#include "gfx/renderer.h"


class Client{

    public:

    //Scene scene;
    //UI ui;
    Renderer scene_renderer;
    Renderer ui_renderer;


    Client();


    void paint();
    void update(int ms);


};

#endif