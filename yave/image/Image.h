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
#ifndef YAVE_IMAGE_IMAGE_H
#define YAVE_IMAGE_IMAGE_H

#include <yave/yave.h>

#include "ImageBase.h"

namespace yave {

template<ImageUsage Usage>
class Image : public ImageBase {
	public:
		Image() = default;

		Image(DevicePtr dptr, ImageFormat format, const math::Vec2ui& image_size, const u8* data = nullptr) : ImageBase(dptr, ImageFormat(format), Usage, image_size, data) {
		}

		Image(Image&& other) {
			swap(other);
		}

		Image& operator=(Image&& other) {
			swap(other);
			return *this;
		}

};

using Texture = Image<ImageUsage::Texture>;
using DepthAttachment = Image<ImageUsage::Depth>;
using ColorAttachment = Image<ImageUsage::Color>;

}

#endif // YAVE_IMAGE_IMAGE_H
