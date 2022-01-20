#include "dev_console.h"
#include "../../log.h"
#include "../../os.h"
#include "../../struct/data_types.h"

using namespace UI;

DeveloperConsole* DeveloperConsole::instance = null;

DeveloperConsole::DeveloperConsole() : Widget("developer_console") {
    DeveloperConsole::instance = this;
    layout.width_scale=Absolute;
    layout.height_scale=Absolute;
    layout.h=200;
    layout.w=512;
    
    last_line_indx=0;
    console_font=-1;
    memset(line_buffers,0,sizeof(wchar)*CACHED_LINE_COUNT*MAX_LINE_LENGTH);
    memset(entry_buffer,0,sizeof(wchar)*MAX_LINE_LENGTH);

 
    //Start inactive by default
    visible = false;
    active = false;
    text_selected=false;
}
DeveloperConsole::~DeveloperConsole(){
    if(DeveloperConsole::instance == this){DeveloperConsole::instance = null;}
}

void DeveloperConsole::Load(){
    console_font = FontManager::LoadFontFace("Merriweather/Merriweather-Regular",14);
    texts.Resize(1+SHOWN_LINE_COUNT);
        for(int i=0;i< (1+SHOWN_LINE_COUNT);i++){
            texts.Set(i,new UI_Text());
            texts[i]->SetString(L"",console_font);
        }

    rects.Resize(2);
        rects.Set(0,new UI_Rect(256,512));
        rects[0]->color = {0,0,0,0.6};
        rects.Set(1,new UI_Rect(256,18));
        rects[1]->color = {0,0,0,0.8};
}

void DeveloperConsole::OnPaint(){}

void DeveloperConsole::OnResize(){
    rect_i layoutrect = layout.GetRect().to_integers();
    rects[0]->rect = {layoutrect.x,layoutrect.y+(font_size+8),
                            layoutrect.w,layoutrect.h-(font_size+8)};
                                
    rects[1]->rect = {layoutrect.x,layoutrect.y,
                       layoutrect.w,font_size+8};


    texts[0]->x = layoutrect.x+2;
    texts[0]->y = layoutrect.y+2;
    for(int i=0; i < DeveloperConsole::SHOWN_LINE_COUNT;i++){
        texts[1+i]->x = layoutrect.x+2;
        texts[1+i]->y = layoutrect.y+10 + (i+1)*(font_size+4);
    }
}

void DeveloperConsole::OnUpdate(int frames){}

bool DeveloperConsole::OnInput(Input::Event event_type){
    Controller::Button toggle = Controller::GetToggleConsole();
    if(event_type == Input::ToggleConsole && toggle.IsJustPressed()){
        active = !active;
        visible = !visible;
        return true;
    }
    if(!active)return false;
    if(event_type == Input::PC_Text && !toggle.IsDown()){
        int cursor;
        wchar* input = Controller::GetTextInput();
        for(cursor=0;entry_buffer[cursor]!=0;cursor++);

        for(int i=0;input[i] != 0;i++){
            //handle backspace
            if(input[i] == 0x08){
                if(cursor > 0){
                    entry_buffer[cursor-1]=0;
                    cursor--;
                }
            }
            //handle enter
            else if(input[i] == 0x0D){
                if(cursor > 0){
                    memcpy(&line_buffers[DeveloperConsole::MAX_LINE_LENGTH*last_line_indx],
                        entry_buffer,sizeof(wchar)*DeveloperConsole::MAX_LINE_LENGTH);
                    memset(entry_buffer,0,sizeof(wchar)*DeveloperConsole::MAX_LINE_LENGTH);
                    cursor=0;
                    last_line_indx++;
                    if(last_line_indx >= DeveloperConsole::CACHED_LINE_COUNT){
                        last_line_indx = 0;
                    }
                    UpdateShownLines();
                }
            }
            else if(input[i] > 0x1F && cursor < 255){
                entry_buffer[cursor]=input[i];
                cursor++;
            }
        }
        texts[0]->SetString(entry_buffer,console_font);
        return true;
    }
    
    if(event_type == Input::PC_LClick && Controller::GetPCLeftMouse().IsJustPressed()){
        text_selected= (rects[1]->rect.contains(Controller::GetPCCursor()));
    }

    return text_selected;///catch all other inputs while text is selected
}



void DeveloperConsole::Write(wchar* str){
    if(instance != null){
        if(wstr::len(str) < MAX_LINE_LENGTH){
            memcpy(&instance->line_buffers[instance->last_line_indx*MAX_LINE_LENGTH],str,sizeof(wchar)*(wstr::len(str)+1));
            instance->last_line_indx++;
            if(instance->last_line_indx > CACHED_LINE_COUNT){instance->last_line_indx=0;}
        }
        //else{} TODO: log to file in both cases
        instance->UpdateShownLines();
    }
}

void DeveloperConsole::UpdateShownLines(){
    int retrieved_line_count=0;
    for(int line_index = last_line_indx-1; retrieved_line_count < SHOWN_LINE_COUNT && line_index != last_line_indx;line_index--){
        if(line_index < 0){line_index = CACHED_LINE_COUNT-1;}
        wchar* line = &line_buffers[line_index*MAX_LINE_LENGTH];
        if(line[0] != 0){
            texts[1+retrieved_line_count]->SetString(line,console_font);
            retrieved_line_count++;
        }
    }
    texts[0]->SetString(entry_buffer,console_font);
}