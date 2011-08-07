/**
 * @file    base/stringmap.h
 * @brief   string map class
 *
 * @author  Alexey Vasilyev <alexa.infra@gmail.com>
 * @section LICENSE
 * 
 * This file is part of Negine (https://alexadotlife.com/hg/negine)
 * Licensed under LGPLv3 (or GPLv3) - see LICENSE file for details
 **/
#pragma once

#include "base/types.h"
#include <map>
#include <functional>
#include <algorithm>

namespace base {

template<class T>
struct map_data_compare : public std::binary_function<typename T::value_type, 
                                                      typename T::mapped_type, 
                                                      bool>
{
public:
    bool operator() (typename T::value_type &pair, 
                     typename T::mapped_type i) const
    {
        return pair.second == i;
    }
};

template<class T, u32 N>
class StringMap
{
public:
    typedef std::map<std::string, T> map_type;

private:
    map_type entries_;
public:
    struct Entry {
        const char* key;
        T value;
    };
    StringMap(const Entry* m) {
        for (u32 i=0; i<N; i++) {
            entries_[std::string(m[i].key)] = m[i].value;
        }
    }
    const char* to_string(const T& value) {
        typename map_type::iterator it = std::find_if(entries_.begin(), entries_.end(),
            std::bind2nd(map_data_compare<map_type>(), value));
        assert(it != entries_.end());
        return it->first.c_str();
    }
    T from_string(const char* key) {
        typename map_type::iterator it = entries_.find(std::string(key));
        assert(it != entries_.end());
        return it->second;
    }
};

}