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
#ifndef YAVE_UTILS_SERDE_H
#define YAVE_UTILS_SERDE_H

#include <yave/yave.h>
#include <y/serde/serde.h>

namespace yave {

namespace fs {
static constexpr u32 magic_number = 0x65766179;
static constexpr u32 mesh_file_type = 1;
static constexpr u32 image_file_type = 2;
static constexpr u32 animation_file_type = 3;
static constexpr u32 font_file_type = 4;
static constexpr u32 scene_file_type = 5;
static constexpr u32 material_file_type = 6;
}

}

#endif // YAVE_UTILS_SERDE_H
