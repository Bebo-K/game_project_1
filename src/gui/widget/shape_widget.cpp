#include "shape_widget.h"



RectWidget::RectWidget(vec4 colour){
    shape.color = colour;
}

RectWidget::RectWidget(int w,int h,vec4 colour):shape(w,h){
    layout.W =w;
    layout.H =h;
    shape.color = colour;
};

RectWidget::~RectWidget(){Destroy();};

void RectWidget::OnPaint(){
    shape.Draw();
}

void RectWidget::OnResize(){
    shape.rect.x = layout.X;
    shape.rect.y = layout.Y;
    shape.rect.w = layout.W;
    shape.rect.h = layout.H;
}