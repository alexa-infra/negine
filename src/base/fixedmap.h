#pragma once

#include "base/types.h"
#include <map>
#include <string.h> // for strcpy, strcmp
#include <string> // for std::hash( const char* )
#include "base/debug.h"

namespace base
{

class SmallString
{
public:
	inline SmallString() {
		buffer_[0] = '\0';
	}
	inline SmallString(const char* s) {
		ASSERT(strlen(s) < 20);
		strcpy(buffer_, s);
	}
	inline SmallString(const SmallString& s) {
		strcpy(buffer_, s.buffer_);
	}
	inline SmallString& operator=(const SmallString& s) {
		strcpy(buffer_, s.buffer_);
		return *this;
	}
	inline bool operator==(const SmallString& s) const {
		return (strcmp(buffer_, s.buffer_) == 0);
	}
	inline bool operator<(const SmallString& s) const {
		return (strcmp(buffer_, s.buffer_) < 0);
	}
	inline const char* c_str() const {
		return buffer_;
	}
private:
	char buffer_[20];
};

class HashString
{
public:
	inline HashString() {
		hash_ = 0;
	}
	inline HashString(const char* s) {
		std::hash<std::string> h;
		hash_ = h(s);
	}
	inline bool operator==(const HashString& s) const {
		return hash_ == s.hash_;
	}
	inline bool operator<(const HashString& s) const {
		return hash_ < s.hash_;
	}
	inline const char* c_str() const {
		return "";
	}
private:
	std::size_t hash_;
};

template<class String, typename T>
class FixedMap
{
	typedef std::map<String, T> MapT;
	typedef typename MapT::const_iterator IteratorT;
	typedef FixedMap<String, T> SelfT;
public:
	T& operator[](const char* name) {
		return map_[name];
	}
	const T& get(const String& name, const T& defaultValue) const {
		IteratorT it = map_.find(name);
		if (it == map_.end())
			return defaultValue;
		return it->second;
	}
	bool tryGet(const String& name, T& value) const {
		IteratorT it = map_.find(name);
		if (it == map_.end())
			return false;
		value = it->second;
		return true;
	}
	class Iterator
	{
	public:
		Iterator(const MapT& m) : Map(m), it(m.begin()) {
		}
		void advance() {
			++it;
		}
		bool isDone() const {
			return it == Map.end();
		}
		const String& key() {
			return it->first;
		}
		const T& value() const {
			return it->second;
		}
		const char* name() const {
			return it->first.c_str();
		}
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