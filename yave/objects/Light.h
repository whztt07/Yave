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
#ifndef YAVE_OBJECTS_LIGHT_H
#define YAVE_OBJECTS_LIGHT_H

#include <yave/graphics/bindings/uniforms.h>
#include "Transformable.h"

namespace yave {

class Light final : public Transformable {
	public:
		enum Type {
			Directional = 0,
			Point = 1
		};

		Light(Type type);

		Type type() const;

		math::Vec3& color();
		const math::Vec3& color() const;

		float& intensity();
		float intensity() const;

		float& radius();
		float radius() const;

		operator uniform::Light() const;

	private:
		Type _type;
		math::Vec3 _color = math::Vec3{1.0f};
		float _intensity = 1.0f;
		float _radius = 1.0f;
};

}

#endif // YAVE_OBJECTS_LIGHT_H
