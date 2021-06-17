#pragma once

#include "FormatType.h"

namespace CPPTools::Fmt {

	/////---------- NamedArgs ----------/////
	template<typename T, typename CharFormatContext = char>
	struct FCNamedArgs
	{
	public:
		FCNamedArgs(const T& t)
			: m_Value(t) {}

	public:
		virtual bool IsRightNameImpl(const CharFormatContext* str) const = 0;
		inline bool IsRightName(const CharFormatContext* str) const {
			return IsRightNameImpl(str);
		}

	public:
		inline const T& GetValue() const { return m_Value; }

	protected:
		const T& m_Value;
	};

	template<typename T, typename CharFormat, typename CharBuffer>
	struct FormatType<FCNamedArgs<T, CharFormat>, BasicFormatContext<CharFormat, CharBuffer>>
	{
		inline static void Write(const FCNamedArgs<T>& t, BasicFormatContext<CharFormat, CharBuffer>& context) {
			FormatType<GetBaseType<T>, BasicFormatContext<CharFormat, CharBuffer>>::Write(t.GetValue(), context);
		}
	};
}


namespace CPPTools::Fmt {
	/////---------- C-style CStringNamedArgs ----------/////
	template<typename T, typename CharName = char, typename CharFormatContext = char>
	struct FCCStringNamedArgs : public FCNamedArgs<T, CharFormatContext>
	{

		FCCStringNamedArgs(const CharName* const name, const T& t)
			: FCNamedArgs(t), m_Name(name) {}

	public:
		bool IsRightNameImpl(const CharFormatContext* str) const override {
			const CharName* subName = m_Name;
			while (*str++ == *subName++);
			--subName;
			return *subName == 0;
		}

	protected:
		const CharName* m_Name;
	};

	template<typename T, typename CharName, typename CharFormat, typename CharBuffer>
	struct FormatType<FCCStringNamedArgs<T, CharName, CharFormat>, BasicFormatContext<CharFormat, CharBuffer>>
	{
		inline static void Write(const FCCStringNamedArgs<T, CharName, CharFormat>& t, BasicFormatContext<CharFormat, CharBuffer>& context) {
			FormatType<GetBaseType<T>, BasicFormatContext<CharFormat, CharBuffer>>::Write(t.GetValue(), context);
		}
	};


	/////---------- c++-style StringNamedArgs ----------/////
	template<typename T, typename CharName = char, typename CharFormatContext = char>
	struct FCStringNamedArgs : public FCNamedArgs<T, CharFormatContext>
	{

		FCStringNamedArgs(const std::basic_string<CharName>& name, const T& t)
			: FCNamedArgs(t), m_Name(name) {}

		FCStringNamedArgs(std::basic_string<CharName>&& name, const T& t)
			: FCNamedArgs(t), m_Name(std::move(name)) {}

	public:
		bool IsRightNameImpl(const CharFormatContext* str) const override {
			const CharName* subName = m_Name.data();
			while (*str++ == *subName++);
			--subName;
			return *subName == 0;
		}

	protected:
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


#define FORMAT(value)				CPPTools::Fmt::FCCStringNamedArgs(#value, value)
#define FORMAT_CSTR(name, value)	CPPTools::Fmt::FCCStringNamedArgs(name, value)

#define FORMAT_STR(name, value)		CPPTools::Fmt::FCStringNamedArgs(name, value)