#pragma once

#include "CPPTools/Formatter/Core/Detail.h"

#define CPPTOOLS_FORMAT_DECLARED

namespace CPPTools::Fmt {

	template<typename CharFormat, typename CharBuffer, typename ...ContextArgs>
	class BasicUnFormatContext;

	template<typename T, typename UnFormatContext = BasicUnFormatContext<char, char>, typename IsEnable = T>
	struct UnFormatType {
		template<class K = T>
		static inline auto Read(K& t, UnFormatContext& context) -> std::enable_if_t<!std::is_integral_v<K> && !std::is_floating_point_v<K>, bool> {
			CPPTOOLS_IF_MSVC(static_assert(false, __FUNCSIG__));
			return false;
		}
	};

}

#define CPPTOOLS_AUTO_UNFORMAT(Type, fmt, ...)	template<typename UnFormatContext>\
													struct CPPTools::Fmt::UnFormatType<Type, UnFormatContext> {\
														static bool Read(Type& value, UnFormatContext& context) {\
															return context.LittleUnFormat(fmt, __VA_ARGS__);\
														}\
													};\
												}








