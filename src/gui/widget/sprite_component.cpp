#include "sprite_widget.h"


    SpriteWidget::SpriteWidget(char* filename): sprite(TextureManager::Get(filename)){
        layout.W = sprite.width;
        layout.H = sprite.height;
    }

    SpriteWidget::SpriteWidget(Texture sprite_tex):sprite(sprite_tex){
        layout.W = sprite.width;
        layout.H = sprite.height;
    }

    SpriteWidget::SpriteWidget(Texture sprite_tex, int w, int h):sprite(sprite_tex){
        layout.W =w;
        layout.H =h;
        sprite.scale.x = (w/sprite.width);
        sprite.scale.y = (h/sprite.height);
    }

    void SpriteWidget::OnPaint(){
        sprite.Draw();
    }
    void SpriteWidget::OnResize(){
        sprite.x = layout.X;
        sprite.y = layout.Y;
        /*TODO: squishing ui sprites*/
    }