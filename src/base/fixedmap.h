#pragma once

#include "base/types.h"
#include <map>

namespace base
{

template<class String, typename T>
class FixedMap
{
    typedef std::map<String, T> MapT;
    typedef typename MapT::const_iterator IteratorT;
    typedef FixedMap<String, T> SelfT;
public:
    T& operator[](const String& name) {
        return map_[name];
    }
    const T& get(const String& name, const T& defaultValue) const {
        IteratorT it = map_.find(name);
        if (it == map_.end())
            return defaultValue;
        return it->second;
    }
    bool contains(const String& name) const {
        IteratorT it = map_.find(name);
        return (it != map_.end());
    }
    bool tryGet(const String& name, T*& value) const {
        IteratorT it = map_.find(name);
        if (it == map_.end())
            return false;
        const T& v = it->second;
        value = const_cast<T*>(&v);
        return true;
    }
    size_t size() const {
        return map_.size();
    }
    class Iterator
    {
    public:
        Iterator(const MapT& m) : Map(m), it(m.begin()) {}
        void advance() { ++it; }
        bool isDone() const { return it == Map.end(); }

        const String& key() { return it->first; }
        const T& value() const { return it->second; }
        const char* name() const { return it->first.c_str(); }

    private:
        IteratorT it;
        const MapT& Map;
    };
    Iterator iterator() const {
        return Iterator(map_);
    }
    void merge(const SelfT& other) {
        for (Iterator it = other.iterator(); !it.isDone(); it.advance())
            (*this)[it.name()] = it.value();
    }
    void clear() {
        map_.clear();
    }
private:
    MapT map_;
};

}
