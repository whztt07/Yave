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

#include "EditorContext.h"

#include "scenes.h"

#include <yave/serialize/serialize.h>

#include <y/io/File.h>

namespace editor {

DevicePtr ContextLinked::device() const {
	return _ctx ? _ctx->device() : nullptr;
}

EditorContext::EditorContext(DevicePtr dptr) :
		DeviceLinked(dptr),
		texture_loader(dptr),
		mesh_loader(dptr),
		_scene(std::make_unique<Scene>()),
		_scene_view(std::make_unique<SceneView>(*_scene)) {

	load_scene();
	load_settings();
}

EditorContext::~EditorContext() {
	save_scene();
	save_settings();
}

void EditorContext::save_scene() {
	if(auto r = io::File::create("./scene.ys"); r.is_ok()) {
		serialize(r.unwrap(), *_scene, mesh_loader);
	}
}

void EditorContext::load_scene() {
	if(auto r = io::File::open("./scene.ys"); r.is_ok()) {
		deserialize(r.unwrap(), *_scene, mesh_loader);
	} else {
		fill_scene(scene(), texture_loader, mesh_loader);
	}
}

void EditorContext::save_settings() {
	if(auto r = io::File::create("./editor_settings.dat"); r.is_ok()) {
		r.unwrap().write_one(camera_settings);
	}
}

void EditorContext::load_settings() {
	if(auto r = io::File::open("./editor_settings.dat"); r.is_ok()) {
		r.unwrap().read_one(camera_settings);
	}
}

Scene* EditorContext::scene() const {
	return _scene.get();
}

SceneView* EditorContext::scene_view() const {
	return _scene_view.get();
}

}