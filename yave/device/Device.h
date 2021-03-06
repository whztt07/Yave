/*******************************
Copyright (c) 2016-2018 Grégoire Angerand

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
#ifndef YAVE_DEVICE_DEVICE_H
#define YAVE_DEVICE_DEVICE_H

#include <yave/yave.h>
#include "PhysicalDevice.h"
#include "ThreadLocalDeviceData.h"

#include "extentions/DebugMarker.h"

#include <yave/graphics/images/Sampler.h>
#include <yave/graphics/queues/QueueFamily.h>
#include <yave/graphics/memory/DeviceAllocator.h>

#include <yave/graphics/vk/destroy.h>

#include <thread>

namespace yave {

class Device : NonCopyable {

	struct ScopedDevice {
		~ScopedDevice();
		vk::Device device;
	};

	public:
		explicit Device(Instance& instance);
		~Device();

		const PhysicalDevice& physical_device() const;
		const Instance& instance() const;

		DeviceAllocator& allocator() const;

		CmdBuffer<CmdBufferUsage::Disposable> create_disposable_cmd_buffer() const;
		CmdBuffer<CmdBufferUsage::Primary> create_cmd_buffer() const;

		const QueueFamily& queue_family(vk::QueueFlags flags) const;
		const Queue& queue(vk::QueueFlags) const;
		Queue& queue(vk::QueueFlags);

		ThreadDevicePtr thread_data() const;

		const vk::PhysicalDeviceLimits& vk_limits() const;

		vk::Device vk_device() const;
		vk::Sampler vk_sampler() const;

		const DebugMarker* debug_marker() const;


		template<typename T>
		auto create_descriptor_set_layout(T&& t) const {
			return _descriptor_layout_pool->create_descriptor_set_layout(y_fwd(t));
		}

		template<typename T>
		void destroy(T t) const {
			/*if(t != T())*/ {
				detail::destroy(this, t);
			}
		}

	private:
		Instance& _instance;
		PhysicalDevice _physical;

		core::Vector<QueueFamily> _queue_families;

		ScopedDevice _device;

		core::Vector<Queue> _queues;

		Sampler _sampler;

		mutable DeviceAllocator _allocator;

		mutable concurrent::SpinLock _lock;
		mutable std::unordered_map<std::thread::id, std::unique_ptr<ThreadLocalDeviceData>> _thread_local_datas;

		mutable CmdBufferPool<CmdBufferUsage::Disposable> _disposable_cmd_pool;
		mutable CmdBufferPool<CmdBufferUsage::Primary> _primary_cmd_pool;

		std::unique_ptr<DescriptorSetLayoutPool> _descriptor_layout_pool;

		struct {
			std::unique_ptr<DebugMarker> debug_marker;
		} _extensions;
};


template<typename T>
void DeviceLinked::destroy(T t) const {
	if(device()) {
		device()->destroy(t);
	}
}

template<typename T>
void ThreadDeviceLinked::destroy(T t) const {
	if(device()) {
		device()->destroy(t);
	}
}


}


#endif // YAVE_DEVICE_DEVICE_H
