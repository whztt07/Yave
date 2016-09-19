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
#ifndef YAVE_CMDBUFFERRECORDER_H
#define YAVE_CMDBUFFERRECORDER_H

#include <yave/yave.h>
#include <yave/Framebuffer.h>

#include "RecordedCmdBuffer.h"

#include <yave/buffer/BufferMemoryReference.h>
#include <yave/mesh/StaticMeshInstance.h>
#include <yave/material/GraphicPipeline.h>

namespace yave {

class CmdBufferRecorder : NonCopyable {

	public:
		CmdBufferRecorder(CmdBuffer&& buffer);

		CmdBufferRecorder(CmdBufferRecorder&& other);
		CmdBufferRecorder& operator=(CmdBufferRecorder&& other);

		RecordedCmdBuffer end();
		~CmdBufferRecorder();

		vk::CommandBuffer get_vk_cmd_buffer() const;

		CmdBufferRecorder& bind_framebuffer(const Framebuffer& framebuffer);
		CmdBufferRecorder& bind_pipeline(const GraphicPipeline& pipeline);
		CmdBufferRecorder& draw(const StaticMeshInstance& static_mesh);

		template<MemoryFlags DstFlags, MemoryFlags SrcFlags>
		CmdBufferRecorder& copy_buffer(BufferMemoryReference<DstFlags, BufferTransfer::TransferDst> dst, BufferMemoryReference<SrcFlags, BufferTransfer::TransferSrc> src) {
			get_vk_cmd_buffer().copyBuffer(src->get_vk_buffer(), dst->get_vk_buffer(), dst->get_copy());

			return *this;
		}

	private:
		void swap(CmdBufferRecorder& other);

		CmdBuffer _cmd_buffer;
		usize _nested_passes;
};

}

#endif // YAVE_CMDBUFFERRECORDER_H