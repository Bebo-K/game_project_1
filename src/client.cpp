#include "client.h"
#include "log.h"
#include <stdio.h>

void Client::Load(){
    logger::info("Initializing client...\n");

    ShaderManager::AddShader("basic_lighting","dat/gfx/basic_lighting.vrt","dat/gfx/basic_lighting.frg");

    scene.Load();
    ui.Load();
}

void Client::Paint(){
    scene.Paint();
    ui.Paint();
}

void Client::Update(int ms){
    scene.Update(ms);
    ui.Update(ms,&scene);
}