#include "rect_component.h"

using namespace UI;

RectComponent::RectComponent(vec4 colour):shape(){
    shape.color = colour;
}
RectComponent::~RectComponent(){}

void RectComponent::OnPaint(Widget* w){
    shape.Draw();
}

void RectComponent::OnResize(Widget* w){
    rect_f l = w->layout.GetRect();
    shape.rect = {(int)l.x, (int)l.y, (int)l.w, (int)l.h};
}