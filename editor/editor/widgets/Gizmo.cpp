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
#include "Gizmo.h"

#include <imgui/imgui.h>

namespace editor {

static constexpr float gizmo_width = 2.0f;
static constexpr float gizmo_size = 0.1f;

static bool is_clicked() {
	return ImGui::IsMouseClicked(0) && !ImGui::IsAnyItemHovered() && !ImGui::IsAnyItemActive() && ImGui::IsWindowHovered();
}

static bool is_clicking(math::Vec2 cursor, const math::Vec2& vec) {
	float len = vec.length();
	math::Vec2 n = vec / len;
	float d = cursor.dot(n);

	if(d > len || d < 0.0f) {
		return false;
	}

	float d_2 = std::fabs(cursor.length2() - (d * d));
	return d_2 < (2.0f * gizmo_width * gizmo_width);
}

static math::Vec2 to_screen_pos(const math::Matrix4<>& view_proj, const math::Vec3& pos) {
	auto h_pos = view_proj * math::Vec4(pos, 1.0f);
	return (h_pos.to<2>() / h_pos.w()) * 0.5f + 0.5f;
}

Gizmo::Gizmo(SceneView* scene_view) : Gadget("Gizmo") {
	set_scene_view(scene_view);
}

void Gizmo::set_scene_view(SceneView* scene_view) {
	_scene_view = scene_view;
	_transformable = nullptr;
}

void Gizmo::set_transformable(Transformable* tr) {
	_transformable = tr;
}

void Gizmo::paint_ui(CmdBufferRecorder<>&, const FrameToken&) {
	if(!_transformable) {
		return;
	}

	math::Matrix4<> view_proj = _scene_view->camera().viewproj_matrix();
	math::Transform<> object = _transformable->transform();

	auto projected = (view_proj * math::Vec4(object.position(), 1.0f));
	auto perspective = gizmo_size * projected.w();

	math::Vec2 viewport = ImGui::GetWindowSize();
	math::Vec2 offset = ImGui::GetWindowPos();

	auto center = to_screen_pos(view_proj, object.position()) * viewport + offset;

	math::Vec2 axis[] = {
			to_screen_pos(view_proj, object.position() + math::Vec3(perspective, 0.0f, 0.0f)) * viewport + offset,
			to_screen_pos(view_proj, object.position() + math::Vec3(0.0f, perspective, 0.0f)) * viewport + offset,
			to_screen_pos(view_proj, object.position() + math::Vec3(0.0f, 0.0f, perspective)) * viewport + offset
		};

	for(usize i = 0; i != 3; ++i) {
		u32 color = 0xFF000000 | (0xFF << (8 * i));
		ImGui::GetWindowDrawList()->AddLine(center, axis[i], color, gizmo_width);
	}
	ImGui::GetWindowDrawList()->AddCircleFilled(center, 1.5f * gizmo_width, 0xFFFFFFFF);



	if(is_clicked()) {
		math::Vec2 cursor = math::Vec2(ImGui::GetIO().MousePos) - center;
		_dragging_mask = 0;
		for(usize i = 0; i != 3; ++i) {
			if(is_clicking(cursor, axis[i] - center)) {
				_dragging_mask |= (1 << i);
			}
		}
	} else if(!ImGui::IsMouseDown(0)) {
		_dragging_mask = 0;
	}



	if(_dragging_mask) {
		auto inv_matrix = _scene_view->camera().inverse_matrix();
		auto cam_pos = _scene_view->camera().position();
		auto object = _transformable->transform();

		math::Vec2 ndc = ((math::Vec2(ImGui::GetIO().MousePos) - offset) / viewport) * 2.0f - 1.0f;
		math::Vec4 h_world = inv_matrix * math::Vec4(ndc, 0.5f, 1.0f);
		math::Vec3 world = h_world.to<3>() / h_world.w();

		math::Vec3 ray = (world - cam_pos).normalized();
		float dist = (object.position() - cam_pos).length();

		math::Vec3 new_pos = cam_pos + ray * dist;

		for(usize i = 0; i != 3; ++i) {
			if(_dragging_mask & (1 << i)) {
				_transformable->position()[i] = new_pos[i];
			}
		}
	}
}


}
