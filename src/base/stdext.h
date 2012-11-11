#pragma once

#include <map>

template<typename T>
bool map_contains(const std::map<
	typename T::key_type,
	typename T::mapped_type,
	typename T::key_compare,
	typename T::allocator_type>& mmap, const typename T::key_type& key)
{
	return mmap.count(key) == 1;
}

template<typename T>
bool try_find(const std::map<
	typename T::key_type,
	typename T::mapped_type,
	typename T::key_compare,
	typename T::allocator_type>& mmap,
	const typename T::key_type& key,
	typename T::mapped_type& value
	)
{
	typename T::const_iterator found = mmap.find(key);
	if (found == mmap.end())
		return false;
	value = found->second;
	return true;
}