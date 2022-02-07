#include "text_widgets.h"
#include "../widget.h"

using namespace UI;


TextBox::TextBox(char* name,FontID font,int num_lines,int line_length,int width,int height):Widget(name){
    font_size = FontManager::GetFontInfo(font)->font_size;
    line_count = num_lines;
    max_line_length = line_length;
    layout.width = width;
    layout.height= height;
    
    line_buffers = (wchar**)calloc(line_count,sizeof(wchar*));
    for(int i=0;i<num_lines;i++){
        line_buffers[i] = (wchar*)calloc(line_length,sizeof(wchar*));
    }
    shown_lines = new DrawableText[line_count];

    wrap_lines=false;
}

TextBox::~TextBox(){
    if(line_buffers != null){
        for(int i=0;i<line_count;i++){free(line_buffers[i]);}
        free(line_buffers);
        line_buffers=null;
    }
    if(shown_lines != null){
        delete[] shown_lines;
        shown_lines=null;
    }
}

void TextBox::OnPaint(){
    for(int i=0;i<line_count;i++){
        shown_lines[i].Draw();
    }
}

void TextBox::OnMove(){
    for(int i=0;i<line_count;i++){
        shown_lines[i].x = layout.center.x;
        shown_lines[i].y = layout.center.y + (layout.height/2) - (i * (font_size + 2));
        shown_lines[i].w = layout.width;
        shown_lines[i].h = font_size;
    }
}

/*
void TextBox::AddLine(wchar* new_line){

}
void TextBox::SetLine(wchar* new_line,int line_num){

}

//Chops entries to fit line size and displays as many as possible, prioritizing last entries
void TextBox::FormatLines(){
    int shown_line_index=line_count-1;
    UI_Text shown_line = shown_lines[shown_line_index];
    for(int i=line_count; i>= 0 && shown_line_index >= 0;i--){
        if(this->wrap_lines){
            int line_count_per_line = 1+wstr::len(lines[i])/max_line_length;
            for(int i=0;i<line_count_per_line;i++){
                shown_lines->SetString(wstr::substr(lines[i],i*max_line_length,max_line_length));
                
                shown_line_index--;shown_line = shown_lines[shown_line_index];
                if(shown_line_index < 0)break;
            }
        }
        else{
            if((wstr::len(lines[i])/max_line_length) > 0 ){
                shown_lines->SetString(wstr::substr(lines[i],0,max_line_length));
                shown_line_index--;shown_line = shown_lines[shown_line_index];  
            }
            else{
                shown_lines->SetString(lines[i]);
                shown_line_index--;shown_line = shown_lines[shown_line_index];  
            }
        }
    }
}
*/


TextEntryField::TextEntryField(char* name,int max_length):Widget(name){
    focused=false;
    player_edited=false;
    font_size=12;
    line = (wchar*)calloc(max_length,sizeof(wchar));
    shown_line = new DrawableText();
}
TextEntryField::TextEntryField(char* name,int max_length, wchar* prompt):Widget(name){
    focused=false;
    player_edited=false;
    font_size=12;
    line = (wchar*)calloc(max_length,sizeof(wchar));
    shown_line = new DrawableText(prompt);
}
TextEntryField::~TextEntryField(){
    if(line != null){free(line);line=null;}
}

void TextEntryField::OnPaint(Widget* w){
    shown_line->Draw();
}