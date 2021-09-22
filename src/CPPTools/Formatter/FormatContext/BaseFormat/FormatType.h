#pragma once

#include "CPPTools/Formatter/Core/Detail.h"

#define CPPTOOLS_FORMAT_DECLARED
namespace CPPTools::Fmt {

	template<typename CharFormat, typename CharBuffer, typename ...Args>
	class BasicFormatContext;

	template<typename T, typename FormatContext = BasicFormatContext<char, char>>
	struct FormatType {
		template<class K = T>
		static inline void Write(const K& t, FormatContext& context) {
			// CPPTOOLS_IF_MSVC(static_assert(false, __FUNCSIG__));
		}
	};
}

#define CPPTOOLS_AUTO_FORMAT(Type, fmt, ...)	template<typename FormatContext>\
													struct CPPTools::Fmt::FormatType<Type, FormatContext> {\
														static void Write(const Type& value, FormatContext& context) {\
															context.LittleFormat(fmt, __VA_ARGS__);\
														}\
													};\
												}



												