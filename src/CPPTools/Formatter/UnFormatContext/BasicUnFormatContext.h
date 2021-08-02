#pragma once

#include "CPPTools/Formatter/Core/Detail.h"
#include "BaseFormat/UnFormatType.h"
#include "../FormatContext/BasicFormatContext.h"

#include "UnFormatContextArgsTuple.h"

namespace CPPTools::Fmt {

	struct UnFormatContextError
	{
		bool error					{false};
		std::int16_t FormatPosError	{0};
		std::int16_t BufferPosError	{0};
		UnFormatContextError()
			: error(false), FormatPosError(0), BufferPosError(0) { }
		UnFormatContextError(std::int16_t formatPosError, std::int16_t bufferPosError)
			: error(true), FormatPosError(formatPosError), BufferPosError(bufferPosError) { }
		inline operator bool() const { return error; }
	};

}

namespace CPPTools::Fmt {

	template<typename CharFormat = char, typename CharBuffer = CharFormat, typename ...ContextArgs>
	class BasicUnFormatContext
	{
	public:
		using CharFormatType = CharFormat;
		using CharBufferType = CharBuffer;

		using FormatDataType		= FormatData<CharFormat>;
		using FormatSpecifierType	= FormatSpecifier<CharFormat>;

		using StringViewFormat = std::basic_string_view<CharFormat>;
		using StringViewBuffer = std::basic_string_view<CharBuffer>;

		using M_Type = BasicUnFormatContext<CharFormat, CharBuffer, ContextArgs...>;

	public:
		BasicUnFormatContext(const std::basic_string_view<CharFormat> format, const std::basic_string_view<CharBuffer> buffer, ContextArgs&& ...args);

		template<typename ParentCharFormat, typename ...ParentContextArgs>
		BasicUnFormatContext(const std::basic_string_view<CharFormat> format, BasicUnFormatContext<ParentCharFormat, CharBuffer, ParentContextArgs...>& parentContext, ContextArgs&& ...args);

		template<typename ChildCharFormat, typename ...ChildContextArgs>
		inline void UpdateContextFromChild(BasicFormatContext<ChildCharFormat, CharBuffer, ChildContextArgs...>& childContext);

	private:
		Detail::FormatterMemoryBufferIn<CharBuffer>	m_BufferIn;
		Detail::FormatterMemoryFormat<CharFormat>	m_FormatStr;

		Detail::UnFormatContextArgsTuple<ContextArgs...>	m_ContextArgs;

		FormatIdx				m_ValuesIdx;
		FormatDataType			m_FormatData;
		Detail::AnsiColorMem	m_ColorMem;

	public:
		inline Detail::FormatterMemoryBufferIn<CharBuffer>&			BufferIn()			{ return m_BufferIn; }
		inline const Detail::FormatterMemoryBufferIn<CharBuffer>&	BufferIn() const	{ return m_BufferIn; }
		inline Detail::FormatterMemoryFormat<CharFormat>&			FormatStr()			{ return m_FormatStr; }
		inline const Detail::FormatterMemoryFormat<CharFormat>&		FormatStr() const	{ return m_FormatStr; }

		inline Detail::AnsiColorMem& GetColorMem()				{ return m_ColorMem; }
		inline const Detail::AnsiColorMem& GetColorMem() const	{ return m_ColorMem; }

		inline FormatDataType&			GetFormatData()				{ return m_FormatData; }
		inline const FormatDataType&	GetFormatData() const		{ return m_FormatData; }
		inline FormatDataType			ForwardFormatData() const	{ return m_FormatData; }

		static inline std::int16_t NoError()				{ return -1; }

	private:
		/////---------- Impl ----------/////
		bool GetFormatIdx(FormatIdx& idx);
		bool ParameterRead();
		void ParameterData();

	private:
		static void GetColorValue();
		static void GetTimerPrinted();
		static void GetDatePrinted();
		void IgnoreParameter();

	public:
		bool UnFormat();
		UnFormatContextError MainUnFormat();

		template<typename NewCharFormat, typename ...NewContextArgs>
		void LittleUnFormat(const std::basic_string_view<NewCharFormat> format, NewContextArgs&& ...args);
		template<typename CharType, std::size_t SIZE, typename ...NewContextArgs>
		inline void LittleFormat(const CharType(&format)[SIZE], NewContextArgs&& ...args)			{ LittleFormat(std::basic_string_view<CharType>(format), std::forward<NewContextArgs>(args)...); }

	public:
		template<typename T> bool FormatReadParameter(T& i);

	public:
		// Other Type from UnFormatType
		template<typename Type>							inline bool ReadType(Type& type) { return UnFormatType<Detail::GetBaseType<Type>, BasicUnFormatContext<CharFormat, CharBuffer, ContextArgs...>>::Read(type, *this); }

	public:
		inline bool FormatIsEndOfParameter()											{ return m_FormatStr.IsEqualTo('}'); }
		inline void FormatGoToEndOfParameter()											{ while (m_FormatStr.IsNotEqualTo('}') && m_FormatStr.CanMoveForward()) m_FormatStr.ForwardNoCheck(); }
		inline void FormatGoOutOfParameter()											{ while (m_FormatStr.IsNotEqualTo('}') && m_FormatStr.CanMoveForward()) m_FormatStr.ForwardNoCheck(); m_FormatStr.Forward(); }

		inline bool Check()																{ return m_BufferIn.IsEqualTo(m_FormatStr.Get()); }
		inline bool CheckAndNext()														{ if (m_BufferIn.IsEqualTo(m_FormatStr.Get())) { m_BufferIn.Forward(); m_FormatStr.Forward(); return true; } return false; }

		template<typename ...CharToTest>
		inline bool CheckUntilNextParameter(const CharToTest ...ele) {
			while (m_FormatStr.IsNotEqualTo('{', ele...) && m_FormatStr.CanMoveForward()) {
				if (!Check()) return false;
				m_FormatStr.ForwardNoCheck(); m_BufferIn.Forward();
			}
			return true;
		}
		template<typename ...CharToTest>
		inline bool CheckUntilEndOfParameter(const CharToTest ...ele) {
			while (m_FormatStr.IsNotEqualTo('}', ele...) && m_FormatStr.CanMoveForward()) {
				if (!Check()) return false;
				m_FormatStr.ForwardNoCheck(); m_BufferIn.Forward();
			}
			return true;
		}
	};
}