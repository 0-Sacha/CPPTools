#pragma once

#include "UnFormatType.h"

namespace CPPTools::Fmt {

	/////---------- NamedArgs ----------/////
	template<typename T, typename UnFormatContext = BasicUnFormatContext<char>>
	struct UFCNamedArgs
	{
	public:
		UFCNamedArgs(T& t)
			: m_Value(t) {}

	public:
		virtual bool IsRightNameImpl(const typename UnFormatContext::CharType* str) const = 0;
		inline bool IsRightName(const typename UnFormatContext::CharType* str) const {
			return IsRightNameImpl(str);
		}

	public:
		inline T& GetValue() { return m_Value; }

	protected:
		T& m_Value;
	};

	template<typename T, typename UnFormatContext>
	struct UnFormatType<UFCNamedArgs<T, UnFormatContext>, UnFormatContext>
	{
		inline static bool Read(const UFCNamedArgs<T, UnFormatContext>& t, UnFormatContext& context) {
			return FormatType<GetBaseType<T>>::Write(t.GetValue(), context);
		}
	};
}


namespace CPPTools::Fmt {
	/////---------- C-style CStringNamedArgs ----------/////
	template<typename T, typename UnFormatContext = BasicUnFormatContext<char>>
	struct UFCCStringNamedArgs : public UFCNamedArgs<T, UnFormatContext>
	{

		UFCCStringNamedArgs(const typename UnFormatContext::CharType* const name, T& t)
			: UFCNamedArgs(t), m_Name(name) {}

	public:
		bool IsRightNameImpl(const typename UnFormatContext::CharType* str) const override {
			const typename UnFormatContext::CharType* subName = m_Name;
			while (*str++ == *subName++);
			--subName;
			return *subName == 0;
		}

	protected:
		const typename UnFormatContext::CharType* m_Name;
	};

	template<typename T, typename UnFormatContext>
	struct UnFormatType<UFCCStringNamedArgs<T, UnFormatContext>, UnFormatContext>
	{
		inline static bool Read(const UFCCStringNamedArgs<T, UnFormatContext>& t, UnFormatContext& context) {
			return FormatType<GetBaseType<T>>::Write(t.GetValue(), context);
		}
	};


	/////---------- c++-style StringNamedArgs ----------/////
	template<typename T, typename UnFormatContext = BasicUnFormatContext<char>>
	struct UFCStringNamedArgs : public UFCNamedArgs<T, UnFormatContext>
	{

		UFCStringNamedArgs(const std::basic_string<typename UnFormatContext::CharType>& name, T& t)
			: UFCNamedArgs(t), m_Name(name) {}

		UFCStringNamedArgs(std::basic_string<typename UnFormatContext::CharType>&& name, T& t)
			: UFCNamedArgs(t), m_Name(std::move(name)) {}

	public:
		bool IsRightNameImpl(const typename UnFormatContext::CharType* str) const override {
			const typename UnFormatContext::CharType* subName = m_Name.data();
			while (*str++ == *subName++);
			--subName;
			return *subName == 0;
		}

	protected:
		std::basic_string<typename UnFormatContext::CharType> m_Name;
	};

	template<typename T, typename UnFormatContext>
	struct UnFormatType<UFCStringNamedArgs<T, UnFormatContext>, UnFormatContext>
	{
		inline static bool Read(const UFCStringNamedArgs<T, UnFormatContext>& t, UnFormatContext& context) {
			return FormatType<GetBaseType<T>>::Write(t.GetValue(), context);
		}
	};
}


#define UNFORMAT(value)					CPPTools::Fmt::UFCCStringNamedArgs(#value, value)
#define UNFORMAT_CSTR(name, value)		CPPTools::Fmt::UFCCStringNamedArgs(name, value)

#define UNFORMAT_STR(name, value)		CPPTools::Fmt::UFCStringNamedArgs(name, value)