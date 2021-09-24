#pragma once


#include "CPPTools/Formatter/Formatter.h"
#include <map>

#include "std_tuple.h"

namespace CPPTools::Fmt {
	template<typename T1, typename T2, typename FormatContext>
	struct FormatType<std::map<T1, T2>, FormatContext>
	{
		static void Write(const std::map<T1, T2>& t, FormatContext& context) {
			FormatType<ForwardAsSTDEnumerable<std::map<T1, T2>, FormatContext>::Write(t, context);
		}
	};

	template<typename T1, typename T2, typename FormatContext>
	struct FormatType<std::multimap<T1, T2>, FormatContext>
	{
		static void Write(const std::multimap<T1, T2>& t, FormatContext& context) {
			FormatType<ForwardAsSTDEnumerable<std::multimap<T1, T2>, FormatContext>::Write(t, context);
		}
	};
}