#pragma once

#include "CPPTools/Formater/Core/Core.h"

#define CPPTOOLS_FORMAT_DECLARED

namespace CPPTools::Fmt {

	template<typename CharFormat, typename CharBuffer, typename ...ContextArgs>
	class BasicUnFormatContext;

	template<typename T, typename UnFormatContext = BasicUnFormatContext<char, char>, typename IsEnable = T>
	struct UnFormatType {
		template<class K = T>
		static inline auto Read(K& t, UnFormatContext& context) -> std::enable_if_t<!std::is_integral_v<K> && !std::is_floating_point_v<K>, bool> {
			static_assert(false, __FUNCSIG__);
			return false;
		}
	};

}

#define CPPTOOLS_AUTO_UNFORMAT(Type, fmt, ...)	namespace CPPTools::Fmt {\
													template<typename UnFormatContext>\
													struct UnFormatType<Type, UnFormatContext> {\
														static bool Read(Type& value, UnFormatContext& context) {\
															return context.LittleUnFormat(fmt, __VA_ARGS__);\
														}\
													};\
												}