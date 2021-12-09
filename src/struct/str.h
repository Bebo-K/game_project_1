#ifndef STR_H
#define STR_H


//String and wide string utilites that should exist

typedef wchar_t wchar;


namespace cstr{
    int len(const char* str);
    char* from_wstr(const wchar* widestr);
    char* from_wstr_utf16(const wchar* widestr);
    char* new_copy(const char* str);
    char* lowercase_copy(const char* str);
    char* append(const char* str1, const char* str2);
    char* append(const char* str1, char seperator, const char* str2);
    bool compare(const char* str1, const char* str2);
    bool starts_with(const char* str, const char* start);
    bool contains(const char* str, const char* substr);
    char* substr_after(const char* str,char separator);//returns the substring after the first occurance of the separator char
    char* substr_before(const char* str,char separator);//returns the substring after the first occurance of the separator char
    char* substr_after_last(const char* str,char separator);//returns the substring after the last occurance of the separator char
    char* substr_before_last(const char* str,char separator);//returns the substring after the last occurance of the separator char
    int integer_from_string(const char* str);
    char* write_integer_string(int a);
    float float_from_string(const char* str);
    char* write_float_string(float a);
    bool bool_from_string(const char* str);
    char* write_bool_string(bool a);  
};

namespace wstr{
    int len(wchar* str);
    wchar* from_cstr(char* str);
    wchar* new_copy(const wchar* old_string);
    wchar* lowercase_copy(const wchar* old_string);
    wchar* append(const wchar* str1, const wchar* str2);
    wchar* append(const wchar* str1, wchar seperator, const wchar* str2);
    bool compare(const wchar* str1, const wchar* str2);
    bool starts_with(const wchar* str, const wchar* start);
    bool contains(const wchar* str, const wchar* substr);
    wchar* substr_after(wchar* str,wchar separator);
    wchar* substr_before(wchar* str,wchar separator);
    wchar* substr_after_last(wchar* str,wchar separator);
    wchar* substr_before_last(wchar* str,wchar separator);
    int integer_from_string(wchar* str);
    wchar* write_integer_string(int a);
    float float_from_string(wchar* str);
    wchar* write_float_string(float a);
    bool bool_from_string(wchar* str);
    wchar* write_bool_string(bool a); 
};












#endif