#ifndef FLOW_NODE_LIST_ITERATOR_HEADER
#define FLOW_NODE_LIST_ITERATOR_HEADER

#include <bits/stdc++.h>

namespace flow {
	template <typename type, typename NodeType,
		bool Const = false, bool Reverse = false>
	class NodeListIterator {
		private:
			NodeType *node;
			typedef NodeListIterator<type, NodeType, Const, Reverse> Iterator;

		public:
			NodeListIterator(NodeType *node)
				: node(node) {}

			NodeType& get_node()
			{
				return *node;
			}

			NodeType *get_node_pointer()
			{
				return node;
			}

			Iterator& operator=(const Iterator& other_iterator)
			{
				node = other_iterator.node;
				return *this;
			}

			const type& operator*() const
			{
				return node->value;
			}

			template <bool T = true>
			typename std::enable_if<T && !Const, type &>::type
			/* type& */ operator*()
			{
				return node->value;
			}

			type *operator->()
			{
				return &(node->value);
			}

			Iterator& /* prefix */ operator++()
			{
				node = node->next;
				return *this;
			}

			Iterator /* postfix */ operator++(int)
			{
				Iterator old_it = *this;
				node = node->next;
				return old_it;
			}

			template <bool T = true>
			typename std::enable_if<T && Reverse, Iterator>::type
			/* Iterator */ /* prefix */ operator--()
			{
				node = node->prev;
				return *this;
			}

			template <bool T = true>
			typename std::enable_if<T && Reverse, Iterator>::type
			/* Iterator */ /* postfix */ operator--(int)
			{
				Iterator old_it = *this;
				node = node->prev;
				return old_it;
			}

			bool operator==(const Iterator& other)
			{
				return node == other.node;
			}

			bool operator!=(const Iterator& other)
			{
				return node != other.node;
			}
	};
};

#endif
