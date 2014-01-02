#pragma once

#include "memory.h"

namespace foundation
{
	/// A temporary memory allocator that primarily allocates memory from a
	/// local stack buffer of size BUFFER_SIZE. If that memory is exhausted it will
	/// use the backing allocator (typically a scratch allocator).
	///
	/// Memory allocated with a TempAllocator does not have to be deallocated. It is
	/// automatically deallocated when the TempAllocator is destroyed.
	template <int BUFFER_SIZE>
	class TempAllocator : public Allocator
	{
	public:
		/// Creates a new temporary allocator using the specified backing allocator.
		TempAllocator(Allocator &backing = memory_globals::default_scratch_allocator());
		virtual ~TempAllocator();

		virtual void *allocate(uint32_t size, uint32_t align = DEFAULT_ALIGN);
		
		/// Deallocation is a NOP for the TempAllocator. The memory is automatically
		/// deallocated when the TempAllocator is destroyed.
		virtual void deallocate(void *) {}

		/// Returns SIZE_NOT_TRACKED.
		virtual uint32_t allocated_size(void *) {return SIZE_NOT_TRACKED;}

		/// Returns SIZE_NOT_TRACKED.
		virtual uint32_t total_allocated() {return SIZE_NOT_TRACKED;}

	private:
		char _buffer[BUFFER_SIZE];	//< Local stack buffer for allocations.
		Allocator &_backing;		//< Backing allocator if local memory is exhausted.
		char *_start;				//< Start of current allocation region
		char *_p;					//< Current allocation pointer.
		char *_end;					//< End of current allocation region
		unsigned _chunk_size;		//< Chunks to allocate from backing allocator
	};

	// If possible, use one of these predefined sizes for the TempAllocator to avoid
	// unnecessary template instantiation.
	typedef TempAllocator<64> TempAllocator64;
	typedef TempAllocator<128> TempAllocator128;
	typedef TempAllocator<256> TempAllocator256;
	typedef TempAllocator<512> TempAllocator512;
	typedef TempAllocator<1024> TempAllocator1024;
	typedef TempAllocator<2048> TempAllocator2048;
	typedef TempAllocator<4096> TempAllocator4096;

	// ---------------------------------------------------------------
	// Inline function implementations
	// ---------------------------------------------------------------

	template <int BUFFER_SIZE>
	TempAllocator<BUFFER_SIZE>::TempAllocator(Allocator &backing) : _backing(backing), _chunk_size(4*1024)
	{
		_p = _start = _buffer;
		_end = _start + BUFFER_SIZE;
		*(void **)_start = 0;
		_p += sizeof(void *);
	}

	template <int BUFFER_SIZE>
	TempAllocator<BUFFER_SIZE>::~TempAllocator()
	{
		void *p = *(void **)_buffer;
		while (p) {
			void *next = *(void **)p;
			_backing.deallocate(p);
			p = next;
		}
	}

	template <int BUFFER_SIZE>
	void *TempAllocator<BUFFER_SIZE>::allocate(uint32_t size, uint32_t align)
	{
		_p = (char *)memory::align_forward(_p, align);
		if ((int)size > _end - _p) {
			uint32_t to_allocate = sizeof(void *) + size + align;
			if (to_allocate < _chunk_size)
				to_allocate = _chunk_size;
			_chunk_size *= 2;
			void *p = _backing.allocate(to_allocate);
			*(void **)_start = p;
			_p = _start = (char *)p;
			_end = _start + to_allocate;
			*(void **)_start = 0;
			_p += sizeof(void *);
			_p = (char *)memory::align_forward(_p, align);
		}
		void *result = _p;
		_p += size;
		return result;
	}
}