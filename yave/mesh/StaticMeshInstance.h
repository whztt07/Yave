/*******************************
Copyright (c) 2016-2017 Grégoire Angerand

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
#ifndef YAVE_MESH_STATICMESHINSTANCE_H
#define YAVE_MESH_STATICMESHINSTANCE_H

#include "MeshData.h"
#include <yave/buffer/buffers.h>

namespace yave {

class StaticMeshInstance : NonCopyable {

	public:
		StaticMeshInstance() = default;

		StaticMeshInstance(DevicePtr dptr, const MeshData& mesh_data);

		StaticMeshInstance(StaticMeshInstance&& other);
		StaticMeshInstance& operator=(StaticMeshInstance&& other);

		const TriangleBuffer<>& triangle_buffer() const;
		const VertexBuffer<>& vertex_buffer() const;
		const vk::DrawIndexedIndirectCommand& indirect_data() const;

		float radius() const;

	private:
		void swap(StaticMeshInstance& other);

		TriangleBuffer<> _triangle_buffer;
		VertexBuffer<> _vertex_buffer;
		vk::DrawIndexedIndirectCommand _indirect_data;

		float _radius;
};

}

#endif // YAVE_MESH_STATICMESHINSTANCE_H
