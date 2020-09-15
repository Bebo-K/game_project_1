#include "ui.h"
#include "../config.h"
#include "../log.h"


UI::UI() : renderer(){}

void UI::Load(){
    logger::info("loading ui...\n");

    renderer.Load();
    ShaderManager::AddShader("ui_default","dat/gfx/ui_default.vrt","dat/gfx/ui_default.frg");
    
    renderer.camera.ortho=true;
    renderer.camera.pitch=0;
    renderer.camera.yaw=0;
    renderer.camera.turn=0;
    renderer.camera.width =config::ui_width;
    renderer.camera.height=config::ui_height;

    renderer.camera.x=(config::ui_width/2.0f)+1;
    renderer.camera.y=(config::ui_height/2.0f)+1;
    renderer.camera.z=1;
    //renderer.camera.ortho=true;

    //defaultSprite = new Sprite("dat/img/atlas_1.png",1024,1024,1,1,0,0);
    //defaultSprite->y += 100;
    //renderer.Add(defaultSprite);
}

void UI::Paint(){
    renderer.Draw();
}

void UI::Update(Scene* scene, int frames){


}

void UI::Unload(){
    logger::info("unloading ui...\n");

    //renderer.Remove(defaultSprite);
}