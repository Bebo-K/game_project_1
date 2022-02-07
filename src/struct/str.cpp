#include "str.h"
#include <iostream>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <wchar.h>

int cstr::len(const char* str){ 
    if(str==nullptr)return 0;
    int i;
    for(i=0;str[i]!=0;i++);
    return i;
}
char* cstr::allocf(const char* format, ...){
    va_list args;
    va_start (args,format);

    int guess_length = len(format)+16+1;
    char* ret= (char*)malloc(guess_length);
    int real_length = snprintf(ret,guess_length,format,args);

    if(real_length < guess_length)return ret;
    free(ret);
    ret = (char*)malloc(real_length+1);
    sprintf(ret,format,args);
    va_end (args);
    return ret;
}
char* cstr::from_wstr(const wchar* widestr){//truncates high-order bits.
    if(widestr==nullptr)return nullptr;
    int str_len = wstr::len(widestr);
    char* str = (char*)malloc(str_len+1);
    for(int i=0;i<str_len;i++){str[i]=widestr[i];}
    str[str_len]=0;
    return str;
}
char* cstr::from_wstr_utf16(const wchar* widestr){
    if(widestr==nullptr){return nullptr;}
    int utf_8_len=1;
    //loop through twice, first to get length of code points
    for(int i=0;widestr[i] != 0;i++){
        int code = widestr[i];
        if(code > 0xD7FF && (code > 0xFFFF || code < 0xE000)){//If 0xD800-0xDF00, it's 2 wchars wide
            code = 0x10000 + ((0xD800-widestr[i]) << 10) + (0XDC00 -widestr[i+1]);
            i++;
        }
        utf_8_len += 1;
        if(code > 0x7F) utf_8_len += 1;
        if(code > 0x7FF) utf_8_len += 1;
        if(code > 0xFFFF) utf_8_len += 1;
    }
    //second to write string
    char* utf8_str = (char*)calloc(utf_8_len,sizeof(char));
    int j=0;//index for utf output
    for(int i=0;widestr[i] != 0;i++){
        int code = widestr[i];
        if(code > 0xD7FF && (code > 0xFFFF || code < 0xE000)){
            code = 0x10000 + ((0xD800-widestr[i]) << 10) + (0XDC00 -widestr[i+1]);
            i++;
        }
        if(code < 0x7F){
            utf8_str[j] = (char)code;
            j++;
        }
        else if(code < 0x7FF){
            utf8_str[j] = 0xC0  + ((code>>6)|0x1F); 
            utf8_str[j+1] = 0x80 + (code|0x3F);
            j+=2;
        }
        else if(code < 0xFFFF){
            utf8_str[j]   = 0xE0 + ((code>>12)|0xF); 
            utf8_str[j+1] = 0x80 + ((code>>6)|0x3F); 
            utf8_str[j+2] = 0x80 + (code|0x3F);
            j+=3;
        }
        else{
            utf8_str[j]   = 0xF0 + ((code>>18)|0x7); 
            utf8_str[j+1] = 0x80 + ((code>>12)|0x3F);
            utf8_str[j+2] = 0x80 + ((code>>6)|0x3F);  
            utf8_str[j+3] = 0x80 + (code|0x3F);
            j+=4;
        }
    }
    utf8_str[j]=0;
    return utf8_str;
}
char* cstr::new_copy(const char* str){
    if(str==nullptr)return nullptr;
    int str_len = len(str);
    if(str_len==0)return nullptr;
    char* s = (char*)malloc(str_len+1);
    strcpy(s,str);
    s[str_len]=0;
    return s;
}
char* cstr::lowercase_copy(const char* str){
    char* s = new_copy(str);
    for(int i=0;s[i]!=0;i++){
        if(s[i] >= 'A' && s[i] <= 'Z')s[i] += ('A'-'a');
    }
    return s;
}
char* cstr::append(const char* str1,const char* str2){
    int str1_len = len(str1);
    int str2_len = len(str2);
    char* str = (char*)malloc(str1_len+str2_len+1);
    strcpy(str,str1);
    strcpy(&str[str1_len],str1);
    str[str1_len+str2_len]=0;
    return str;
}
char* cstr::append(const char* str1, char seperator, const char* str2){
    int str1_len = len(str1);
    int str2_len = len(str2);
    char* str = (char*)malloc(str1_len+str2_len+2);
    strcpy(str,str1);
    str[str1_len]=seperator;
    strcpy(&str[str1_len+1],str2);
    str[str1_len+1+str2_len]=0;
    return str;
}
char* cstr::append(const char* str1,char char1){
    int strlen = len(str1);
    char* str = (char*)malloc((strlen+2)*sizeof(char));
    if(str != nullptr){strcpy(str,str1);}
    str[strlen]=char1;
    str[strlen+1]=0;
    return str;
}
bool cstr::compare(const char* str1,const char* str2){
    if(str1 == str2)return true;//pointer comparison shortcut
    return strcmp(str1,str2)==0;
}
bool cstr::starts_with(const char* str, const char* start){
    int i=0;
    for(i=0;str[i] != 0 && start[i]!= 0;i++){
        if(str[i] != start[i])return false;
    }
    if(str[i]==0 && start[i] != 0)return false;
    return true;
}
bool cstr::contains(const char* str, const char* substr){
    int i,j;
    for(i=0;str[i] != 0;i++){
        for(j=0;str[i+j]==substr[j];j++){
            if(substr[j]==0){return true;}
        }
    }
    return false;
}
char* cstr::substr(const char* str,int start,int length){
    char* ret = (char*)malloc(length+1);
    memcpy(ret,&str[start],length);
    ret[length]=0;
    return ret;
}
char* substring_before(const char* str,char separator,bool last){
    int sep_pos =0;
    for(int i=0;str[i]!=0;i++){if(str[i]==separator){sep_pos=i+1;if(!last)break;}}
    int new_strlen = sep_pos-1;
    if(new_strlen==0)return nullptr;
    char* result= (char*)calloc(new_strlen+1,sizeof(char));
    for(int i=0;i<sep_pos;i++){
        result[i]=str[i];
    }
    result[new_strlen]=0;
    return result;
}
char* substring_after(const char* str,char separator,bool last){
    int old_strlen=cstr::len(str);
    int sep_pos = old_strlen;
    for(int i=0;str[i]!=0;i++){if(str[i]==separator){sep_pos=i+1;if(!last)break;}}
    int new_strlen = old_strlen-sep_pos;
    if(new_strlen <= 0)return nullptr;
    char* result= (char*)calloc(new_strlen+1,sizeof(char));
    for(int i=sep_pos;str[i]!=0;i++){
        result[i-sep_pos]=str[i];
    }
    result[new_strlen]=0;
    return result;
}
char* cstr::substr_before(const char* str,char separator){
return substring_before(str,separator,false);
}
char* cstr::substr_after(const char* str,char separator){
    return substring_after(str,separator,false);
}
char* cstr::substr_before_last(const char* str,char separator){
    return substring_before(str,separator,true);
}
char* cstr::substr_after_last(const char* str,char separator){
    return substring_after(str,separator,true);
}
int cstr::integer_from_string(const char* str){
    int value=0;
    for(int index=0;str[index]!=0;index++){
        if(str[index] >= '0' && str[index] <= '9'){
            value *= 10;
            value += str[index]-'0';
        }
        else return -1;
    }
    return value;
}
char* cstr::write_integer_string(int a){
    int len=0;
    for(int val=a;val!=0;len++){
        val = val/10;
    }
    char* ret = (char*)malloc(len+1);
    int ones=0;
    int val = a;
    for(int i=0;i<len;i++){
        ones = val%10;
        ret[(len-1)-i]=ones+'0';
        val/= 10;
    }
    
    ret[len]=0;
    return ret;
}
float cstr::float_from_string(const char* str){
    return (float)atof(str);
}
char* cstr::write_float_string(float a){
    int before_decimal_digits = 1;
    int after_decimal_digits=8;
    int sign_place = (a<0)? 1:0;
    float a2=(a<0)?-a:a;
    float highest_base_10 =1;
    while(highest_base_10*10 < a2){highest_base_10 *= 10;before_decimal_digits++;}

    char* str = (char*)malloc(sign_place+before_decimal_digits+1+after_decimal_digits+1);
    int mark=0;
    int digit=0;
    if(a<0){str[mark]='-';mark++;}
    while(highest_base_10 > 1){
        digit=0;
        while(a2>highest_base_10){a2-=highest_base_10;digit++;}
        str[mark]=digit;mark++;
        highest_base_10/=10;
    }
    str[mark]='.';
    highest_base_10=0.1f;
    for(int i=0;i<after_decimal_digits;i++){
        digit=0;
        while(a2>highest_base_10){a2-=highest_base_10;digit++;}
        str[mark]=digit;mark++;
        highest_base_10/=10;
    }
    str[mark]=0;
    return str;
}
bool cstr::bool_from_string(const char* str){
    if(cstr::compare(str,"true"))return true;
    if(cstr::compare(str,"false"))return false;
    return false;
}
char* cstr::write_bool_string(bool val){
    return cstr::new_copy(val?"true":"false");
}













