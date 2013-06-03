#include "base/profiler.h"
#include <algorithm>
#include <iterator>

namespace base {

f32& Profiler::counter(const std::string& name)
{
    return counters_[name];
}

void Profiler::clear()
{
    counters_.clear();
}

void Profiler::reset()
{
    std::for_each(counters_.begin(), counters_.end(), 
        [](Map::value_type& p) { p.second = 0.0f; });
}

void Profiler::reportHeader(std::ostream* out)
{
    auto mapFirst = [](const Map::value_type& p){ return p.first; };
    std::transform(counters_.begin(), std::prev(counters_.end()),
        std::ostream_iterator<std::string>(*out, ","),
        mapFirst);
    if (!counters_.empty())
        *out << mapFirst(*std::prev(counters_.end())) << std::endl;
}

void Profiler::report(std::ostream* out)
{
    auto mapSecond = [](const Map::value_type& p){ return p.second; };
    std::transform(counters_.begin(), std::prev(counters_.end()),
        std::ostream_iterator<f32>(*out, ","),
        mapSecond);
    if (!counters_.empty())
        *out << mapSecond(*std::prev(counters_.end())) << std::endl;
}

ProfilerScope::ProfilerScope(const std::string& name, Profiler* profiler)
    : counter_(profiler->counter(name))
{
    running_ = true;
}

ProfilerScope::~ProfilerScope()
{
    stop();
}

void ProfilerScope::stop()
{
    if (running_) {
        counter_ += timer_.reset();
        running_ = false;
    }
}

void ProfilerScope::start()
{
    if (!running_) {
        timer_.reset();
        running_ = true;
    }
}

void ProfilerScope::toggle()
{
    if (running_)
        stop();
    else
        start();
}


} // namespace base