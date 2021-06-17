#pragma once

#include "CPPTools/Formater/Core/Core.h"

#define CPPTOOLS_FORMAT_DECLARED

namespace CPPTools::Fmt {

	template<typename Char>
	class BasicUnFormatContext;

	template<typename T, typename UnFormatContext = BasicUnFormatContext<char>>
	struct UnFormatType {
		static bool Read(T& i, UnFormatContext& formater) {
			static_assert(false, __FUNCSIG__);
			return false;
		}
	};

}

#define CPPTOOLS_AUTO_UNFORMAT(Type, fmt, ...)	namespace CPPTools::Fmt {\
													template<typename UnFormatContext>\
													struct FormatType<Type> {\
														static bool Read(Type& value, UnFormatContext& formater) {\
															return formater.LittleUnFormat(fmt, __VA_ARGS__);\
														}\
													};\
												}