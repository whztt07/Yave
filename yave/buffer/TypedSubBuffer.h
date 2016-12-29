/*******************************
Copyright (C) 2013-2016 gregoire ANGERAND

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
**********************************/
#ifndef YAVE_BUFFER_TYPEDSUBBUFFER_H
#define YAVE_BUFFER_TYPEDSUBBUFFER_H

#include "TypedBuffer.h"
#include "SubBuffer.h"

namespace yave {

template<typename Elem, BufferUsage Usage, MemoryFlags Flags = prefered_memory_flags<Usage>()>
class TypedSubBuffer : public SubBuffer<Usage, Flags> {

	using Base = SubBuffer<Usage, Flags>;

	public:
		using Element = Elem;

		template<BufferUsage BuffUsage>
		TypedSubBuffer(const TypedBuffer<Elem, BuffUsage, Flags>& buffer, usize offset, usize count) : Base(buffer, offset * sizeof(Elem), count * sizeof(Elem)) {
		}

		template<BufferUsage BuffUsage>
		explicit TypedSubBuffer(const TypedBuffer<Elem, BuffUsage, Flags>& buffer, usize offset = 0) : Base(buffer, offset * sizeof(Elem)) {
		}

		usize size() const {
			return this->byte_size() / sizeof(Elem);
		}

		usize offset() const {
			return this->byte_offset() / sizeof(Elem);
		}

		TypedMapping<Element, Flags> map() {
			return TypedMapping<Element, Flags>(*this);
		}
};


template<typename Elem, BufferUsage Usage, MemoryFlags Flags>
inline auto sub_buffer(const TypedBuffer<Elem, Usage, Flags>& buffer, usize offset = 0) {
	return TypedSubBuffer<Elem, Usage, Flags>(buffer, offset);
}

}

#endif // YAVE_BUFFER_TYPEDSUBBUFFER_H
