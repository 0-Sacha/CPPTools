#pragma once

#include "CPPTools/Formater/Core/Core.h"

#define CPPTOOLS_FORMAT_DECLARED

namespace CPPTools::Fmt {

	template<typename CharFormat, typename CharBuffer>
	class BasicFormatContext;

	template<typename T, typename FormatContext = BasicFormatContext<char, char>>
	struct FormatType {
		static void Write(const T& i, FormatContext& context) {
			static_assert(false, __FUNCSIG__);
		}
	};

}

#define CPPTOOLS_AUTO_FORMAT(Type, fmt, ...)	namespace CPPTools::Fmt {\
													template<typename FormatContext>\
													struct FormatType<Type> {\
														static void Write(const Type& value, FormatContext& context) {\
															context.LittleFormat(fmt, __VA_ARGS__);\
														}\
													};\
												}