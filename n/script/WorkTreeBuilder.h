/*******************************
Copyright (C) 2013-2015 gregoire ANGERAND

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
#ifndef N_SCRIPT_WORKTREEBUILDER_H
#define N_SCRIPT_WORKTREEBUILDER_H

#include "WorkTreeNode.h"
#include "SymbolStack.h"

namespace n {
namespace script {

class ValidationErrorException : public std::exception
{
	public:
		ValidationErrorException(const core::String &m, TokenPosition tk) : msg(m), position(tk) {
		}

		virtual const char *what() const noexcept override;
		virtual const char *what(const core::String &code) const noexcept;

	private:
		core::String msg;
		TokenPosition position;

		mutable core::String buffer;
};

class WorkTreeBuilder : NonCopyable
{
	public:
		class Guard : NonCopyable
		{
			public:
				~Guard() {
					if(builder) {
						builder->popStack();
					}
				}

				Guard(Guard &&g) : builder(g.builder) {
					g.builder = 0;
				}

			private:
				friend class WorkTreeBuilder;
				Guard(WorkTreeBuilder *b) : builder(b) {
					builder->pushStack();
				}

				WorkTreeBuilder *builder;
		};

		WorkTreeBuilder();

		WorkTreeVariable *declareVar(const core::String &name, const core::String &typeName, TokenPosition tk = TokenPosition());
		WorkTreeVariable *getVar(const core::String &name, TokenPosition tk = TokenPosition());

		Guard stack();
		void pushStack();
		void popStack();

	private:
		SymbolStack<WorkTreeVariable *> variablesStack;
		core::Array<WorkTreeVariable *> variables;


};

}
}

#endif // WORKTREEBUILDER_H
