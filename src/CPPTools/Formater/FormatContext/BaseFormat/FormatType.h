#pragma once

#include "CPPTools/Formater/Core/Core.h"
#include "../BasicFormatContext.h"

#define CPPTOOLS_FORMAT_DECLARED

namespace CPPTools::Fmt {

	template<typename CharFormat, typename CharBuffer, typename ...Args>
	class BasicFormatContext;

	template<typename T, typename FormatContext = BasicFormatContext<char, char>, typename IsEnable = T>
	struct FormatType {
		template<class K = T>
		static inline void Write(const K& t, FormatContext& context) {
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