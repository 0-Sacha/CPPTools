#pragma once

#include "FormatType.h"

namespace CPPTools::Fmt {
	/////---------- string_view NamedArgs Do not allocate memory (Best) ----------/////
	template<typename T, typename CharName = char, typename CharFormatContext = char>
	struct FCStringViewNamedArgs
	{
		template<std::size_t SIZE>
		FCStringViewNamedArgs(const CharName (&name)[SIZE], const T& t)
			: m_Name(name), value(t) {}

		FCStringViewNamedArgs(const std::basic_string_view<CharName> name, const T& t)
			: m_Name(name), value(t) {}

	public:
		const T& GetValue() const								{ return value; }
		const std::basic_string_view<CharName> GetName() const	{ return m_Name; }

	protected:
		std::basic_string_view<CharName> m_Name;
		const T& value;
	};

	template<typename T, typename CharName, typename CharFormat, typename CharBuffer>
	struct FormatType<FCStringViewNamedArgs<T, CharName, CharFormat>, BasicFormatContext<CharFormat, CharBuffer>>
	{
		inline static void Write(const FCStringViewNamedArgs<T, CharName, CharFormat>& t, BasicFormatContext<CharFormat, CharBuffer>& context) {
			FormatType<GetBaseType<T>, BasicFormatContext<CharFormat, CharBuffer>>::Write(t.GetValue(), context);
		}
	};


	/////---------- stringNamedArgs Allocate memory (Only if necessary) ----------/////
	template<typename T, typename CharName = char, typename CharFormatContext = char>
	struct FCStringNamedArgs
	{
		FCStringNamedArgs(const std::string& str, const T& t)
			: m_Name(name), value(t) {}

		FCStringNamedArgs(std::string&& str, const T& t)
			: m_Name(std::move(name)), value(t) {}

	public:
		const T& GetValue() const								{ return value; }
		const std::basic_string<CharName>& GetName() const		{ return m_Name; }

	protected:
		const T& value;
		std::basic_string<CharName> m_Name;
	};

	template<typename T, typename CharName, typename CharFormat, typename CharBuffer>
	struct FormatType<FCStringNamedArgs<T, CharName, CharFormat>, BasicFormatContext<CharFormat, CharBuffer>>
	{
		inline static void Write(const FCStringNamedArgs<T, CharName, CharFormat>& t, BasicFormatContext<CharFormat, CharBuffer>& context) {
			FormatType<GetBaseType<T>, BasicFormatContext<CharFormat, CharBuffer>>::Write(t.GetValue(), context);
		}
	};
}


#define FORMAT(value)				CPPTools::Fmt::FCStringViewNamedArgs(#value, value)
#define FORMAT_CSTR(name, value)	CPPTools::Fmt::FCStringViewNamedArgs(name, value)

#define FORMAT_STR(name, value)		CPPTools::Fmt::FCStringNamedArgs(name, value)