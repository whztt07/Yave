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

#include "Ui.h"

namespace editor {

Ui::Ui(ContextPtr ctx) : ContextLinked(ctx) {
}

core::ArrayView<std::unique_ptr<Widget>> Ui::widgets() const {
	return _widgets;
}

bool Ui::confirm(const char* message) {
#ifdef Y_OS_WIN
	return MessageBox(GetActiveWindow(), message, "Confirm", MB_OKCANCEL) != IDCANCEL;
#else
#warning not supported
	return true;
#endif
}

void Ui::ok(const char* title, const char* message) {
#ifdef Y_OS_WIN
	MessageBox(GetActiveWindow(), message, title, MB_OK);
#else
#warning not supported
#endif
}

void Ui::paint(CmdBufferRecorder& recorder, const FrameToken& token) {
	for(auto& e : _widgets) {
		e->paint(recorder, token);
	}

	for(usize i = 0; i < _widgets.size();) {
		if(!_widgets[i]->is_visible()) {
			_widgets.erase_unordered(_widgets.begin() + i);
		} else {
			++i;
		}
	}
}

}
