#pragma once

namespace base
{

class Log
{
public:
    enum Level {
        Info, Warning, Error
    };
    virtual ~Log() {}
    virtual void write(Level l, const char* message) = 0;
};

void openLog(Log* log);
void writeLog(Log::Level level, const char* fmt, ...);
void setLogLevel(Log::Level level);

#define LOG_LEVEL(l, x, ...) ::base::writeLog(l, x, ##__VA_ARGS__)
#define LOG(x, ...) LOG_LEVEL(::base::Log::Info, x, ##__VA_ARGS__)
#define WARN(x, ...) LOG_LEVEL(::base::Log::Warning, x, ##__VA_ARGS__)
#define ERR(x, ...) LOG_LEVEL(::base::Log::Error, x, ##__VA_ARGS__)

class ConsoleLog : public Log
{
public:
    enum Color {
        Default, Yellow, Red
    };
    virtual ~ConsoleLog() {}
    void write(Level l, const char* message);
    void writeColored(Color c, const char* message);
};

}
