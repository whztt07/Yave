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
#ifndef YAVE_GRAPHICS_IMAGES_IMAGEDATA_H
#define YAVE_GRAPHICS_IMAGES_IMAGEDATA_H

#include <yave/utils/serde.h>

#include <y/math/Vec.h>

#include "ImageFormat.h"

namespace yave {

class ImageData : NonCopyable {

	public:
		ImageData() = default;

		ImageData(ImageData&& other);
		ImageData& operator=(ImageData&& other);

		usize byte_size(usize mip = 0) const;
		usize layer_byte_size() const;
		usize combined_byte_size() const;

		const math::Vec3ui& size() const;
		math::Vec3ui size(usize mip) const;

		const ImageFormat& format() const;

		usize layers() const;
		usize mipmaps() const;

		usize data_offset(usize layer = 0, usize mip = 0) const;
		const u8* data(usize layer = 0, usize mip = 0) const;

		ImageData(const math::Vec2ui& size, const u8* data, ImageFormat format, u32 mips = 1);

		y_deserialize(fs::magic_number, fs::image_file_type, u32(3),
					y_serde_call([this](const math::Vec2ui& size) { _size = math::Vec3ui(size, 1); }),
					_layers, _mips, _format,
					y_serde_call([this] { _data = std::make_unique<u8[]>(combined_byte_size()); }),
					y_serde_fixed_array(combined_byte_size(), _data.get()))

		y_serialize(fs::magic_number, fs::image_file_type, u32(3),
					_size.to<2>(), _layers, _mips, _format, combined_byte_size(),
					y_serde_fixed_array(combined_byte_size(), _data.get()))

	private:
		void swap(ImageData& other);

		math::Vec3ui _size;
		ImageFormat _format;

		u32 _layers = 1;
		u32 _mips = 1;

		std::unique_ptr<u8[]> _data;
};

}

#endif // YAVE_GRAPHICS_IMAGES_IMAGEDATA_H
