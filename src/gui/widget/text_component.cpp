#include "text_component.h"

using namespace UI;

TextComponent::TextComponent() :text(){}
TextComponent::TextComponent(char* str):text(str){}
TextComponent::TextComponent(char* str,FontID font_id):text(TextString::from_cstr(str),font_id){}
TextComponent::TextComponent(text_char* str):text(str){}
TextComponent::TextComponent(text_char* str,FontID font_id):text(str,font_id){}
TextComponent::~TextComponent(){}

void TextComponent::OnPaint(Widget* w){
    text.Draw();
}
void TextComponent::OnResize(Widget* w){
    text.x = w->layout.center.x - (text.w/2);
    text.y = w->layout.center.y - (text.h/2);
}
void TextComponent::SetString(char* str){
    text.SetString(str);
}
void TextComponent::SetString(text_char* str,FontID font){
    text.SetString(str,font);
}


TextBoxComponent::TextBoxComponent(int num_lines){
    line_count=num_lines;
    max_line_length=256;
    wrap_lines=false;
    font_size=12;
    lines = (text_char**)calloc(line_count,sizeof(text_char*));
    shown_lines = new UI_Text[line_count];
}
TextBoxComponent::TextBoxComponent(int num_lines,int line_length){
    line_count=num_lines;
    max_line_length=line_length;
    wrap_lines=false;
    font_size=12;
    lines = (text_char**)calloc(line_count,sizeof(text_char*));
    shown_lines = new UI_Text[line_count];
}

TextBoxComponent::~TextBoxComponent(){
    if(lines != null){
        for(int i=0;i<line_count;i++){free(lines[i]);}
        free(lines);
        lines=null;
    }
    if(shown_lines != null){
        delete[] shown_lines;
        shown_lines=null;
    }
}

void TextBoxComponent::OnPaint(Widget* w){
    for(int i=0;i<line_count;i++){
        shown_lines[i].Draw();
    }
}

void TextBoxComponent::OnResize(Widget* w){
    for(int i=0;i<line_count;i++){
        shown_lines[i].x = w->layout.center.x;
        shown_lines[i].y = w->layout.center.y + (w->layout.h/2) - (i * (font_size + 2));
        shown_lines[i].w = w->layout.w;
        shown_lines[i].h = font_size;
    }
}
void TextBoxComponent::AddLine(text_char* new_line){

}
void TextBoxComponent::SetLine(text_char* new_line,int line_num){

}

//Chops entries to fit line size and displays as many as possible, prioritizing last entries
void TextBoxComponent::FormatLines(){
    int shown_line_index=line_count-1;
    UI_Text shown_line = shown_lines[shown_line_index];
    for(int i=line_count; i>= 0 && shown_line_index >= 0;i--){
        if(this->wrap_lines){
            int line_count_per_line = 1+TextString::length(lines[i])/max_line_length;
            for(int i=0;i<line_count_per_line;i++){
                shown_lines->SetString(TextString::substr(lines[i],i*max_line_length,max_line_length));
                
                shown_line_index--;shown_line = shown_lines[shown_line_index];
                if(shown_line_index < 0)break;
            }
        }
        else{
            if((TextString::length(lines[i])/max_line_length) > 0 ){
                shown_lines->SetString(TextString::first(lines[i],max_line_length));
                shown_line_index--;shown_line = shown_lines[shown_line_index];  
            }
            else{
                shown_lines->SetString(lines[i]);
                shown_line_index--;shown_line = shown_lines[shown_line_index];  
            }
        }
    }
}


TextEntryComponent::TextEntryComponent(int max_length){
    focused=false;
    player_edited=false;
    font_size=12;
    line = (text_char*)calloc(max_length,sizeof(text_char));
    shown_line = new UI_Text();
}
TextEntryComponent::TextEntryComponent(int max_length, text_char* prompt){
    focused=false;
    player_edited=false;
    font_size=12;
    line = (text_char*)calloc(max_length,sizeof(text_char));
    shown_line = new UI_Text(prompt);
}
TextEntryComponent::~TextEntryComponent(){
    if(line != null){free(line);line=null;}
}

void TextEntryComponent::OnPaint(Widget* w){
    shown_line->Draw();
}