/*******************************
Copyright (c) 2016-2017 Gr�goire Angerand

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

#include "Pipeline.h"

#include <unordered_set>

namespace yave {

static void find_nodes(Node* node, std::unordered_set<Node*>& nodes) {
	for(Node* n : node->dependencies()) {
		nodes.insert(n);
	}
}

Pipeline::Pipeline(core::Unique<Node> root) : _root(std::move(root)) {
}

void Pipeline::process(const FrameToken& token) {
	std::unordered_set<Node*> nodes;
	find_nodes(_root.as_ptr(), nodes);

	while(true) {
		auto it = std::find_if(nodes.begin(), nodes.end(), [&nodes](Node* n) {
			auto deps = n->dependencies();
			return std::none_of(deps.begin(), deps.end(), [&nodes](Node* d) { return nodes.find(d) != nodes.end(); });
		});

		if(it == nodes.end()) {
			fatal("Unable to find processable node in pipeline.");
		}

		nodes.erase(it);
		(*it)->process(token);

		if(nodes.empty()) {
			return;
		}
	}
}

}