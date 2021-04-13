#ifndef FLOW_POINTER_HEADER
#define FLOW_POINTER_HEADER

#include <bits/stdc++.h>

namespace flow {
	/**
	 *  @brief  Owned pointer implementation. The pointer is deleted when the
	 *  Pointer object goes out of scope.
	 */
	template <typename type>
	class Pointer {
		protected:
			type *ptr;

		public:
			/**
			 *  @brief  Creates an uninitialised owned Pointer.
			 */
			Pointer()
			{
				ptr = new type;
			}

			/**
			 *  @brief  Creates an owned Pointer and initialises it.
			 */
			 Pointer(type&& value)
			 {
					ptr = new type;
					*ptr = std::move(value);
			 }

			/**
			 *  @brief  Transfers ownership of another Pointer to this Pointer.
			 */
			Pointer(Pointer&& other) : ptr(other.ptr)
			{
				other.ptr = NULL;
			}

			/**
			 *  @brief  Owned Pointers cannot be copied. Use move semantics instead.
			 */
			Pointer(const Pointer&) = delete;

			/**
			 *  @brief  Transfers ownership of another Pointer to this Pointer.
			 */
			Pointer& operator=(Pointer&& other)
			{
				if (this == &other) return *this;

				ptr = other.ptr;
				other.ptr = NULL;

				return *this;
			}

			/**
			 *  @brief  Owned Pointers cannot be copied. Use move semantics instead.
			 */
			Pointer& operator=(const Pointer&) = delete;

			/**
			 *  @brief  Deletes an owned Pointer.
			 */
			~Pointer()
			{
				delete ptr;
			}

			/**
			 *  @brief  Returns a read-only reference to the value the owned Pointer points at.
			 */
			const type& operator*() const
			{
				return *ptr;
			}

			/**
			 *  @brief  Returns a read/write reference to the value the owned Pointer points at.
			 */
			type& operator*()
			{
				return *ptr;
			}

			/**
			 *  @brief  Returns the raw pointer of this owned Pointer.
			 */
			type *operator->()
			{
				return ptr;
			}
	};
};

#endif