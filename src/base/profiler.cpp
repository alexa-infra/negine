#include "base/profiler.h"
#include <algorithm>
#include <iterator>

namespace base {

Profiler* Profiler::instance_ = nullptr;

f32& Profiler::_counter(const std::string& name)
{
    return counters_[name];
}

void Profiler::_clear()
{
    counters_.clear();
}

void Profiler::_reset()
{
    std::for_each(counters_.begin(), counters_.end(), 
        [](Map::value_type& p) { p.second = 0.0f; });
}

void Profiler::_reportHeader(std::ostream* out)
{
    auto mapFirst = [](const Map::value_type& p){ return p.first; };
    std::transform(counters_.begin(), std::prev(counters_.end()),
        std::ostream_iterator<std::string>(*out, ","),
        mapFirst);
    if (!counters_.empty())
        *out << mapFirst(*std::prev(counters_.end())) << std::endl;
}

void Profiler::_report(std::ostream* out)
{
    auto mapSecond = [](const Map::value_type& p){ return p.second; };
    std::transform(counters_.begin(), std::prev(counters_.end()),
        std::ostream_iterator<f32>(*out, ","),
        mapSecond);
    if (!counters_.empty())
        *out << mapSecond(*std::prev(counters_.end())) << std::endl;
}

ProfilerScope::ProfilerScope(const std::string& name)
: name_(name)
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
        Profiler::counter(name_) += timer_.reset();
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