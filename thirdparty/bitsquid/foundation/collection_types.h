#pragma once

#include "foundation/types.h"
#include "foundation/memory_types.h"
#include <type_traits>

/// All collection types assume that they are used to store POD objects. I.e. they:
///
/// * Don't call constructors and destructors on elements.
/// * Move elements with memmove().
///
/// If you want to store items that are not PODs, use something other than these collection
/// classes.
namespace foundation
{
	/// Dynamically resizable array of POD objects.
	template<typename T> struct Array
	{
		static_assert(std::is_pod<T>::value, "only POD objects!");
		Array(Allocator &a);
		~Array();
		Array(const Array &other);
		Array &operator=(const Array &other);
		
		T &operator[](uint32_t i);
		const T &operator[](uint32_t i) const;

		Allocator *_allocator;
		uint32_t _size;
		uint32_t _capacity;
		T *_data;
	};

	/// A double-ended queue/ring buffer.
	template <typename T> struct Queue
	{
		static_assert(std::is_pod<T>::value, "only POD objects!");
		Queue(Allocator &a);

		T &operator[](uint32_t i);
		const T &operator[](uint32_t i) const;

		Array<T> _data;
		uint32_t _size;
		uint32_t _offset;
	};

	/// Hash from an uint64_t to POD objects. If you want to use a generic key
	/// object, use a hash function to map that object to an uint64_t.
	template<typename T> struct Hash
	{
		static_assert(std::is_pod<T>::value, "only POD objects!");
	public:
		Hash(Allocator &a);
		
		struct Entry {
			uint64_t key;
			uint32_t next;
			T value;
		};

		Array<uint32_t> _hash;
		Array<Entry> _data;
	};
}