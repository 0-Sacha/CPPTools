#pragma once


#include "CPPTools/Core/Core.h"

namespace CPPTools::Fmt {

	/////---------- NamedArgs ----------/////
	template<typename T>
	struct NamedArgs
	{

		NamedArgs(T& t)
			: m_Value(t) {}

	public:
		virtual bool IsRightNameImpl(const char* str) const = 0;
		inline bool IsRightName(const char* str) const {
			return IsRightNameImpl(str);
		}

	public:
		inline T& GetValue() { return m_Value; }
		inline const T& GetValue() const { return m_Value; }

	protected:
		T& m_Value;
	};

	template<typename T>
	struct FormatType<NamedArgs<T>>
	{
		inline static void Write(const NamedArgs<T>& t, Formater& formater) {
			FormatType<GetBaseType<T>>::Write(t.GetValue(), formater);
		}

		inline static bool Read(const NamedArgs<T>& t, UnFormater& formater) {
			return false;
		}
	};
}


namespace CPPTools::Fmt {
	/////---------- CharPtNamedArgs ----------/////
	template<typename T>
	struct CharPtNamedArgs : public NamedArgs<T>
	{

		CharPtNamedArgs(const char* const name, T& t)
			: NamedArgs(t), m_Name(name) {}

	public:
		bool IsRightNameImpl(const char* str) const override {
			const char* subName = m_Name;
			while (*str++ == *subName++);
			--subName;
			return *subName == 0;
		}

	protected:
		const char* m_Name;
	};

	template<typename T>
	struct FormatType<CharPtNamedArgs<T>>
	{
		inline static void Write(const CharPtNamedArgs<T>& t, Formater& formater) {
			FormatType<GetBaseType<T>>::Write(t.GetValue(), formater);
		}

		inline static bool Read(const CharPtNamedArgs<T>& t, UnFormater& formater) {
			return false;
		}
	};


	/////---------- StringNamedArgs ----------/////
	template<typename T>
	struct StringNamedArgs : public NamedArgs<T>
	{

		StringNamedArgs(const std::string& name, T& t)
			: NamedArgs(t), m_Name(name) {}

		StringNamedArgs(std::string&& name, T& t)
			: NamedArgs(t), m_Name(std::move(name)) {}

	public:
		bool IsRightNameImpl(const char* str) const override {
			const char* subName = m_Name.data();
			while (*str++ == *subName++);
			--subName;
			return *subName == 0;
		}

	protected:
		std::string m_Name;
	};

	template<typename T>
	struct FormatType<StringNamedArgs<T>>
	{
		inline static void Write(const StringNamedArgs<T>& t, Formater& formater) {
			FormatType<GetBaseType<T>>::Write(t.GetValue(), formater);
		}

		inline static bool Read(const StringNamedArgs<T>& t, UnFormater& formater) {
			return false;
		}
	};
}


#define FORMAT(value)				CPPTools::Fmt::CharPtNamedArgs(#value, value)
#define FORMAT_CSTR(name, value)	CPPTools::Fmt::CharPtNamedArgs(name, value)

#define FORMAT_STR(name, value)		CPPTools::Fmt::StringNamedArgs(name, value)