#pragma once

#include "CPPTools/Formatter/Formatter.h"
#include <forward_list>

namespace CPPTools::Fmt {
	template<typename T, typename FormatContext>
	struct FormatType<std::forward_list<T>, FormatContext>
	{
		static void Write(const std::list<T>& t, FormatContext& context) {
			FormatType<ForwardAsSTDEnumerable<std::forward_list<T>>, FormatContext>::Write(t, context);
		}
	};
}