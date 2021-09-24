#pragma once

#include "CPPTools/Formatter/Formatter.h"
#include <vector>

namespace CPPTools::Fmt {

	template<typename T, typename FormatContext>
	struct FormatType<std::vector<T>, FormatContext>
	{
		static void Write(const std::vector<T>& t, FormatContext& context) {
			FormatType<ForwardAsSTDEnumerable<std::vector<T>>, FormatContext>::Write(t, context);
		}
	};

}