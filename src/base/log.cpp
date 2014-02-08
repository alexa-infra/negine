#include "base/log.h"
#include "base/env.h"
#include <stdio.h>
#include <stdarg.h>
#include <vector>

#ifdef OS_WIN
#include <windows.h>
#include "foundation/snprintf_msvc.h"
#endif

namespace base {

#define LOG_BUFFER_SIZE 1024
char logBuffer_[LOG_BUFFER_SIZE];

struct LogWrapper
{
    std::vector<Log*> loggers_;
    Log::Level level_;
    LogWrapper()
    {
        level_ = Log::LEVEL_INFO;
        openLog(new ConsoleLog());
    }
    ~LogWrapper()
    {
        size_t len = loggers_.size();
        for (size_t i=0; i<len; i++)
            delete loggers_[i];
    }
} logWrapper;

void openLog(Log* logger)
{
    logWrapper.loggers_.push_back(logger);
}

void writeLog(Log::Level level, const char* fmt, ...)
{
    va_list args;
    va_start(args, fmt);
    if (level < logWrapper.level_)
        return;
    vsnprintf(logBuffer_, LOG_BUFFER_SIZE, fmt, args);
    size_t len = logWrapper.loggers_.size();
    for (size_t i=0; i<len; i++)
         logWrapper.loggers_[i]->write(level, logBuffer_);
    va_end(args);
}

void setLogLevel(Log::Level level)
{
    logWrapper.level_ = level;
}

ConsoleLog::ConsoleLog()
{
#ifdef OS_WIN
    useColor_ = true;
#else
    std::string term = env::variable("TERM", "");
    useColor_ =
        term == "xterm" || term == "xterm-color" ||
        term == "xterm-256color" || term == "screen" ||
        term == "linux";
#endif
}
    
void ConsoleLog::write(Log::Level l, const char* message)
{
    if (l == Log::LEVEL_INFO)
        writeColored(ConsoleLog::COLOR_DEFAULT, message);
    else if (l == Log::LEVEL_WARNING)
        writeColored(ConsoleLog::COLOR_YELLOW, message);
    else if (l == Log::LEVEL_ERROR)
        writeColored(ConsoleLog::COLOR_RED, message);
    else
        writeColored(ConsoleLog::COLOR_DEFAULT, message);
}

#ifdef OS_WIN
WORD GetColorAttribute(ConsoleLog::Color color)
{
    switch(color) {
        case ConsoleLog::COLOR_RED: return FOREGROUND_RED;
        case ConsoleLog::COLOR_YELLOW: return FOREGROUND_RED | FOREGROUND_GREEN;
        case ConsoleLog::COLOR_DEFAULT:
        default: return 0;
    }
}
#else
const char* GetColorAttribute(ConsoleLog::Color color)
{
    switch(color) {
        case ConsoleLog::COLOR_RED: return "1";
        case ConsoleLog::COLOR_YELLOW: return "3";
        case ConsoleLog::COLOR_DEFAULT:
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
    if (!useColor_ || color == ConsoleLog::COLOR_DEFAULT) {
        printf("%s\n", message);
    } else {
        printf("\033[0;3%sm", GetColorAttribute(color));
        printf("%s", message);
        printf("\033[m\n");
    }
#endif
}

}
