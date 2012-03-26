/**
 * \file
 * \brief       string map class
 * \author      Alexey Vasilyev <alexa.infra@gmail.com>
 * \copyright   MIT License
 **/
#pragma once

#include "base/types.h"
#include <unordered_map>
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
    typedef std::unordered_map<std::string, T, hash_string> map_type;

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
    bool to_string(const T& value, std::string& ret) {
        typename map_type::iterator it = std::find_if(entries_.begin(), entries_.end(),
            std::bind2nd(map_data_compare<map_type>(), value));
        if (it == entries_.end())
            return false;
        ret = it->first;
        return true;
    }
    bool from_string(const std::string& key, T& ret) {
        typename map_type::iterator it = entries_.find(key);
        if (it == entries_.end())
            return false;
        ret = it->second;
        return true;
    }
};

}