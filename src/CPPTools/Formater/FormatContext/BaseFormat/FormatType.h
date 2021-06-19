#pragma once

#include "CPPTools/Formater/Core/Core.h"
#include "../BasicFormatContext.h"

#define CPPTOOLS_FORMAT_DECLARED

namespace CPPTools::Fmt {

	template<typename CharFormat, typename CharBuffer, typename ...Args>
	class BasicFormatContext;

	template<typename T, typename FormatContext = BasicFormatContext<char, char>>
	struct FormatType {
		template<class K = T>
		static inline auto Write(const K t, FormatContext& context) -> std::enable_if_t<std::is_floating_point_v<K>> {
			context.BufferWriteFloat(t);
		}

		template<class K = T>
		static inline auto Write(const K t, FormatContext& context) -> std::enable_if_t<std::is_integral_v<K> && std::is_signed_v<K>> {
			context.BufferWriteInt(t);
		}

		template<class K = T>
		static inline auto Write(const K t, FormatContext& context) -> std::enable_if_t<std::is_integral_v<K>&& std::is_unsigned_v<K>> {
			context.BufferWriteUInt(t);
		}

		template<class K = T>
		static inline auto Write(const K& t, FormatContext& context) -> std::enable_if_t<!std::is_integral_v<K> && !std::is_floating_point_v<K>> {
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