#pragma once

#include "CPPTools/Formatter/Formatter.h"
#include <unordered_map>

#include "std_tuple.h"

namespace CPPTools::Fmt {

	template<typename T1, typename T2, typename FormatContext>
	struct FormatType<std::unordered_map<T1, T2>, FormatContext>
	{
		static void Write(const std::unordered_map<T1, T2>& t, FormatContext& context) {
			FormatType<ForwardAsSTDEnumerable<std::unordered_map<T1, T2>, FormatContext>::Write(t, context);
		}
	};

	template<typename T1, typename T2, typename FormatContext>
	struct FormatType<std::unordered_multimap<T1, T2>, FormatContext>
	{
		static void Write(const std::unordered_multimap<T1, T2>& t, FormatContext& context) {
			FormatType<ForwardAsSTDEnumerable<std::unordered_multimap<T1, T2>, FormatContext>::Write(t, context);
		}
	};
}