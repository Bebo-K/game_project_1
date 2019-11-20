#include "log.h"
#include <iostream>
#include <stdarg.h>

void log(const char* text, ...){
  va_list args;
  va_start (args,text);
  vprintf (text, args);
  va_end (args);
}