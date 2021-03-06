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
#ifndef YAVE_OBJECTS_TEXT_H
#define YAVE_OBJECTS_TEXT_H

#include <yave/assets/AssetPtr.h>
#include <yave/graphics/buffers/buffers.h>
#include <yave/font/Font.h>

#include "Renderable.h"

namespace yave {

class Text final : public Renderable, NonCopyable {

	public:
		Text(const AssetPtr<Font>& font, const core::String& text);

		void render(RenderPassRecorder&, const SceneData&) const override;


	private:
		AssetPtr<Font> _font;
		core::String _text;

		TypedUniformBuffer<Font::CharData, MemoryType::DeviceLocal> _buffer;
};

}

#endif // YAVE_OBJECTS_TEXT_H