///
//Wide Strings (wstr/wchar)
///


int wstr::len(const wchar* str){ 
    if(str==nullptr)return 0;
    int i;
    for(i=0;str[i]!=0;i++);
    return i;
}
wchar* wstr::allocf(const wchar* format, ...){
    va_list args;
    va_start (args,format);

    int guess_length = len(format)+16+1;
    wchar* ret= (wchar*)malloc(sizeof(wchar)*guess_length);
    int real_length = vsnwprintf(ret,guess_length,format,args);

    if(real_length < guess_length)return ret;
    free(ret);
    ret = (wchar*)malloc(sizeof(wchar)*(real_length+1));
    vswprintf(ret,format,args);
    

    va_end (args);
    return ret;
}
wchar* wstr::from_cstr(const char* cstr){//truncates high-order bits.
    if(cstr==nullptr)return nullptr;
    int str_len = cstr::len(cstr);
    wchar* str = (wchar*)malloc((str_len+1)*sizeof(wchar));
    for(int i=0;i<str_len;i++){str[i]=cstr[i];}
    str[str_len]=0;
    return str;
}
wchar* wstr::new_copy(const wchar* str){
    if(str==nullptr)return nullptr;
    int str_len = len(str);
    if(str_len==0)return nullptr;
    wchar* s = (wchar*)malloc((str_len+1)*sizeof(wchar));
    wcscpy(s,str);
    s[str_len]=0;
    return s;
}
wchar* wstr::lowercase_copy(const wchar* str){
    wchar* s = new_copy(str);
    for(int i=0;s[i]!=0;i++){
        if(s[i] >= 'A' && s[i] <= 'Z')s[i] += ('A'-'a');
    }
    return s;
}
wchar* wstr::append(const wchar* str1,const wchar* str2){
    int str1_len = len(str1);
    int str2_len = len(str2);
    wchar* str = (wchar*)malloc((str1_len+str2_len+1)*sizeof(wchar));
    wcscpy(str,str1);
    wcscpy(&str[str1_len],str1);
    str[str1_len+str2_len]=0;
    return str;
}
wchar* wstr::append(const wchar* str1, wchar seperator, const wchar* str2){
    int str1_len = len(str1);
    int str2_len = len(str2);
    wchar* str = (wchar*)malloc((str1_len+str2_len+2)*sizeof(wchar));
    wcscpy(str,str1);
    str[str1_len]=seperator;
    wcscpy(&str[str1_len+1],str2);
    str[str1_len+1+str2_len]=0;
    return str;
}
wchar* wstr::append(const wchar* str1,wchar char1){
    int strlen = len(str1);
    wchar* str = (wchar*)malloc((strlen+2)*sizeof(wchar));
    if(str1 != nullptr){wcscpy(str,str1);}
    str[strlen]=char1;
    str[strlen+1]=0;
    return str;
}
bool wstr::compare(const wchar* str1,const wchar* str2){
    if(str1 == str2)return true;//pointer comparison shortcut
    return wcscmp(str1,str2)==0;
}
bool wstr::starts_with(const wchar* str, const wchar* start){
    int i=0;
    for(i=0;str[i] != 0 && start[i]!= 0;i++){
        if(str[i] != start[i])return false;
    }
    if(str[i]==0 && start[i] != 0)return false;
    return true;
}
bool wstr::contains(const wchar* str, const wchar* substr){
    int i,j;
    for(i=0;str[i] != 0;i++){
        for(j=0;str[i+j]==substr[j];j++){
            if(substr[j]==0){return true;}
        }
    }
    return false;
}
wchar* wstr::substr(const wchar* str,int start,int length){
    wchar* ret = (wchar*)malloc((length+1)*sizeof(wchar));
    memcpy(ret,&str[start],length*sizeof(wchar));
    ret[length]=0;
    return ret;
}
wchar* substring_before(const wchar* str,wchar separator,bool last){
    int sep_pos =0;
    for(int i=0;str[i]!=0;i++){if(str[i]==separator){sep_pos=i+1;if(!last)break;}}
    int new_strlen = sep_pos-1;
    if(new_strlen==0)return nullptr;
    wchar* result= (wchar*)calloc(new_strlen+1,sizeof(wchar));
    for(int i=0;i<sep_pos;i++){
        result[i]=str[i];
    }
    result[new_strlen]=0;
    return result;
}
wchar* substring_after(const wchar* str,wchar separator,bool last){
    int old_strlen=wstr::len(str);
    int sep_pos = old_strlen;
    for(int i=0;str[i]!=0;i++){if(str[i]==separator){sep_pos=i+1;if(!last)break;}}
    int new_strlen = old_strlen-sep_pos;
    if(new_strlen <= 0)return nullptr;
    wchar* result= (wchar*)calloc(new_strlen+1,sizeof(wchar));
    for(int i=sep_pos;str[i]!=0;i++){
        result[i-sep_pos]=str[i];
    }
    result[new_strlen]=0;
    return result;
}
wchar* wstr::substr_before(const wchar* str,wchar separator){
return substring_before(str,separator,false);
}
wchar* wstr::substr_after(const wchar* str,wchar separator){
    return substring_after(str,separator,false);
}
wchar* wstr::substr_before_last(const wchar* str,wchar separator){
    return substring_before(str,separator,true);
}
wchar* wstr::substr_after_last(const wchar* str,wchar separator){
    return substring_after(str,separator,true);
}
int wstr::integer_from_string(const wchar* str){
    int value=0;
    for(int index=0;str[index]!=0;index++){
        if(str[index] >= '0' && str[index] <= '9'){
            value *= 10;
            value += str[index]-'0';
        }
        else return -1;
    }
    return value;
}
wchar* wstr::write_integer_string(int a){
    int len=0;
    for(int val=a;val!=0;len++){
        val = val/10;
    }
    wchar* ret = (wchar*)malloc((len+1)*sizeof(wchar));
    int ones=0;
    int val = a;
    for(int i=0;i<len;i++){
        ones = val%10;
        ret[(len-1)-i]=ones+'0';
        val/= 10;
    }
    
    ret[len]=0;
    return ret;
}
float wstr::float_from_string(const wchar* str){
    return (float)_wtof(str);
}
wchar* wstr::write_float_string(float a){
    int before_decimal_digits = 1;
    int after_decimal_digits=8;
    int sign_place = (a<0)? 1:0;
    float a2=(a<0)?-a:a;
    float highest_base_10 =1;
    while(highest_base_10*10 < a2){highest_base_10 *= 10;before_decimal_digits++;}

    wchar* str = (wchar*)malloc((sign_place+before_decimal_digits+1+after_decimal_digits+1)*sizeof(wchar));
    int mark=0;
    int digit=0;
    if(a<0){str[mark]='-';mark++;}
    while(highest_base_10 > 1){
        digit=0;
        while(a2>highest_base_10){a2-=highest_base_10;digit++;}
        str[mark]=digit;mark++;
        highest_base_10/=10;
    }
    str[mark]='.';
    highest_base_10=0.1f;
    for(int i=0;i<after_decimal_digits;i++){
        digit=0;
        while(a2>highest_base_10){a2-=highest_base_10;digit++;}
        str[mark]=digit;mark++;
        highest_base_10/=10;
    }
    str[mark]=0;
    return str;
}
bool wstr::bool_from_string(const wchar* str){
    if(wstr::compare(str,L"true"))return true;
    if(wstr::compare(str,L"false"))return false;
    return false;
}
wchar* wstr::write_bool_string(bool val){
    return wstr::new_copy(val?L"true":L"false");
}
