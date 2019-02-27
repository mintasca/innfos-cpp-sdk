#ifndef LOGFUNC_H
#define LOGFUNC_H
#include <iostream>
using namespace std;

namespace LogFunc {
    void loadConfig(const std::string& name);
    void shutdown();
    void emerg(const std::string& name,const char* stringFormat, ...);
    void info(const std::string& name,const char* stringFormat, ...);
    void debug(const std::string& name,const char* stringFormat, ...);
    void crit(const std::string& name,const char* stringFormat, ...);
    void error(const std::string& name,const char* stringFormat, ...);
    void fatal(const std::string& name,const char* stringFormat, ...);
    void notice(const std::string& name,const char* stringFormat, ...);
    void warn(const std::string& name,const char* stringFormat, ...);
    char*   log_Time();
}

#endif // LOGFUNC_H
