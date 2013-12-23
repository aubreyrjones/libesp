/* 
 * File:   lockless_ring.h
 * Author: netzapper
 *
 * Created on December 16, 2013, 10:02 PM
 */
#include <atomic>
#include <string.h>
#ifndef LOCKLESS_RING_H
#define	LOCKLESS_RING_H

namespace devious {

	template <typename T, int capacity>
	class LockessRingQueue 
	{
	private:
		T buffer[capacity + 1];
		std::atomic<int> back, front, readfront;
		
		inline int modcap(int i)
		{
			return i % capacity;
		}
		
		inline bool modcap_eq(int a, int b)
		{
			return modcap(a) == modcap(b);
		}
	public:

		LockessRingQueue() : back(0), front(0), readfront(0) {
		};

		
		
		/**
		 * Is the queue empty?
         * @return 
         */
		bool Empty() 
		{
			int f = front;
			int b = back.load(std::memory_order_acquire);
			return modcap_eq(f, b);
		}
		
		/**
		 * Is the queue full?
         * @return 
         */
		bool Full() 
		{
			int f = front;
			int b = back.load(std::memory_order_acquire);
			
			return modcap_eq(f + 1, b);
		}
		
		int GetSize()
		{
			return front - back;
		}
		
		bool TryEnqueue(const T& value)
		{
			int f = front.load(std::memory_order_acquire);

			do {
				int b = back;
				if (modcap_eq(f + 1, b)){
					return false; //full
				}
			} while (!front.compare_exchange_weak(f, f + 1, std::memory_order_release, std::memory_order_acquire)); //somebody beat us
			
			buffer[modcap(f)] = value;
			readfront.fetch_add(1, std::memory_order_acq_rel);
			return true;
		}
		
		bool TryDequeue(T* out)
		{
			int f = readfront.load(std::memory_order_acquire);
			int b = back.load(std::memory_order_acquire);
			
			if (modcap_eq(f, b)){
				return false;
			}
			
			*out = buffer[modcap(b)];
			back.store(b + 1, std::memory_order_release);
			
			return true;
		}
		
		int TryDequeue(T* out, int maxOut)
		{
			int f = modcap(readfront.load(std::memory_order_acquire));
			int b = modcap(back.load(std::memory_order_acquire));
			
			if (f == b){ //empty
				return 0;
			}
			
			int top = (f < b) ? capacity : f;

			int blockCopySize = top - b;
			blockCopySize = (blockCopySize > maxOut) ? maxOut : blockCopySize;
			memcpy(out, buffer + b, blockCopySize * sizeof (T));
			back.fetch_add(blockCopySize, std::memory_order_release);
			return blockCopySize;
		}
	};
}
#endif	/* LOCKLESS_RING_H */

