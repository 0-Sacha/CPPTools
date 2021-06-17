#pragma once

#include "FormatType.h"

namespace CPPTools::Fmt {

	/////---------- NamedArgs ----------/////
	template<typename T, typename Char = char>
	struct FCNamedArgs
	{
	public:
		FCNamedArgs(const T& t)
			: m_Value(t) {}

	public:
		virtual bool IsRightNameImpl(const Char* str) const = 0;
		inline bool IsRightName(const Char* str) const {
			return IsRightNameImpl(str);
		}

	public:
		inline const T& GetValue() const { return m_Value; }

	protected:
		const T& m_Value;
	};

	template<typename T, typename Char>
	struct FormatType<FCNamedArgs<T, Char>, BasicFormatContext<Char>>
	{
		inline static void Write(const FCNamedArgs<T, Char>& t, BasicFormatContext<Char>& context) {
			FormatType<GetBaseType<T>, BasicFormatContext<Char>>::Write(t.GetValue(), context);
		}
	};
}


namespace CPPTools::Fmt {
	/////---------- C-style CStringNamedArgs ----------/////
	template<typename T, typename Char = char>
	struct FCCStringNamedArgs : public FCNamedArgs<T, Char>
	{

		FCCStringNamedArgs(const Char* const name, const T& t)
			: FCNamedArgs(t), m_Name(name) {}

	public:
		bool IsRightNameImpl(const Char* str) const override {
			const Char* subName = m_Name;
			while (*str++ == *subName++);
			--subName;
			return *subName == 0;
		}

	protected:
		const Char* m_Name;
	};

	template<typename T, typename Char>
	struct FormatType<FCCStringNamedArgs<T, Char>, BasicFormatContext<Char>>
	{
		inline static void Write(const FCCStringNamedArgs<T, Char>& t, BasicFormatContext<Char>& context) {
			FormatType<GetBaseType<T>, BasicFormatContext<Char>>::Write(t.GetValue(), context);
		}
	};


	/////---------- c++-style StringNamedArgs ----------/////
	template<typename T, typename Char = char>
	struct FCStringNamedArgs : public FCNamedArgs<T, Char>
	{

		FCStringNamedArgs(const std::basic_string<Char>& name, const T& t)
			: FCNamedArgs(t), m_Name(name) {}

		FCStringNamedArgs(std::basic_string<Char>&& name, const T& t)
			: FCNamedArgs(t), m_Name(std::move(name)) {}

	public:
		bool IsRightNameImpl(const Char* str) const override {
			const Char* subName = m_Name.data();
			while (*str++ == *subName++);
			--subName;
			return *subName == 0;
		}

	protected:
		std::basic_string<Char> m_Name;
	};

	template<typename T, typename Char>
	struct FormatType<FCStringNamedArgs<T, Char>, BasicFormatContext<Char>>
	{
		inline static void Write(const FCStringNamedArgs<T, Char>& t, BasicFormatContext<Char>& context) {
			FormatType<GetBaseType<T>, BasicFormatContext<Char>>::Write(t.GetValue(), context);
		}
	};
}


#define FORMAT(value)				CPPTools::Fmt::FCCStringNamedArgs(#value, value)
#define FORMAT_CSTR(name, value)	CPPTools::Fmt::FCCStringNamedArgs(name, value)

#define FORMAT_STR(name, value)		CPPTools::Fmt::FCStringNamedArgs(name, value)