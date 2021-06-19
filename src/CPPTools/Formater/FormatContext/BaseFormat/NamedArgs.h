#pragma once

#include "FormatType.h"

namespace CPPTools::Fmt {
	/////---------- string_view NamedArgs Do not allocate memory (Best) ----------/////
	template<typename T, typename CharName = char, typename CharFormatContext = char>
	struct StringViewNamedArgs
	{
		template<std::size_t SIZE>
		StringViewNamedArgs(const CharName (&name)[SIZE], const T& t)
			: m_Name(name), value(t) {}

		StringViewNamedArgs(const std::basic_string_view<CharName> name, const T& t)
			: m_Name(name), value(t) {}

	public:
		const T& GetValue() const								{ return value; }
		const std::basic_string_view<CharName> GetName() const	{ return m_Name; }

	protected:
		std::basic_string_view<CharName> m_Name;
		const T& value;
	};

	template<typename T, typename CharName, typename CharFormat, typename CharBuffer, typename ...ContextArgs>
	struct FormatType<StringViewNamedArgs<T, CharName, CharFormat>, BasicFormatContext<CharFormat, CharBuffer, ContextArgs...>>
	{
		inline static void Write(const StringViewNamedArgs<T, CharName, CharFormat>& t, BasicFormatContext<CharFormat, CharBuffer, ContextArgs...>& context) {
			FormatType<GetBaseType<T>, BasicFormatContext<CharFormat, CharBuffer, ContextArgs...>>::Write(t.GetValue(), context);
		}
	};


	/////---------- stringNamedArgs Allocate memory (Only if necessary) ----------/////
	template<typename T, typename CharName = char, typename CharFormatContext = char>
	struct StringNamedArgs
	{
		StringNamedArgs(const std::string& str, const T& t)
			: m_Name(name), value(t) {}

		StringNamedArgs(std::string&& str, const T& t)
			: m_Name(std::move(name)), value(t) {}

	public:
		const T& GetValue() const								{ return value; }
		const std::basic_string<CharName>& GetName() const		{ return m_Name; }

	protected:
		const T& value;
		std::basic_string<CharName> m_Name;
	};

	template<typename T, typename CharName, typename CharFormat, typename CharBuffer, typename ...ContextArgs>
	struct FormatType<StringNamedArgs<T, CharName, CharFormat>, BasicFormatContext<CharFormat, CharBuffer, ContextArgs...>>
	{
		inline static void Write(const StringNamedArgs<T, CharName, CharFormat>& t, BasicFormatContext<CharFormat, CharBuffer, ContextArgs...>& context) {
			FormatType<GetBaseType<T>, BasicFormatContext<CharFormat, CharBuffer, ContextArgs...>>::Write(t.GetValue(), context);
		}
	};
}


#define FORMAT(value)				CPPTools::Fmt::StringViewNamedArgs(#value, value)
#define FORMAT_CSTR(name, value)	CPPTools::Fmt::StringViewNamedArgs(name, value)

#define FORMAT_STR(name, value)		CPPTools::Fmt::StringNamedArgs(name, value)