#include "text_widget.h"

TextWidget::TextWidget() :text(){}
TextWidget::TextWidget(char* str):text(str){}
TextWidget::TextWidget(char* str,FontID font_id):text(TextString::from_cstr(str),font_id){}
TextWidget::TextWidget(text_char* str):text(str){}
TextWidget::TextWidget(text_char* str,FontID font_id):text(str,font_id){}
TextWidget::~TextWidget(){Destroy();}
void TextWidget::OnDestroy(){}
void TextWidget::OnPaint(){
    text.Draw();
}
void TextWidget::OnResize(){
    text.x = layout.X;
    text.y = layout.Y;
}


TextBoxWidget::TextBoxWidget(int num_lines){
    line_count=num_lines;
    max_line_length=256;
    wrap_lines=false;
    font_size=12;
    lines = (text_char**)calloc(line_count,sizeof(text_char*));
    shown_lines = new UI_Text[line_count];
}
TextBoxWidget::TextBoxWidget(int num_lines,int line_length){
    line_count=num_lines;
    max_line_length=line_length;
    wrap_lines=false;
    font_size=12;
    lines = (text_char**)calloc(line_count,sizeof(text_char*));
    shown_lines = new UI_Text[line_count];
}
TextBoxWidget::~TextBoxWidget(){Destroy();}
void TextBoxWidget::OnDestroy(){
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

void TextBoxWidget::OnPaint(){
    for(int i=0;i<line_count;i++){
        shown_lines[i].Draw();
    }
}
void TextBoxWidget::AddLine(text_char* new_line){

}
void TextBoxWidget::SetLine(text_char* new_line,int line_num){

}

//Chops entries to fit line size and displays as many as possible, prioritizing last entries
void TextBoxWidget::FormatLines(){
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


TextEntryWidget::TextEntryWidget(int max_length){
    focused=false;
    player_edited=false;
    font_size=12;
    line = (text_char*)calloc(max_length,sizeof(text_char));
    shown_line = new UI_Text();
}
TextEntryWidget::TextEntryWidget(int max_length, text_char* prompt){
    focused=false;
    player_edited=false;
    font_size=12;
    line = (text_char*)calloc(max_length,sizeof(text_char));
    shown_line = new UI_Text(prompt);
}
TextEntryWidget::~TextEntryWidget(){Destroy();}
void TextEntryWidget::OnDestroy(){
    if(line != null){free(line);line=null;}
}

void TextEntryWidget::OnPaint(){
    shown_line->Draw();
}