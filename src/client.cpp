#include "client.h"
#include "io/log.h"
#include "io/config.h"
#include <stdio.h>

Client::Client(){
    logger::info("Initializing client...");
    scene = Scene();
    ui = UI();
    scene.Load();
    
}

void Client::Paint(){
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); 
	glViewport(0, 0,config::window_width,config::window_height);
	glEnable(GL_BLEND);
	glEnable(GL_TEXTURE_2D);
    glDisable(GL_DEPTH_TEST);
	glEnable(GL_ALPHA_TEST);
	glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
 
    scene.Paint();
    ui.Paint();
}

void Client::Update(int ms){
    scene.Update(ms);
    ui.Update(ms,&scene);
}