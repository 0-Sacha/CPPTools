#pragma once

#include "CPPTools/Formatter/Formatter.h"
#include <stack>

namespace CPPTools::Fmt {

	template<typename T, typename FormatContext>
	struct FormatType<std::stack<T>, FormatContext>
	{
		static void Write(const std::stack<T>& t, FormatContext& context) {

		}
	};

}