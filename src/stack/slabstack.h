/* 
 * File:   slab_stack.h
 * Author: netzapper
 *
 * Created on December 14, 2013, 3:08 PM
 */

#ifndef SLAB_STACK_H
#define	SLAB_STACK_H

namespace devious
{
	/**
	 * A stack that does not allocate any additional memory.
	 */
	template <typename T>
	class SlabStack
	{
	private:
		T *slab;
		int capacity;
		int topIndex;
		
	public:
		/**
		 * Create a new SlabStack.
         * @param maxSize the maximum size of this stack.
         */
		SlabStack(int maxSize) : slab(nullptr), capacity(maxSize), topIndex(0)
		{
			slab = malloc(capacity * sizeof(T));
		}
		
		virtual ~SlabStack()
		{
			if (slab){
				free(slab);
			}
		}
		
		/**
		 * Is the stack empty?
		 */
		bool Empty()
		{
			return topIndex == 0;
		}
		
		/**
		 * Is the stack full?
         */
		bool Full()
		{
			return topIndex == capacity;
		}
		
		/**
		 * Increment the head index, returning a pointer
		 * to the top of the stack.
		 * 
		 * @return nullptr on full stack.
         */
		T* Push()
		{
			if (Full()){
				return nullptr;
			}
			
			return slab + topIndex++;
		}
		
		/**
		 * Return a pointer to the top of the stack, and decrement
		 * the top.
         * @return nullptr on empty stack.
         */
		T* Pop()
		{
			if (Empty())
			{
				return nullptr;
			}
			
			return slab + --topIndex;
		}
		
		
		/**
		 * Return a pointer to the top element of the stack, but do
		 * not decrement the top of the stack.
		 * 
		 * @return nullptr on empty stack.
         */
		T* Peek()
		{
			if (Empty())
			{
				return nullptr;
			}
			
			return  slab + (topIndex - 1);
			
		}
	};
}

#endif	/* SLAB_STACK_H */

