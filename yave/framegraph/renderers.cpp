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

#include "renderers.h"

namespace yave {

SceneRender SceneRender::create(FrameGraph& framegraph, const SceneView& view) {
	SceneRender render;

	//render._camera_buffer = framegraph.declare<decltype(render._camera_buffer)::resource_type>(framegraph.device(), usize(1));
	//render._transform_buffer = framegraph.declare<decltype(render._transform_buffer)::resource_type>(framegraph.device(), max_batch_count);
	//framegraph.declare_descriptor_set(render._camera_set, render._camera_buffer);

	render._scene_view = view;

	return render;
}

void SceneRender::render(RenderPassRecorder& recorder, const FrameGraphResourcePool* resources) const {
	/*auto& camera_buffer = resources->get(_camera_buffer);
	auto& transform_buffer = resources->get(_transform_buffer);
	auto& descriptor_set = resources->get(_camera_set);

	// fill render data
	{
		{
			auto camera_mapping = TypedMapping(camera_buffer);
			camera_mapping[0] = _scene_view.camera().viewproj_matrix();
		}

		if(transform_buffer.size() < _scene_view.scene().renderables().size() +_scene_view.scene().static_meshes().size()) {
			y_fatal("SceneRender::_attrib_buffer overflow");
		}

		auto transform_mapping = TypedMapping(transform_buffer);
		u32 attrib_index = 0;
		{
			// renderables
			for(const auto& r : _scene_view.scene().renderables()) {
				transform_mapping[attrib_index++] = r->transform();
			}

			// static meshes
			for(const auto& r : _scene_view.scene().static_meshes()) {
				transform_mapping[attrib_index++] = r->transform();
			}
		}
	}

	// render stuff
	{
		u32 attrib_index = 0;

#warning clean unnecessary buffer binding
		recorder.bind_attrib_buffers({transform_buffer, transform_buffer});

		// renderables
		{
			for(const auto& r : _scene_view.scene().renderables()) {
				r->render(recorder, Renderable::SceneData{descriptor_set, attrib_index++});
			}
		}

		// static meshes
		{
			for(const auto& r : _scene_view.scene().static_meshes()) {
				r->render(recorder, Renderable::SceneData{descriptor_set, attrib_index++});
			}
		}
	}*/
}

GBufferPass render_gbuffer(FrameGraph& framegraph, const SceneView& view, const math::Vec2ui& size) {
	static constexpr vk::Format depth_format = vk::Format::eD32Sfloat;
	static constexpr vk::Format color_format = vk::Format::eR8G8B8A8Unorm;
	static constexpr vk::Format normal_format = vk::Format::eR16G16B16A16Unorm;

	GBufferPass pass;
	pass.depth = framegraph.declare_image(depth_format, size);
	pass.color = framegraph.declare_image(color_format, size);
	pass.normal = framegraph.declare_image(normal_format, size);

	FrameGraphPassBuilder builder = framegraph.add_pass("G-buffer pass");
	builder.add_depth_output(pass.depth);
	builder.add_color_output(pass.color);
	builder.add_color_output(pass.normal);
	builder.set_render_func([=](CmdBufferRecorder& recorder, const FrameGraphPass* self) {
			auto render_pass = recorder.bind_framebuffer(self->framebuffer());
			pass.scene.render(render_pass, self->resources());
		});

	return pass;
}

}
