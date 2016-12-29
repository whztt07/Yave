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
#ifndef YAVE_SWAPCHAIN_H
#define YAVE_SWAPCHAIN_H

#include "yave.h"

#include "Framebuffer.h"

#include <yave/image/ImageFormat.h>
#include <yave/image/Image.h>
#include <yave/DeviceLinked.h>

namespace yave {

class Window;

class SwapchainImage : public Image<ImageUsage::Swapchain | ImageUsage::Color> {
	private:
		friend class Swapchain;

		SwapchainImage() : Image<ImageUsage::Swapchain | ImageUsage::Color>() {
		}
};

using SwapchainImageView = ImageView<ImageUsage::Swapchain | ImageUsage::Color>;

class Swapchain : NonCopyable, public DeviceLinked {

	struct Buffer {
		SwapchainImage color;
		DepthAttachment depth;
		Framebuffer framebuffer;

		Buffer(RenderPass& render_pass, SwapchainImage&& color_att, DepthAttachment&& depth_att);
	};

	public:
#ifdef Y_OS_WIN
		Swapchain(DevicePtr dptr, HINSTANCE instance, HWND handle);
#endif

		Swapchain(DevicePtr dptr, vk::SurfaceKHR&& surface);
		Swapchain(DevicePtr dptr, Window* window);
		~Swapchain();

		const Framebuffer& framebuffer(usize index) const;
		const RenderPass& render_pass() const;

		vk::SwapchainKHR vk_swapchain() const;

		const math::Vec2ui& size() const;
		usize buffer_count() const;

	private:
		math::Vec2ui _size;
		ImageFormat _depth_format;
		ImageFormat _color_format;

		core::Vector<Buffer> _buffers;
		RenderPass _render_pass;

		Owner<vk::SurfaceKHR> _surface;
		vk::SwapchainKHR _swapchain;

};





}

#endif // YAVE_SWAPCHAIN_H
