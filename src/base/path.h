#pragma once

#include "base/types.h"
#include <string>
#include <tuple>

namespace base { namespace path {

std::string toDir(const std::string& p);

std::string basename(const std::string& p);

std::string dirname(const std::string& p);

std::tuple<std::string, std::string> split(const std::string& p);

std::tuple<std::string, std::string> splitext(const std::string& p);

std::string normpath(const std::string& p);

inline std::string join(const std::string& a) {
    return a;
}

#if !defined COMPILER_MSVC
template<typename T, typename... Args>
std::string join(const std::string& a, const T& b, Args... p) {
    return join(a + '/' + b, p...);
}
#endif

}}