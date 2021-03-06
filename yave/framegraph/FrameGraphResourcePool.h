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
#ifndef YAVE_FRAMEGRAPH_FRAMEGRAPHRESOURCEPOOL_H
#define YAVE_FRAMEGRAPH_FRAMEGRAPHRESOURCEPOOL_H


#include <yave/graphics/buffers/Mapping.h>

#include <yave/graphics/bindings/DescriptorSet.h>
#include <yave/graphics/framebuffer/Framebuffer.h>

#include "FrameGraphResourceToken.h"
#include "FrameGraphPass.h"

namespace yave {

class FrameGraphResourcePool : public DeviceLinked, NonCopyable {
	public:
		FrameGraphResourcePool(DevicePtr dptr);
		~FrameGraphResourcePool();

		template<ImageUsage Usage>
		ImageView<Usage> get_image(FrameGraphImageId res) const {
			if(!res.is_valid()) {
				y_fatal("Invalid image resource.");
			}
			return TransientImageView<Usage>(_images.find(res)->second);
		}

		template<BufferUsage Usage, MemoryType Memory = MemoryType::DontCare>
		SubBuffer<Usage, Memory> get_buffer(FrameGraphBufferId res) const {
			if(!res.is_valid()) {
				y_fatal("Invalid buffer resource.");
			}
			return TransientSubBuffer<Usage, Memory>(_buffers.find(res)->second);
		}

		template<BufferUsage Usage, MemoryType Memory = MemoryType::DontCare, typename T>
		TypedSubBuffer<T, Usage, Memory> get_buffer(FrameGraphTypedBufferId<T> res) const {
			if(!res.is_valid()) {
				y_fatal("Invalid buffer resource.");
			}
			return TransientSubBuffer<Usage, Memory>(_buffers.find(res)->second);
		}

		template<typename T>
		TypedMapping<T> get_mapped_buffer(FrameGraphTypedBufferId<T> res) const {
			return TypedMapping<T>(get_buffer<BufferUsage::None, MemoryType::CpuVisible>(res));
		}

		/*template<BufferUsage Usage>
		SubBuffer<Usage> get_buffer(FrameGraphBufferToken<Usage> token) const {
			if(!token.is_valid()) {
				y_fatal("Invalid buffer resource token.");
			}
			return TransientSubBuffer<Usage>(_buffers.find(token.resource())->second);
		}

		template<typename T>
		TypedSubBuffer<T, BufferUsage::None, MemoryType::CpuVisible> get_buffer(FrameGraphBufferCpuToken<T> token) const {
			if(!token.is_valid()) {
				y_fatal("Invalid buffer resource token.");
			}
			return TransientSubBuffer<BufferUsage::None, MemoryType::CpuVisible>(_buffers.find(token.resource())->second);
		}

		template<typename T>
		auto get_mapped_buffer(FrameGraphBufferCpuToken<T> token) const {
			return TypedMapping<T>(get_buffer(token));
		}*/

		void create_image(FrameGraphImageId res, ImageFormat format, const math::Vec2ui& size, ImageUsage usage);
		void create_buffer(FrameGraphBufferId res, usize byte_size, BufferUsage usage, MemoryType memory);

		void release(FrameGraphImageId res);
		void release(FrameGraphBufferId res);

		ImageBarrier barrier(FrameGraphImageId res) const;
		BufferBarrier barrier(FrameGraphBufferId res) const;

		usize allocated_resources() const;

		u32 create_resource_id();

	private:
		bool create_image_from_pool(TransientImage<>& res, ImageFormat format, const math::Vec2ui& size, ImageUsage usage);
		bool create_buffer_from_pool(TransientBuffer& res, usize byte_size, BufferUsage usage, MemoryType memory);

		using hash_t = std::hash<FrameGraphResourceId>;
		std::unordered_map<FrameGraphImageId, TransientImage<>, hash_t> _images;
		std::unordered_map<FrameGraphBufferId, TransientBuffer, hash_t> _buffers;

		core::Vector<TransientImage<>> _released_images;
		core::Vector<TransientBuffer> _released_buffers;

		u32 _next_id = 0;
};

}

#endif // YAVE_FRAMEGRAPH_FRAMEGRAPHRESOURCEPOOL_H
