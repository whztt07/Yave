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
#ifndef  YAVE_RENDERERS_GBUFFERRENDERER_H
#define  YAVE_RENDERERS_GBUFFERRENDERER_H

#include "SceneRenderer.h"

namespace yave {

class GBufferRenderer : public Renderer {

	public:
		static constexpr vk::Format depth_format = vk::Format::eD32Sfloat;
		static constexpr vk::Format diffuse_format = vk::Format::eR8G8B8A8Unorm;
		static constexpr vk::Format normal_format = vk::Format::eR16G16B16A16Unorm;

		GBufferRenderer(const Ptr<SceneRenderer>& scene, const math::Vec2ui& size);

		const math::Vec2ui& size() const;

		TextureView depth() const;
		TextureView albedo_metallic() const;
		TextureView normal_roughness() const;

		TextureView output() const override {
			return albedo_metallic();
		}

		const SceneView& scene_view() const {
			return _scene->scene_view();
		}

		const auto& scene_renderer() const {
			return _scene;
		}

	protected:
		void build_frame_graph(FrameGraphNode& frame_graph) override;
		void render(CmdBufferRecorder& recorder, const FrameToken& token) override;

	private:
		Ptr<SceneRenderer> _scene;

		DepthTextureAttachment _depth;
		ColorTextureAttachment _color;
		ColorTextureAttachment _normal;

		Framebuffer _gbuffer;
};

}

#endif //  YAVE_RENDERERS_GBUFFERRENDERER_H
