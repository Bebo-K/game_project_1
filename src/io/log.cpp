#include "log.h"
#include <iostream>
#include <stdarg.h>

#define MAX_ERRSPRINTF_LENGTH 256



void WriteLog(const char* text, ...){
  va_list args;
  va_start (args,text);
  vprintf (text, args);
  printf("\n");
  va_end (args);
}

void logger::info(const char* text,...){
  va_list args;
  va_start (args,text);
  WriteLog(text,args);
  va_end (args);
}

void logger::warn(const char* text,...){
  va_list args;
  va_start (args,text);
  WriteLog(text,args);
  //Raise warning flag
  va_end (args);
}

void logger::exception(const char* text,...){
  va_list args;
  va_start (args,text);
  char* exception_string = (char*)malloc(MAX_ERRSPRINTF_LENGTH);
  snprintf(exception_string,MAX_ERRSPRINTF_LENGTH,text,args);
  throw std::runtime_error(exception_string);
  free(exception_string);
  va_end (args);
}

void logger::fatal(const char* text,...){
  va_list args;
  va_start (args,text);
  WriteLog(text,args);
  va_end (args);
  exit(1);
}
