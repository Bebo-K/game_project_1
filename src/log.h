#ifndef LOG_H
#define LOG_H

#include <stdexcept>

namespace logger{
void start(const char* text);
void info(const char* text, ...);
void debug(const char* text, ...);
void warn(const char* text,...);
void exception(const char* text,...);
void flush();
void fatal(const char* text,...);

};

#endif