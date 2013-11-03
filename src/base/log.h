#pragma once

#include "base/types.h"

namespace base
{

class NEGINE_API Log
{
public:
    enum Level {
        LEVEL_INFO, LEVEL_WARNING, LEVEL_ERROR
    };
    virtual ~Log() {}
    virtual void write(Level l, const char* message) = 0;
};

NEGINE_API void openLog(Log* log);
NEGINE_API void writeLog(Log::Level level, const char* fmt, ...);
NEGINE_API void setLogLevel(Log::Level level);

#define LOG_LEVEL(level, fmt, ...) ::base::writeLog(level, fmt, ##__VA_ARGS__)
#define LOG(fmt, ...)     LOG_LEVEL(::base::Log::LEVEL_INFO, fmt, ##__VA_ARGS__)
#define WARN(fmt, ...)    LOG_LEVEL(::base::Log::LEVEL_WARNING, fmt, ##__VA_ARGS__)
#define ERR(fmt, ...)     LOG_LEVEL(::base::Log::LEVEL_ERROR, fmt, ##__VA_ARGS__)

class NEGINE_API ConsoleLog : public Log
{
public:
    enum Color {
        COLOR_DEFAULT, COLOR_YELLOW, COLOR_RED
    };
    ConsoleLog();
    virtual ~ConsoleLog() {}
    void write(Level level, const char* message);
    void writeColored(Color color, const char* message);
private:
    bool useColor_;
};

}
