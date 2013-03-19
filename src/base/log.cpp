#include "base/log.h"
#include <cstdarg>
#include <vector>
#include "base/types.h"
#include "base/env.h"

#ifdef OS_WIN
#include <windows.h>
#endif

namespace base {

#define LOG_BUFFER_SIZE 1024
char _logBuffer[LOG_BUFFER_SIZE];

struct LogWrapper
{
    std::vector<Log*> loggers;
    Log::Level level;
    LogWrapper()
    {
        level = Log::Info;
        openLog(new ConsoleLog());
    }
    ~LogWrapper()
    {
        u32 len = loggers.size();
        for (u32 i=0; i<len; i++)
            delete loggers[i];
    }
} _logWrapper;

void openLog(Log* log)
{
    _logWrapper.loggers.push_back(log);
}

void writeLog(Log::Level level, const char* fmt, ...)
{
    va_list args;
    va_start(args, fmt);
    if (level < _logWrapper.level)
        return;
    vsnprintf(_logBuffer, LOG_BUFFER_SIZE, fmt, args);
    u32 len = _logWrapper.loggers.size();
    for (u32 i=0; i<len; i++)
         _logWrapper.loggers[i]->write(level, _logBuffer);
    va_end(args);
}

void setLogLevel(Log::Level level)
{
    _logWrapper.level = level;
}

ConsoleLog::ConsoleLog()
{
#ifdef OS_WIN
    useColor = true;
#else
    std::string term = Env::variable("TERM", "");
    useColor =
        term == "xterm" || term == "xterm-color" ||
        term == "xterm-256color" || term == "screen" ||
        term == "linux";
#endif
}
    
void ConsoleLog::write(Log::Level l, const char* message)
{
    if (l == Log::Info) writeColored(ConsoleLog::Default, message);
    else if (l == Log::Warning) writeColored(ConsoleLog::Yellow, message);
    else if (l == Log::Error) writeColored(ConsoleLog::Red, message);
    else writeColored(ConsoleLog::Default, message);
}

#ifdef OS_WIN
WORD GetColorAttribute(ConsoleLog::Color color)
{
    switch(color) {
        case ConsoleLog::Red: return FOREGROUND_RED;
        case ConsoleLog::Yellow: return FOREGROUND_RED | FOREGROUND_GREEN;
        case ConsoleLog::Default:
        default: return 0;
    }
}
#else
const char* GetColorAttribute(ConsoleLog::Color color)
{
    switch(color) {
        case ConsoleLog::Red: return "1";
        case ConsoleLog::Yellow: return "3";
        case ConsoleLog::Default:
        default: return NULL;
    }
}
#endif

void ConsoleLog::writeColored(ConsoleLog::Color color, const char* message)
{
#ifdef OS_WIN
    const HANDLE stdoutHandle = GetStdHandle(STD_OUTPUT_HANDLE);
  
    CONSOLE_SCREEN_BUFFER_INFO bufferInfo;
    GetConsoleScreenBufferInfo(stdoutHandle, &bufferInfo);
    const WORD oldColorAttrs = bufferInfo.wAttributes;
  
    fflush(stdout);
    SetConsoleTextAttribute(stdoutHandle,
                            GetColorAttribute(color) | FOREGROUND_INTENSITY);
    fprintf(stdout, "%s\n", message);
  
    fflush(stdout);
    SetConsoleTextAttribute(stdoutHandle, oldColorAttrs);
#else
    if (!useColor || color == ConsoleLog::Default) {
        printf("%s\n", message);
    } else {
        printf("\033[0;3%sm", GetColorAttribute(color));
        printf("%s", message);
        printf("\033[m\n");
    }
#endif
}

}
