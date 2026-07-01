export module Logger;
import Config;
import <iostream>;
import <stdarg>;
import <stdexcept>;

#define MAX_ERRSPRINTF_LENGTH 256

FILE* logfile = nullptr;


namespace logger {
  export void start(const char* filename){
    logfile = fopen(filename,"w");
    setbuf(stdout, NULL);
  }

  export void info(const char* text,...){
    va_list args;
    va_start (args,text);
    vprintf(text, args);
    vfprintf(logfile,text,args);
    fflush(logfile);
    va_end (args);
    fflush(stdout);
  }
  export void infoW(const wchar* text,...){
    va_list args;
    va_start (args,text);
    vwprintf(text, args);
    vfwprintf(logfile,text,args);
    fflush(logfile);
    va_end (args);
    fflush(stdout);
  }

  export void debug(const char* text,...){
    if(config::debug_mode != 0){
      va_list args;
      va_start (args,text);
      vprintf(text, args);
      vfprintf(logfile,text,args);
      fflush(logfile);
      va_end (args);
      fflush(stdout);
    }
  }

  export void debugW(const wchar* text,...){
    if(config::debug_mode != 0){
      va_list args;
      va_start (args,text);
      vwprintf(text, args);
      vfwprintf(logfile,text,args);
      fflush(logfile);
      va_end (args);
      fflush(stdout);
    }
  }

  export void warn(const char* text,...){
    va_list args;
    va_start (args,text);
    vprintf (text, args);
    vfprintf(logfile,text,args);
    fflush(stderr);
    fflush(logfile);
    //Raise warning flag
    va_end (args);
  }
  export void warnW(const wchar* text,...){
    va_list args;
    va_start (args,text);
    vwprintf (text, args);
    vfwprintf(logfile,text,args);
    fflush(stderr);
    fflush(logfile);
    //Raise warning flag
    va_end (args);
  }

  export void exception(const char* text,...){
    va_list args;
    va_start (args,text);
    vprintf (text, args);
    vfprintf(logfile,text,args);
    vfprintf(stderr,text,args);
    fflush(stderr);
    fflush(logfile);

    char* exception_string = (char*)malloc(MAX_ERRSPRINTF_LENGTH);
    snprintf(exception_string,MAX_ERRSPRINTF_LENGTH,text,args);
    throw std::runtime_error(exception_string);
    free(exception_string);
    va_end (args);
  }

  export void exceptionW(const wchar* text,...){
    va_list args;
    va_start (args,text);
    vwprintf (text, args);
    vfwprintf(logfile,text,args);
    vfwprintf(stderr,text,args);
    fflush(stderr);
    fflush(logfile);

    wchar* exception_string = (wchar*)malloc(MAX_ERRSPRINTF_LENGTH*sizeof(wchar));
    snwprintf(exception_string,MAX_ERRSPRINTF_LENGTH,text,args);
    char* exception_string_cstr = cstr::from_wstr(exception_string);
    throw std::runtime_error(exception_string_cstr);
    free(exception_string);
    free(exception_string_cstr);
    va_end (args);
  }

  export void flush(){
    fflush(stderr);
    fflush(logfile);
  }

  export void fatal(const char* text,...){
    va_list args;
    va_start (args,text);
    vprintf (text, args);
    vfprintf(logfile,text,args);
    va_end (args);
    fflush(stderr);
    fflush(logfile);
    exit(1);
  }
}