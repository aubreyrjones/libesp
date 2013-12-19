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
		std::atomic_int value;
		
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
		std::atomic_int nextIndex;
	
	public:
		PointerHash() : nextIndex(0) {};
		
		int Put(PT ptr)
		{
			uint64_t searchStart = HashInteger64(ptr) % capacity;
			uint64_t idx = searchStart;
			
			do {
				PT prevKey = nullptr;
				
				if (table[idx].ptr.compare_exchange_weak(prevKey, ptr, std::memory_order_release, std::memory_order_acquire)){ //successful claim
					int index = nextIndex.fetch_add(1);
					table[idx].value.store(index, std::memory_order_release);
					return index;
				}
				else {
					idx = (idx + 1) % capacity;
				}
				
			} while (searchStart != idx);
			
			return -1;
		}
		
		int Get(PT ptr)
		{
			uint64_t searchStart = HashInteger64(ptr) % capacity;
			uint64_t idx = searchStart;
			
			do {
				PT probedKey = table[idx].ptr.load(std::memory_order_acquire);
				if (probedKey == ptr){
					return table[idx].value.load(std::memory_order_acquire);
				}
				else if (probedKey == 0){
					return -1;
				}
				else {
					idx = (idx + 1) % capacity;
				}
				
			} while (searchStart != idx);		
		}
		
		int Lookup(PT ptr)
		{
			int existing = Get(ptr);
			if (existing < 0){
				return Put(ptr);
			}
		}
	};
}
#endif	/* POINTER_HASH_H */

