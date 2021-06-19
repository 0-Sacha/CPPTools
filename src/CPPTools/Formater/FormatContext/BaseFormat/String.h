#pragma once

#include "../BasicFormatContext.h"

#include <string>
#include <string_view>
#include <sstream>

namespace CPPTools::Fmt {
	
	template<typename CharFormat, typename CharBuffer, typename ...ContextArgs>
		struct FormatType<std::basic_string<CharBuffer>, BasicFormatContext<CharFormat, CharBuffer, ContextArgs...>> {
		inline static void Write(const std::basic_string<CharBuffer>& t, BasicFormatContext<CharFormat, CharBuffer, ContextArgs...>& context) {
			context.BufferWriteCharType(t.c_str());
		}
	};

		template<typename CharFormat, typename CharBuffer, typename ...ContextArgs>
	struct FormatType<std::basic_string_view<CharBuffer>, BasicFormatContext<CharFormat, CharBuffer, ContextArgs...>> {
		inline static void Write(const std::basic_string_view<CharBuffer>& t, BasicFormatContext<CharFormat, CharBuffer, ContextArgs...>& context) {
			context.BufferWriteCharType(t.data(), t.size());
		}
	};

	template<typename CharFormat, typename CharBuffer, typename ...ContextArgs>
	struct FormatType<std::basic_stringstream<CharBuffer>, BasicFormatContext<CharFormat, CharBuffer, ContextArgs...>> {
		inline static void Write(const std::basic_stringstream<CharBuffer>& t, BasicFormatContext<CharFormat, CharBuffer, ContextArgs...>& context) {
			FormatType<std::basic_string<CharBuffer>>::Write(t.str(), context);
		}
	};
}