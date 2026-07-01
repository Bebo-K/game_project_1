export module String;

import Memory;
import <iostream>;
import <stdlib>;
import <string>;
import <wchar>;
import <stdarg>;

//String and wide string utilites that should exist

export typedef wchar_t wchar;

//pre-resolve wstr::len for cstr from_wstr
namespace wstr {
    export int len(const wchar* str);
}

namespace cstr{
    //C-string length, not including terminating zero
    export int len(const char* str){ 
        if(str==nullptr)return 0;
        int i;
        for(i=0;str[i]!=0;i++);
        return i;
    }
    export void cpy(const char* src,char* dst) {
        if(src==nullptr)return;
        for(int i=0;(dst[i]=src[i])!=0;i++);
    }
    //Allocate formattted C-string
    export char* allocf(const char* format, ...){
        va_list args;
        va_start (args,format);

        int guess_length = len(format)+16+1;
        char* ret= Alloc<char>(guess_length);
        int real_length = snprintf(ret,guess_length,format,args);

        if(real_length < guess_length)return ret;
        Free(ret);
        ret = Alloc<char>(real_length+1);
        sprintf(ret,format,args);
        va_end (args);
        return ret;
    }
    export char* from_wstr(const wchar* widestr){//truncates high-order bits.
        if(widestr==nullptr)return nullptr;
        int str_len = wstr::len(widestr);
        char* str = Alloc<char>(str_len+1);
        for(int i=0;i<str_len;i++){str[i]=widestr[i];}
        str[str_len]=0;
        return str;
    }
    export char* from_wstr_utf16(const wchar* widestr){
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
        char* utf8_str = ZeroAlloc<char>(utf_8_len);
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
    export char* new_copy(const char* str){
        if(str==nullptr)return nullptr;
        int str_len = len(str);
        if(str_len==0)return nullptr;
        char* s = Alloc<char>(str_len+1);
        cpy(str,s);
        s[str_len]=0;
        return s;
    }
    export char* lowercase_copy(const char* str){
        if (str == nullptr)return nullptr;
        char* s = new_copy(str);
        for(int i=0;s[i]!=0;i++){
            if(s[i] >= 'A' && s[i] <= 'Z')s[i] += ('A'-'a');
        }
        return s;
    }
    export char* append(const char* str1,const char* str2){
        int str1_len = len(str1);
        int str2_len = len(str2);
        char* str = Alloc<char>(str1_len+str2_len+1);
        cpy(str1,str);
        cpy(str2,&str[str1_len]);
        str[str1_len+str2_len]=0;
        return str;
    }
    export char* append(const char* str1, char seperator, const char* str2){
        int str1_len = len(str1);
        int str2_len = len(str2);
        char* str = Alloc<char>(str1_len+str2_len+2);
        cpy(str1,str);
        str[str1_len]=seperator;
        cpy(str2,&str[str1_len+1]);
        str[str1_len+1+str2_len]=0;
        return str;
    }
    export char* append(const char* str1,char char1){
        int strlen = len(str1);
        char* str = Alloc<char>(strlen+2);
        if(str1 != nullptr) {
            cpy(str1,str);
        }
        str[strlen]=char1;
        str[strlen+1]=0;
        return str;
    }
    export bool compare(const char* str1,const char* str2){
        if(str1 == str2)return true;//pointer comparison shortcut
        return strcmp(str1,str2)==0;
    }
    export bool compare_caseless(const char* str1,const char* str2){
        if(str1 == str2)return true;//pointer comparison shortcut
        if(str1 == nullptr || str2 == nullptr)return false;
        int indx;
        for(indx=0;str1[indx]==0;indx++){
            char char1=str1[indx];
            char char2=str2[indx];
            if(char2 == 0)return false;
            if(char1 >= 'A'){char1 -= ('A'-'a');}
            if(char2 >= 'A'){char2 -= ('A'-'a');}
            if(char1 != char2)return false;
        }
        if(str2[indx] != 0)return false;
        return true;
    }
    export bool starts_with(const char* str, const char* start){
        int i=0;
        for(i=0;str[i] != 0 && start[i]!= 0;i++){
            if(str[i] != start[i])return false;
        }
        if(str[i]==0 && start[i] != 0)return false;
        return true;
    }
    export bool ends_with(const char* str, const char* end){
        int i=len(str)-1;
        int j=len(end)-1;
        for(;i>=0 && j>=0; i--,j--){
            if(str[i] != end[j])return false;
        }
        if(i < 0)return false;
        return true;
    }

    export bool contains(const char* str, const char* substr){
        int i,j;
        for(i=0;str[i] != 0;i++){
            for(j=0;str[i+j]==substr[j];j++){
                if(substr[j]==0){return true;}
            }
        }
        return false;
    }
    export char* substr(const char* str,int start,int length){
        char* ret = Alloc<char>(length+1);
        MemCopy(ret,&str[start],length);
        ret[length]=0;
        return ret;
    }
    export char* substring_before(const char* str,char separator,bool last){
        int sep_pos =0;
        for(int i=0;str[i]!=0;i++){if(str[i]==separator){sep_pos=i+1;if(!last)break;}}
        int new_strlen = sep_pos-1;
        if(new_strlen==0)return nullptr;
        char* result= ZeroAlloc<char>(new_strlen+1);
        for(int i=0;i<sep_pos;i++){
            result[i]=str[i];
        }
        result[new_strlen]=0;
        return result;
    }
    export char* substring_after(const char* str,char separator,bool last){
        int old_strlen=len(str);
        int sep_pos = old_strlen;
        for(int i=0;str[i]!=0;i++){if(str[i]==separator){sep_pos=i+1;if(!last)break;}}
        int new_strlen = old_strlen-sep_pos;
        if(new_strlen <= 0)return nullptr;
        char* result= ZeroAlloc<char>(new_strlen+1);
        for(int i=sep_pos;str[i]!=0;i++){
            result[i-sep_pos]=str[i];
        }
        result[new_strlen]=0;
        return result;
    }
    export char* substr_before(const char* str,char separator){
    return substring_before(str,separator,false);
    }
    export char* substr_after(const char* str,char separator){
        return substring_after(str,separator,false);
    }
    export char* substr_before_last(const char* str,char separator){
        return substring_before(str,separator,true);
    }
    export char* substr_after_last(const char* str,char separator){
        return substring_after(str,separator,true);
    }
    export int integer_from_string(const char* str){
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
    export char* write_integer_string(int a){
        int len=0;
        for(int val=a;val!=0;len++){
            val = val/10;
        }
        char* ret = Alloc<char>(len+1);
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
    export float float_from_string(const char* str){
        return static_cast<float>(atof(str));
    }
    export char* write_float_string(float a){
        int before_decimal_digits = 1;
        int after_decimal_digits=8;
        int sign_place = (a<0)? 1:0;
        float a2=(a<0)?-a:a;
        float highest_base_10 =1;
        while(highest_base_10*10 < a2){highest_base_10 *= 10;before_decimal_digits++;}

        char* str = Alloc<char>(sign_place+before_decimal_digits+1+after_decimal_digits+1);
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
    export bool bool_from_string(const char* str){
        if(compare(str,"true"))return true;
        if(compare(str,"false"))return false;
        return false;
    }
    export char* write_bool_string(bool val){
        return new_copy(val?"true":"false");
    }
};

namespace wstr{
    export int len(const wchar* str){
        if(str==nullptr)return 0;
        int i;
        for(i=0;str[i]!=0;i++);
        return i;
    }
    export wchar* allocf(const wchar* format, ...){
        va_list args;
        va_start (args,format);

        int guess_length = len(format)+16+1;
        wchar* ret= Alloc<wchar>(guess_length);
        int real_length = vsnwprintf(ret,guess_length,format,args);

        if(real_length < guess_length)return ret;
        Free(ret);
        ret = Alloc<wchar>(real_length+1);
        vswprintf(ret,format,args);
        va_end (args);
        return ret;
    }
    export wchar* from_cstr(const char* cstr){//truncates high-order bits.
        if(cstr==nullptr)return nullptr;
        int str_len = cstr::len(cstr);
        wchar* str = Alloc<wchar>(str_len+1);
        for(int i=0;i<str_len;i++){str[i]=cstr[i];}
        str[str_len]=0;
        return str;
    }
    export wchar* new_copy(const wchar* str){
        if(str==nullptr)return nullptr;
        int str_len = len(str);
        if(str_len==0)return nullptr;
        wchar* s = Alloc<wchar>(str_len+1);
        wcscpy(s,str);
        s[str_len]=0;
        return s;
    }
    export wchar* lowercase_copy(const wchar* str){
        wchar* s = new_copy(str);
        for(int i=0;s[i]!=0;i++){
            if(s[i] >= 'A' && s[i] <= 'Z')s[i] += ('A'-'a');
        }
        return s;
    }
    export wchar* append(const wchar* str1,const wchar* str2){
        int str1_len = len(str1);
        int str2_len = len(str2);
        wchar* str = Alloc<wchar>(str1_len+str2_len+1);
        wcscpy(str,str1);
        wcscpy(&str[str1_len],str2);
        str[str1_len+str2_len]=0;
        return str;
    }
    export wchar* append(const wchar* str1, wchar seperator, const wchar* str2){
        int str1_len = len(str1);
        int str2_len = len(str2);
        wchar* str = Alloc<wchar>(str1_len+str2_len+2);
        wcscpy(str,str1);
        str[str1_len]=seperator;
        wcscpy(&str[str1_len+1],str2);
        str[str1_len+1+str2_len]=0;
        return str;
    }
    export wchar* append(const wchar* str1,wchar char1){
        int strlen = len(str1);
        wchar* str = Alloc<wchar>(strlen+2);
        if(str1 != nullptr){wcscpy(str,str1);}
        str[strlen]=char1;
        str[strlen+1]=0;
        return str;
    }
    export bool compare(const wchar* str1,const wchar* str2){
        if(str1 == str2)return true;//pointer comparison shortcut
        return wcscmp(str1,str2)==0;
    }
    export bool compare_caseless(const wchar* str1,const wchar* str2){
        if(str1 == str2)return true;//pointer comparison shortcut
        if(str1 == nullptr || str2 == nullptr)return false;
        int indx;
        for(indx=0;str1[indx]==0;indx++){
            wchar char1=str1[indx];
            wchar char2=str2[indx];
            if(char2 == 0)return false;
            if(char1 >= 'A'){char1 -= ('A'-'a');}
            if(char2 >= 'A'){char2 -= ('A'-'a');}
            if(char1 != char2)return false;
        }
        if(str2[indx] != 0)return false;
        return true;
    }
    export bool starts_with(const wchar* str, const wchar* start){
        int i=0;
        for(i=0;str[i] != 0 && start[i]!= 0;i++){
            if(str[i] != start[i])return false;
        }
        if(str[i]==0 && start[i] != 0)return false;
        return true;
    }
    export bool contains(const wchar* str, const wchar* substr){
        int i,j;
        for(i=0;str[i] != 0;i++){
            for(j=0;str[i+j]==substr[j];j++){
                if(substr[j]==0){return true;}
            }
        }
        return false;
    }
    export wchar* substr(const wchar* str,int start,int length){
        wchar* ret = Alloc<wchar>(length+1);
        MemCopy(ret,&str[start],length*sizeof(wchar));
        ret[length]=0;
        return ret;
    }
    export wchar* substring_before(const wchar* str,wchar separator,bool last){
        int sep_pos =0;
        for(int i=0;str[i]!=0;i++){if(str[i]==separator){sep_pos=i+1;if(!last)break;}}
        int new_strlen = sep_pos-1;
        if(new_strlen==0)return nullptr;
        wchar* result= ZeroAlloc<wchar>(new_strlen+1);
        for(int i=0;i<sep_pos;i++){
            result[i]=str[i];
        }
        result[new_strlen]=0;
        return result;
    }
    export wchar* substring_after(const wchar* str,wchar separator,bool last){
        int old_strlen=len(str);
        int sep_pos = old_strlen;
        for(int i=0;str[i]!=0;i++){if(str[i]==separator){sep_pos=i+1;if(!last)break;}}
        int new_strlen = old_strlen-sep_pos;
        if(new_strlen <= 0)return nullptr;
        wchar* result= ZeroAlloc<wchar>(new_strlen+1);
        for(int i=sep_pos;str[i]!=0;i++){
            result[i-sep_pos]=str[i];
        }
        result[new_strlen]=0;
        return result;
    }
    export wchar* substr_before(const wchar* str,wchar separator){
    return substring_before(str,separator,false);
    }
    export wchar* substr_after(const wchar* str,wchar separator){
        return substring_after(str,separator,false);
    }
    export wchar* substr_before_last(const wchar* str,wchar separator){
        return substring_before(str,separator,true);
    }
    export wchar* substr_after_last(const wchar* str,wchar separator){
        return substring_after(str,separator,true);
    }
    export int integer_from_string(const wchar* str){
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
    export wchar* write_integer_string(int a){
        int len=0;
        for(int val=a;val!=0;len++){
            val = val/10;
        }
        wchar* ret = Alloc<wchar>(len+1);
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
    export float float_from_string(const wchar* str){
        return static_cast<float>(_wtof(str));
    }
    export wchar* write_float_string(float a){
        int before_decimal_digits = 1;
        int after_decimal_digits=8;
        int sign_place = (a<0)? 1:0;
        float a2=(a<0)?-a:a;
        float highest_base_10 =1;
        while(highest_base_10*10 < a2){highest_base_10 *= 10;before_decimal_digits++;}

        wchar* str = Alloc<wchar>(sign_place+before_decimal_digits+1+after_decimal_digits+1);
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
    export bool bool_from_string(const wchar* str){
        if(compare(str,L"true"))return true;
        if(compare(str,L"false"))return false;
        return false;
    }
    export wchar* write_bool_string(bool val){
        return new_copy(val?L"true":L"false");
    }
};

//Translates a C-string translation key into a user (wide) string.
export wchar* Translate(const char* msg) {
    //TODO: if(translation lookup key exists) return key[msg];
    return wstr::from_cstr(msg);
}

//Translates a C-string translation key into a user (char) string. Not sure how many ascii translation strings there'll be.
export char* TranslateC(const char* msg){
    //TODO: if(translation lookup key exists) return key[msg];
    return cstr::new_copy(msg);
}

//Translates a C-string translation key into a user (wide) string, then applies Printf formatting to the translated string.
export wchar* TranslateF(char* msg,...){
    va_list args;
    va_start (args,msg);

    wchar* translateMsg = Translate(msg);
    int guess_length = wstr::len(translateMsg)+16+1;
    wchar* ret= Alloc<wchar>(guess_length);


    int real_length = vsnwprintf(ret,guess_length,translateMsg,args);

    if(real_length < guess_length)return ret;
    Free(ret);
    ret = Alloc<wchar>(real_length+1);
    vswprintf(ret,translateMsg,args);
    va_end (args);
    return ret;

    if(real_length < guess_length)return ret;
    Free(ret);
    ret = Alloc<char>(real_length+1);
    sprintf(ret,translateMsg,args);
    va_end (args);
    return ret;

    return wstr::from_cstr(msg);
}