#ifndef FLOW_SHARED_POINTER_HEADER
#define FLOW_SHARED_POINTER_HEADER

#include <bits/stdc++.h>

namespace flow {
	/**
	 *  @brief  Shared pointer implementation. The pointer is deleted when
	 *  all SharedPointers referencing this object go out of scope.
	 */
	template <typename type>
	class SharedPointer {
		public:
			struct Reference {
				size_t ref_count;
				type obj;

				Reference() : ref_count(1) {}
				Reference(const type& obj) : ref_count(1), obj(obj) {}
				Reference(type&& obj) : ref_count(1), obj(std::move(obj)) {}
			};

		protected:
			Reference *ptr;

		public:
			/**
			 *  @brief  Creates an uninitialised SharedPointer.
			 */
			SharedPointer()
			{
				ptr = new Reference();
			}

			/**
			 *  @brief  Creates a new SharedPointer and initialises it.
			 */
			SharedPointer(type&& value)
			{
				ptr = new Reference(std::move(value));
			}

			/**
			 *  @brief  Creates a new reference to this SharedPointer. The new
			 *  reference will also own the reference and the reference count
			 *  will be incremented.
			 */
			SharedPointer(const SharedPointer& other) : ptr(other.ptr)
			{
				ptr->ref_count++;
			}

			/**
			 *  @brief  Transfers ownership of another SharedPointer to
			 *  this SharedPointer.
			 */
			SharedPointer(SharedPointer&& other) : ptr(other.ptr)
			{
				other.ptr = NULL;
			}

			/**
			 *  @brief  Creates a new reference to an existing SharedPointer.
			 *  The new reference will also own the reference and the reference
			 *  count will be incremented.
			 */
			SharedPointer& operator=(const SharedPointer& other)
			{
				if (this == &other) return *this;

				ptr = other.ptr;
				ptr->ref_count++;

				return *this;
			}

			/**
			 *  @brief  Transfers ownership of another SharedPointer to
			 *  this SharedPointer.
			 */
			SharedPointer& operator=(SharedPointer&& other)
			{
				if (this == &other) return *this;

				ptr = other.ptr;
				other.ptr = NULL;

				return *this;
			}

			/**
			 *  @brief  Deletes a SharedPointer.
			 */
			~SharedPointer()
			{
				if (ptr == NULL) return;
				ptr->ref_count--;
				if (ptr->ref_count == 0) delete ptr;
			}

			/**
			 *  @brief  Returns a read-only reference to the value the SharedPointer points at.
			 */
			const type& operator*() const
			{
				return ptr->obj;
			}

			/**
			 *  @brief  Returns a read/write reference to the value the SharedPointer points at.
			 */
			type& operator*()
			{
				return ptr->obj;
			}

			/**
			 *  @brief  Returns the raw pointer of this SharedPointer.
			 */
			type *operator->()
			{
				return &(ptr->obj);
			}

			/**
			 *  @brief  Returns the number of references to this SharedPointer.
			 */
			size_t ref_count() const
			{
				return ptr->ref_count;
			}
	};
};

#endif