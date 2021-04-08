#ifndef FLOW_BUFFER_ITERATOR_HEADER
#define FLOW_BUFFER_ITERATOR_HEADER

#include <bits/stdc++.h>

namespace flow {
	template <typename type, bool Const = false>
	class BufferIterator {
		private:
			type *ptr;

		public:
			BufferIterator(type *ptr) : ptr(ptr) {}

			BufferIterator(const BufferIterator<type, Const>& other) : ptr(other.ptr) {}

			BufferIterator(BufferIterator<type, Const>&& other) : ptr(other.ptr) {}

			BufferIterator<type, Const>& operator=(
				const BufferIterator<type, Const>& other
			) {
				if (this == &other) return *this;

				ptr = other.ptr;
				return *this;
			}

			BufferIterator<type, Const>& operator=(
				BufferIterator<type, Const>&& other
			) {
				if (this == &other) return *this;

				ptr = other.ptr;
				return *this;
			}

			const type& operator*() const
			{
				return *ptr;
			}

			template <bool T = true>
			typename std::enable_if<T && !Const, type&>::type
			/* type & */ operator*()
			{
				return *ptr;
			}

			type *operator->()
			{
				return ptr;
			}

			BufferIterator<type, Const>& /* prefix */ operator++()
			{
				ptr++;
				return *this;
			}

			BufferIterator<type, Const> /* postfix */ operator++(int)
			{
				BufferIterator<type, Const> old_it = *this;
				ptr++;
				return old_it;
			}

			BufferIterator<type, Const>& /* prefix */ operator--()
			{
				ptr--;
				return *this;
			}

			BufferIterator<type, Const> /* postfix */ operator--(int)
			{
				BufferIterator<type, Const> old_it = *this;
				ptr--;
				return old_it;
			}

			void operator+=(size_t increment)
			{
				ptr += increment;
			}

			void operator-=(size_t decrement)
			{
				ptr -= decrement;
			}

			bool operator==(const BufferIterator<type, Const>& other)
			{
				return ptr == other.ptr;
			}

			bool operator!=(const BufferIterator<type, Const>& other)
			{
				return ptr != other.ptr;
			}

			bool operator<(const BufferIterator<type, Const>& other)
			{
				return ptr < other.ptr;
			}

			bool operator<=(const BufferIterator<type, Const>& other)
			{
				return ptr <= other.ptr;
			}

			bool operator>(const BufferIterator<type, Const>& other)
			{
				return ptr > other.ptr;
			}

			bool operator>=(const BufferIterator<type, Const>& other)
			{
				return ptr >= other.ptr;
			}
	};
};


#endif