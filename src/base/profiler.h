#pragma once

#include "base/singleton.h"
#include "base/timer.h"
#include <map>
#include <string>
#include <ostream>

namespace base {

class Profiler : public Singleton<Profiler>
{
private:
    f32& _counter(const std::string& name);
    void _clear();
    void _reset();
    void _report(std::ostream* out);
    void _reportHeader(std::ostream* out);
public:
    static f32& counter(const std::string& name) {
        return instance()._counter(name);
    }
    static void clear() {
        instance()._clear();
    }
    static void reset() {
        instance().reset();
    }
    static void report(std::ostream* out) {
        instance().report(out);
    }
    static void reportHeader(std::ostream* out) {
        instance().reportHeader(out);
    }
private:
    typedef std::map<std::string, f32> Map;
    Map counters_;
};

class ProfilerScope
{
public:
    ProfilerScope(const std::string& name);
    ~ProfilerScope();
    void stop();
    void start();
    void toggle();
private:
    bool running_;
    Timer timer_;
    std::string name_;
};

#define PROFILER_SCOPE(name) ProfilerScope scope ## __LINE__(name)

} // namespace base