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

#include "ImageImporter.h"

#include <editor/context/EditorContext.h>
#include <editor/import/image.h>

#include <y/io/Buffer.h>

#include <imgui/imgui.h>

namespace editor {

ImageImporter::ImageImporter(ContextPtr ctx) : Widget("Mesh importer"), ContextLinked(ctx) {
	_browser.set_has_parent(true);
	_browser.set_extension_filter(import::supported_image_extensions());
	_browser.set_selected_callback([this](const auto& filename) { import_async(filename); return true; });
	_browser.set_canceled_callback([this] { close(); return true; });
}

void ImageImporter::paint_ui(CmdBufferRecorder& recorder, const FrameToken& token)  {
	_browser.paint(recorder, token);

	if(is_loading()) {
		if(done_loading()) {
			try {
				const auto& imported = _import_future.get();
				_callback(imported);
			} catch(std::exception& e) {
				context()->ui().ok("Unable to import", fmt("Unable to import scene: %" , e.what()).data());
				_browser.show();
			}
			close();
		} else {
			ImGui::Text("Loading...");
		}
	}

}

bool ImageImporter::is_loading() const {
	return _import_future.valid();
}

bool ImageImporter::done_loading() const {
	return _import_future.wait_for(std::chrono::seconds(0)) == std::future_status::ready;
}

void ImageImporter::import_async(const core::String& filename) {
	_import_future = std::async(std::launch::async, [=] {
		return import::import_images(filename);
	});
}

}
