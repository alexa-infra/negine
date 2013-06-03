#pragma once

#include "base/singleton.h"
#include "base/timer.h"
#include <map>
#include <ostream>

namespace base {

class Profiler : public Singleton<Profiler>
{
public:
    f32& counter(const std::string& name);

    void clear();

    void reset();

    void reportHeader(std::ostream* out);

    void report(std::ostream* out);

private:
    typedef std::map<std::string, f32> Map;
    Map counters_;
};

class ProfilerScope
{
public:
    ProfilerScope(const std::string& name, Profiler* profiler);
    
    ~ProfilerScope();
    
    void stop();
    
    void start();
    
    void toggle();

private:
    bool running_;
    f32& counter_;
    Timer timer_;
};

#define PROFILE_SCOPE(name) ProfilerScope scope ## __LINE__(name, Profiler::instance())

} // namespace base