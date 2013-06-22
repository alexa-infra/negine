#include "base/path.h"
#include <vector>

namespace base { namespace path {

std::string toDir(const std::string& p) {
    size_t pos = p.rfind('/');
    if (pos == std::string::npos)
        return p + '/';
    if (pos != p.size() - 1)
        return p + '/';
    return p;
}

std::string basename(const std::string& p) {
    size_t pos = p.rfind('/');
    if (pos != std::string::npos)
        return p.substr(pos + 1);
    return p;
}

std::string dirname(const std::string& p) {
    size_t pos = p.rfind('/');
    if (pos != std::string::npos)
        return p.substr(0, pos);
    return p;
}

std::tuple<std::string, std::string> split(const std::string& p) {
    size_t pos = p.rfind('/');
    if (pos != std::string::npos)
        return std::make_tuple(p.substr(0, pos), p.substr(pos + 1));
    return std::make_tuple("", p);
}

std::tuple<std::string, std::string> splitext(const std::string& p) {
    size_t pos = p.rfind('.');
    if (pos != std::string::npos)
        return std::make_tuple(p.substr(0, pos), p.substr(pos));
    return std::make_tuple(p, "");
}

std::string normpath(const std::string& p) {
    std::vector<char> ret;
    ret.reserve(p.size());
    for(size_t i=0; i<p.size(); i++) {
        char ch = p[i];
        if (ch == '\\') ch = '/';
        if (i > 0 && ch == '/' && ret.back() == '/')
            continue;
        ret.push_back(ch);
    }
    return std::string(ret.begin(), ret.end());
}

}}