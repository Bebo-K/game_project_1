#include "dev_console.h"
#include "../../log.h"
#include "../../os.h"
#include "../../struct/data_types.h"

using namespace UI;

DeveloperConsole* DeveloperConsole::instance = null;
FontID DeveloperConsole::console_font = -1;

DeveloperConsole::DeveloperConsole() : entry_line(),background_rect(256,512),entry_rect(256,18){
    DeveloperConsole::instance = this;
    start_timer=999;
    layout.width_scale=Absolute;
    layout.height_scale=Absolute;
    layout.h=200;
    layout.w=512;

    background_rect.color = {0,0,0,0.6};
    entry_rect.color = {0,0,0,0.8};

    last_line_indx=0;
    memset(line_buffers,0,sizeof(text_char)*CACHED_LINE_COUNT*MAX_LINE_LENGTH);
    memset(entry_buffer,0,sizeof(text_char)*MAX_LINE_LENGTH);
    memset(shown_lines,0,sizeof(UI_Text)* SHOWN_LINE_COUNT);
    if(console_font < 0){
        console_font = FontManager::LoadFontFace("Merriweather/Merriweather-Regular",14);
    }

    //Start inactive by default
    visible = false;
    active = false;
}
DeveloperConsole::~DeveloperConsole(){
    if(DeveloperConsole::instance == this){DeveloperConsole::instance = null;}
}

void DeveloperConsole::OnPaint(){
    background_rect.Draw();
    entry_rect.Draw();
    for(int i=0; i < DeveloperConsole::SHOWN_LINE_COUNT;i++){
        shown_lines[i].Draw();
    }
    entry_line.Draw();
}

void DeveloperConsole::OnResize(){
    rect_i layoutrect = layout.GetRect().to_integers();
    background_rect.rect = {layoutrect.x,layoutrect.y+(font_size+8),
                            layoutrect.w,layoutrect.h-(font_size+8)};
                                
    entry_rect.rect = {layoutrect.x,layoutrect.y,
                       layoutrect.w,font_size+8};

    for(int i=0; i < DeveloperConsole::SHOWN_LINE_COUNT;i++){
        shown_lines[i].x = layoutrect.x+2;
        shown_lines[i].y = layoutrect.y+10 + (i+1)*(font_size+4);
    }
    entry_line.x = layoutrect.x;
    entry_line.y = layoutrect.y+2;// (SHOWN_LINE_COUNT*(font_size+2)) + 8;
}

void DeveloperConsole::OnUpdate(int frames){
    if(!active){start_timer=999;}
    else if(start_timer > 0){start_timer--;}
}

bool DeveloperConsole::OnInput(Input::Event event_type){
    if(event_type == Input::ToggleConsole){
        if(Controller::GetAnyButton().IsJustPressed()){
            active = !active;
            start_timer=3;
            return true;
        }
    }
    if(!active)return false;
    if(event_type == Input::PC_Text && start_timer == 0){
        int cursor;
        text_char* input = Controller::GetTextInput();
        for(cursor=0;entry_buffer[cursor]!=0;cursor++);

        for(int i=0;input[i] != 0;i++){
            //handle backspace
            if(input[i] == 0x08 && cursor > 0){
                entry_buffer[cursor-1]=0;
                cursor--;
            }
            else if(input[i] == 0x0D && cursor > 0){
                memcpy(&line_buffers[DeveloperConsole::MAX_LINE_LENGTH*last_line_indx],
                    entry_buffer,sizeof(text_char)*DeveloperConsole::MAX_LINE_LENGTH);
                memset(entry_buffer,0,sizeof(text_char)*DeveloperConsole::MAX_LINE_LENGTH);
                cursor=0;
                last_line_indx++;
                if(last_line_indx >= DeveloperConsole::CACHED_LINE_COUNT){
                    last_line_indx = 0;
                }
                UpdateShownLines();
            }
            else if(cursor < 255){
                entry_buffer[cursor]=input[i];
                cursor++;
            }
        }
        entry_line.SetString(entry_buffer,console_font);
        return true;
    }
    ///catch all other inputs while console is open.
    return true;
}



void DeveloperConsole::Write(char* str){
    if(instance != null){
        if(cstr::len(str) < MAX_LINE_LENGTH){
            TextString::write(str,&instance->line_buffers[instance->last_line_indx*MAX_LINE_LENGTH]);
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
        text_char* line = &line_buffers[line_index*MAX_LINE_LENGTH];
        if(line[0] != 0){
            shown_lines[retrieved_line_count].SetString(line,console_font);
            retrieved_line_count++;
        }
    }
}