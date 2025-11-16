#ifndef LOG_H
#define LOG_H

#include <game_project_1/types/str.hpp>

namespace logger{
void start(const char* text);
void info(const char* text, ...);
void infoW(const wchar* text,...);
void debug(const char* text, ...);
void debugW(const wchar* text,...);
void warn(const char* text,...);
void warnW(const wchar* text,...);
void exception(const char* text,...);
void exceptionW(const wchar* text,...);
void flush();
void fatal(const char* text,...);

};

#endif