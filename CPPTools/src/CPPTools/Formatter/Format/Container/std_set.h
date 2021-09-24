#pragma once

#include "CPPTools/Formatter/Formatter.h"
#include <set>

namespace CPPTools::Fmt {
	template<typename T, typename FormatContext>
	struct FormatType<std::set<T>, FormatContext>
	{
		static void Write(const std::set<T>& t, FormatContext& context) {
			FormatType<ForwardAsSTDEnumerable<std::set<T>, FormatContext>::Write(t, context);
		}
	};

	template<typename T, typename FormatContext>
	struct FormatType<std::multiset<T>, FormatContext>
	{
		static void Write(const std::multiset<T>& t, FormatContext& context) {
			FormatType<ForwardAsSTDEnumerable<std::multiset<T>, FormatContext>::Write(t, context);
		}
	};
}