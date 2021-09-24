#pragma once

#include "FormatType.h"

namespace CPPTools::Fmt {
	/////---------- string_view NamedArgs Do not allocate memory (Best) ----------/////
	template<typename T, typename CharName = char>
	struct StringViewNamedArgs
	{
		template<std::size_t SIZE>
		StringViewNamedArgs(const CharName(&name)[SIZE], T&& t)
			: m_Name(name), value(t) {}

		StringViewNamedArgs(const std::basic_string_view<CharName> name, T&& t)
			: m_Name(name), value(t) {}

		template<std::size_t SIZE>
		StringViewNamedArgs(const CharName(&name)[SIZE], T& t)
			: m_Name(name), value(t) {}

		StringViewNamedArgs(const std::basic_string_view<CharName> name, T& t)
			: m_Name(name), value(t) {}

	public:
		T& GetValue()											{ return value; }
		const T& GetValue() const								{ return value; }
		std::basic_string_view<CharName> GetName() const		{ return m_Name; }

	protected:
		std::basic_string_view<CharName> m_Name;
		T& value;
	};

	template<typename T, typename CharName, typename CharFormat, typename CharBuffer, typename ...ContextArgs>
	struct FormatType<StringViewNamedArgs<T, CharName>, BasicFormatContext<CharFormat, CharBuffer, ContextArgs...>>
	{
		inline static void Write(const StringViewNamedArgs<T, CharName>& t, BasicFormatContext<CharFormat, CharBuffer, ContextArgs...>& context) {
			FormatType<Detail::GetBaseType<T>, BasicFormatContext<CharFormat, CharBuffer, ContextArgs...>>::Write(t.GetValue(), context);
		}
	};


	/////---------- stringNamedArgs Allocate memory (Only if necessary) ----------/////
	template<typename T, typename CharName = char>
	struct StringNamedArgs
	{

		StringNamedArgs(const std::string& str, T&& t)
			: m_Name(str), value(t) {}

		StringNamedArgs(std::string&& str, T&& t)
			: m_Name(std::move(str)), value(t) {}


		StringNamedArgs(const std::string& str, T& t)
			: m_Name(str), value(t) {}

		StringNamedArgs(std::string&& str, T& t)
			: m_Name(std::move(str)), value(t) {}

	public:
		T& GetValue()											{ return value; }
		const T& GetValue() const								{ return value; }
		std::basic_string_view<CharName> GetName() const		{ return m_Name; }

	protected:
		std::basic_string<CharName> m_Name;
		T& value;
	};

	template<typename T, typename CharName, typename CharFormat, typename CharBuffer, typename ...ContextArgs>
	struct FormatType<StringNamedArgs<T, CharName>, BasicFormatContext<CharFormat, CharBuffer, ContextArgs...>>
	{
		inline static void Write(const StringNamedArgs<T, CharName>& t, BasicFormatContext<CharFormat, CharBuffer, ContextArgs...>& context) {
			FormatType<Detail::GetBaseType<T>, BasicFormatContext<CharFormat, CharBuffer, ContextArgs...>>::Write(t.GetValue(), context);
		}
	};


	namespace Detail {
		template <typename T>
		struct IsANamedArgs {
		public:
			inline constexpr static bool value = false;
		};

		template <typename T, typename CharName>
		struct IsANamedArgs<StringViewNamedArgs<T, CharName>> {
			inline constexpr static bool value = true;
		};

		template <typename T, typename CharName>
		struct IsANamedArgs<StringNamedArgs<T, CharName>> {
			inline constexpr static bool value = true;
		};

		template <typename T>
		inline constexpr bool IsANamedArgsValue = false;

		template <typename T, typename CharName>
		inline constexpr bool IsANamedArgsValue<StringViewNamedArgs<T, CharName>> = true;

		template <typename T, typename CharName>
		inline constexpr bool IsANamedArgsValue<StringNamedArgs<T, CharName>> = true;
	}
}


#define FORMAT(value)				CPPTools::Fmt::StringViewNamedArgs(#value, value)
#define FORMAT_SV(name, value)		CPPTools::Fmt::StringViewNamedArgs(name, value)

#define FORMAT_STR(name, value)		CPPTools::Fmt::StringNamedArgs(name, value)