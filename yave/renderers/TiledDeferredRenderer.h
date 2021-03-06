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
#ifndef  YAVE_RENDERERS_TILEDDEFERREDRENDERER_H
#define  YAVE_RENDERERS_TILEDDEFERREDRENDERER_H

#include "GBufferRenderer.h"

#include <yave/graphics/images/IBLProbe.h>

#include <yave/graphics/shaders/ComputeProgram.h>
#include <yave/graphics/bindings/DescriptorSet.h>

namespace yave {

class IBLData : public DeviceLinked, NonCopyable {
	public:
		IBLData(DevicePtr dptr);

		const IBLProbe& envmap() const;
		TextureView brdf_lut() const;

	private:
		Texture _brdf_lut;
		IBLProbe _envmap;
};

class TiledDeferredRenderer : public Renderer {
	public:
		static constexpr usize max_light_count = 1024 * 16;

		TiledDeferredRenderer(const Ptr<GBufferRenderer>& gbuffer, const Ptr<IBLData>& ibl_data);

		const math::Vec2ui& size() const;

		TextureView lighting() const;
		TextureView output() const override;

		const SceneView scene_view() const {
			return _gbuffer->scene_view();
		}

	protected:
		void build_frame_graph(FrameGraphNode& frame_graph) override;
		void pre_render(CmdBufferRecorder& recorder, const FrameToken&) override;
		void render(CmdBufferRecorder& recorder, const FrameToken&) override;

	private:

		Ptr<GBufferRenderer> _gbuffer;

		ComputeProgram _lighting_program;

		Ptr<IBLData> _ibl_data;

		StorageTexture _acc_buffer;
		TypedBuffer<uniform::Light, BufferUsage::StorageBit, MemoryType::CpuVisible> _lights_buffer;
		DescriptorSet _descriptor_set;

		u32 _directional_count;

};

}

#endif //  YAVE_RENDERERS_TILEDDEFERREDRENDERER_H
