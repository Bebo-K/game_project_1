#include "text_renderer.h"
#include "../io/image.h"

Image* text_img_atlas;




const int TEXT_ATLAS_SIZE = 1024;


void TextRenderer::Init(){
    text_img_atlas = new Image(TEXT_ATLAS_SIZE,TEXT_ATLAS_SIZE);


}



