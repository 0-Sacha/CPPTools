#pragma once

#include "../BasicFormatContext.h"

#include <string>
#include <string_view>
#include <sstream>

namespace CPPTools::Fmt {
	
	template<typename Char>
		struct FormatType<std::basic_string<Char>, BasicFormatContext<Char>> {
		inline static void Write(const std::basic_string<Char>& t, BasicFormatContext<Char>& context) {
			context.BufferWriteCharType(t.c_str());
		}
	};

	template<typename Char>
	struct FormatType<std::basic_string_view<Char>, BasicFormatContext<Char>> {
		inline static void Write(const std::basic_string_view<Char>& t, BasicFormatContext<Char>& context) {
			context.BufferWriteCharType(t.data(), t.size());
		}
	};

	template<typename Char>
	struct FormatType<std::basic_stringstream<Char>, BasicFormatContext<Char>> {
		inline static void Write(const std::basic_stringstream<Char>& t, BasicFormatContext<Char>& context) {
			FormatType<std::basic_string<Char>>::Write(t.str(), context);
		}
	};
}