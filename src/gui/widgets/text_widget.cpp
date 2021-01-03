#include "text_widget.h"


TextWidget::TextWidget(char* text,int h, int w):text(text){
    layout.H=h;
    layout.W=w;
}
TextWidget::~TextWidget(){}

void TextWidget::Paint(){
    text.Draw();
}
void TextWidget::OnResize(){
    text.x = layout.X;
    text.y = layout.Y;
}
void TextWidget::OnUpdate(int frames){}
bool TextWidget::OnInput(InputCode event_type){return false;}
bool TextWidget::OnSignal(int signal_id,int metadata_len, byte* metadata){return false;}