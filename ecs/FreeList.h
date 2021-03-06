/*******************************
Copyright (c) 2016-2018 Gr�goire Angerand

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
**********************************/
#ifndef YAVE_ECS_FREELIST_H
#define YAVE_ECS_FREELIST_H

#include "FreeListNode.h"

namespace yave {
namespace ecs {

template<typename T, typename IDTag = T>
class FreeList {
	using node_t = FreeListNode<T, IDTag>;

	template<bool Const>
	class Iterator {
		using internal_t = std::conditional_t<Const, const node_t*, node_t*>;
		public:
			using reference = decltype(std::declval<internal_t>()->get());
			using pointer = decltype(&std::declval<internal_t>()->get());
			using value_type = std::remove_reference_t<reference>;
			using difference_type = usize;
			using iterator_category = std::forward_iterator_tag;

			static_assert(std::is_reference_v<reference>);
			static_assert(std::is_pointer_v<pointer>);
			static_assert(!std::is_reference_v<pointer>);

			Iterator(internal_t it  = nullptr) : _it(it) {
				skip_emtpy();
			}

			Iterator& operator++() {
				++_it;
				skip_emtpy();
				return *this;
			}

			Iterator& operator++(int) {
				auto a = *this;
				++(*this);
				return a;
			}

			auto& operator*() const {
				return _it->get();
			}

			auto operator->() const {
				return &_it->get();
			}

			bool operator==(const Iterator& other) const {
				return _it == other._it;
			}

			bool operator!=(const Iterator& other) const {
				return _it != other._it;
			}

			bool operator<(const Iterator& other) const {
				return _it < other._it;
			}

			bool operator>(const Iterator& other) const {
				return _it > other._it;
			}

		private:
			void skip_emtpy() {
				while(_it->is_free()) {
					++_it;
				}
			}
			internal_t _it;
	};

	public:
		using Id = typename node_t::Id;

		using iterator = Iterator<false>;
		using const_iterator = Iterator<true>;

		static constexpr u32 invalid_index = node_t::invalid_index;


		// We need the last element of the _nodes vector to be an empty node to ensure that an iterator can not run past the end
		FreeList() {
			_nodes.emplace_back();
		}

		Id add() {
			y_debug_assert(_nodes.last().is_free());

			u32 index = _next;
			if(index == invalid_index) {
				// account for extra end node, and add a new one
				index = _nodes.size() - 1;
				_nodes.emplace_back();
			}
			node_t& node = _nodes[index];
			_next = node.next_free();
			node.create(index);
			y_debug_assert(_nodes.last().is_free());
			return node.id();
		}

		void remove(Id id) {
			y_debug_assert(_nodes.last().is_free());

			if(id.index() >= _nodes.size()) {
				return;
			}
			node_t& node = _nodes[id.index()];
			if(node.id() != id) {
				return;
			}
			_next = node.destroy(_next);
		}

		T* get(Id id) {
			if(id.index() >= _nodes.size()) {
				return nullptr;
			}
			node_t& node = _nodes[id.index()];
			if(node.id() != id) {
				return nullptr;
			}
			return &node.get();
		}

		const T* get(Id id) const{
			return const_cast<std::remove_const_t<decltype(this)>>(this)->get(id);
		}

		iterator begin() {
			return iterator(_nodes.begin());
		}

		iterator end() {
			y_debug_assert(_nodes.last().is_free());
			return iterator(_nodes.end() - 1);
		}

		const_iterator begin() const {
			return const_iterator(_nodes.begin());
		}

		const_iterator end() const {
			y_debug_assert(_nodes.last().is_free());
			return const_iterator(_nodes.end() - 1);
		}

	private:
		core::Vector<FreeListNode<T, IDTag>> _nodes;
		u32 _next = invalid_index;
};

}
}

#endif // YAVE_ECS_FREELIST_H
