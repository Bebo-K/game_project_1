#include "log.h"
#include <iostream>
#include <stdarg.h>


void indent_log(int tabs){
  for(int i=0;i<tabs;i++){
    printf("    ");
  }
}

void log(const char* text, ...){
  va_list args;
  va_start (args,text);
  vprintf (text, args);
  va_end (args);
}