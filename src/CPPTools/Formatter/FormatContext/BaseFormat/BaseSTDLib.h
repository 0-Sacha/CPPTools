#pragma once

#include "../BasicFormatContext.h"

#include <string>
#include <string_view>
#include <sstream>

namespace CPPTools::Fmt {

	//------------------------------------------//
	//----------------- String -----------------//
	//------------------------------------------//

	// String
	template<typename CharFormat, typename CharBuffer, typename ...ContextArgs>
	struct FormatType<std::basic_string<CharBuffer>, BasicFormatContext<CharFormat, CharBuffer, ContextArgs...>> {
		inline static void Write(const std::basic_string<CharBuffer>& t, BasicFormatContext<CharFormat, CharBuffer, ContextArgs...>& context) {
			context.BufferOut().WriteCharPt(t.c_str(), t.size());
		}
	};

	// StringView
	template<typename CharFormat, typename CharBuffer, typename ...ContextArgs>
	struct FormatType<std::basic_string_view<CharBuffer>, BasicFormatContext<CharFormat, CharBuffer, ContextArgs...>> {
		inline static void Write(const std::basic_string_view<CharBuffer>& t, BasicFormatContext<CharFormat, CharBuffer, ContextArgs...>& context) {
			context.BufferOut().WriteCharPt(t.data(), t.size());
		}
	};

	// StringStream
	template<typename CharFormat, typename CharBuffer, typename ...ContextArgs>
	struct FormatType<std::basic_stringstream<CharBuffer>, BasicFormatContext<CharFormat, CharBuffer, ContextArgs...>> {
		inline static void Write(const std::basic_stringstream<CharBuffer>& t, BasicFormatContext<CharFormat, CharBuffer, ContextArgs...>& context) {
			FormatType<std::basic_string<CharBuffer>, BasicFormatContext<CharFormat, CharBuffer, ContextArgs...>>::Write(t.str(), context);
		}
	};



	//------------------------------------------//
	//----------------- Memory -----------------//
	//------------------------------------------//

	// UniquePtr
	template <typename T, typename FormatContext>
	struct FormatType<std::unique_ptr<T>, FormatContext> {
		inline static void Write(const std::unique_ptr<T>& t, FormatContext& context) {
			if (context.GetFormatData().BaseValue)	FormatType<T*, FormatContext>::Write(t.get(), context);
			else									FormatType<T, FormatContext>::Write(*t, context);
		}
	};

	// SharedPtr
	template <typename T, typename FormatContext>
	struct FormatType<std::shared_ptr<T>, FormatContext> {
		inline static void Write(const std::shared_ptr<T>& t, FormatContext& context) {
			if (context.GetFormatData().BaseValue)	FormatType<T*, FormatContext>::Write(t.get(), context);
			else									FormatType<T, FormatContext>::Write(*t, context);
		}
	};

	// WeakPtr
	template <typename T, typename FormatContext>
	struct FormatType<std::weak_ptr<T>, FormatContext> {
		inline static void Write(const std::weak_ptr<T>& t, FormatContext& context) {
			FormatType<std::shared_ptr<T>, FormatContext>::Write(t.lock(), context);
		}
	};
}