#include "ui.h"
#include "../os.h"
#include "../config.h"
#include "../log.h"
#include <math.h>

UI::UI() : renderer(){}

void UI::Load(){
    logger::info("loading ui...\n");

    renderer.Load();
    ShaderManager::AddShader("ui_default","dat/gfx/ui_default.vrt","dat/gfx/ui_default.frg");
    ShaderManager::AddShader("text_default","dat/gfx/text_default.vrt","dat/gfx/text_default.frg");
    
    renderer.camera.ortho=true;
    renderer.camera.pitch=0;
    renderer.camera.yaw=0;
    renderer.camera.turn=0;

    //defaultSprite = new Sprite("dat/img/atlas_1.png",1024,1024,1,1,0,0);
    //defaultSprite->y += 100;
    //renderer.Add(defaultSprite);
}

void UI::Paint(){
    renderer.camera.width =Window::width;//config::ui_width;
    renderer.camera.height=Window::height;//config::ui_height;
    
    renderer.camera.x=0;//config::window_width/2;//config::ui_width;
    renderer.camera.y= config::window_height/2;//config::ui_height;
    renderer.Draw();
}

void UI::Update(Scene* scene, int frames){


}

void UI::Unload(){
    logger::info("unloading ui...\n");

    //renderer.Remove(defaultSprite);
}