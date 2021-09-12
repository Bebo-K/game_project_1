#include "rect_component.h"

using namespace UI;

RectComponent::RectComponent(vec4 colour):shape(){
    shape.color = colour;
}

void RectComponent::OnPaint(Widget* w){
    shape.Draw();
}

void RectComponent::OnResize(Widget* w){
    shape.rect = w->layout.GetRect();
}