/* 
 * File:   pointer_hash.h
 * Author: aubrey
 *
 * Created on December 19, 2013, 11:30 AM
 */

#include <atomic>
#include <stdint.h>

#ifndef POINTER_HASH_H
#define	POINTER_HASH_H

namespace devious
{
	
	template <typename PT>
	inline uint64_t HashInteger64(PT pt)
	{
		uint64_t h = (uint64_t) pt;
		h ^= h >> 33;
		h *= 0xff51afd7ed558ccd;
		h ^= h >> 33;
		h *= 0xc4ceb9fe1a85ec53;
		h ^= h >> 33;
		
		return h;
	}

	template <typename PT>
	struct PointerHashEntry
	{
		std::atomic<PT> ptr;
		std::atomic<uint_fast32_t> value;
		
		PointerHashEntry()
		{
			ptr = nullptr;
			value = 0;
		};
	};
	

	template <typename PT, int capacity>
	class PointerHash
	{
	private:
		PointerHashEntry<PT> table[capacity];
		std::atomic<uint_fast32_t> nextIndex;
	
	public:
		PointerHash() : nextIndex(0) {};
		
		uint32_t Put(PT ptr)
		{
			uint64_t searchStart = HashInteger64(ptr) % capacity;
			uint64_t idx = searchStart;
			
			do {
				PT prevKey = nullptr;
				
				prevKey = table[idx].ptr.load(std::memory_order_relaxed);
				
				if (prevKey == ptr){ //already found it, and it already exists, so bail
					return -1;
				}
				else if (prevKey != nullptr){ //already taken by some other pointer, so keep looking.
					goto next_step;
				}
				
				if (table[idx].ptr.compare_exchange_weak(prevKey, ptr, std::memory_order_release, std::memory_order_acquire)){ //successful claim
					int index = nextIndex.fetch_add(1);
					table[idx].value.store(index, std::memory_order_release);
					return index;
				}
				else { //something stole it from us, so keep looking.
					goto next_step;
				}
				
next_step:
				idx = (idx + 1) % capacity;
				
			} while (searchStart != idx);
			
			return -1;
		}
		
		uint32_t Get(PT ptr)
		{
			uint64_t searchStart = HashInteger64(ptr) % capacity;
			uint64_t idx = searchStart;
			
			do {
				PT probedKey = table[idx].ptr.load(std::memory_order_acquire);
				if (probedKey == ptr){
					return table[idx].value.load(std::memory_order_acquire);
				}
				else if (!probedKey){
					return -1;
				}
				else {
					idx = (idx + 1) % capacity;
				}
				
			} while (searchStart != idx);
			
			return -1;
		}
		
		uint32_t Lookup(PT ptr)
		{
			int existing = Get(ptr);
			if (existing < 0){
				return Put(ptr);
			}
			return existing;
		}
	};
}
#endif	/* POINTER_HASH_H */

