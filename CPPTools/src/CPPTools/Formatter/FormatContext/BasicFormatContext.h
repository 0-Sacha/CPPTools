#pragma once

#include "CPPTools/Formatter/Core/Detail.h"
#include "CPPTools/Formatter/Core/Buffer/Buffer.h"

#include "BaseFormat/FormatType.h"
#include "BaseFormat/NamedArgs.h"
#include "BaseFormat/FormatArgs.h"
#include "BaseFormat/STDEnumerable.h"

#include "FormatContextArgsTuple.h"

#include "../Core/FormatterHandler.h"

namespace CPPTools::Fmt {
	template<typename CharFormat = char, typename CharBuffer = CharFormat, typename ...ContextArgs>
	class BasicFormatContext {
	public:
		using CharFormatType = CharFormat;
		using CharBufferType = CharBuffer;

		using FormatDataType		= FormatData<CharFormat>;
		using FormatSpecifierType	= FormatSpecifier<CharFormat>;

		using StringViewFormat = std::basic_string_view<CharFormat>;
		using StringViewBuffer = std::basic_string_view<CharBuffer>;

		using M_Type = BasicFormatContext<CharFormat, CharBuffer, ContextArgs...>;

	public:
		BasicFormatContext(const std::basic_string_view<CharFormat> format, CharBuffer* const buffer, const std::size_t bufferSize, ContextArgs &&...args);
		BasicFormatContext(const bool bufferIsAutoResize, const std::basic_string_view<CharFormat> format, ContextArgs &&...args);

		template<typename ParentCharFormat, typename ...ParentContextArgs>
		BasicFormatContext(const std::basic_string_view<CharFormat> format, BasicFormatContext<ParentCharFormat, CharBuffer, ParentContextArgs...> &parentContext, ContextArgs &&...args);

		template<typename ChildCharFormat, typename ...ChildContextArgs>
		inline void UpdateContextFromChild(BasicFormatContext<ChildCharFormat, CharBuffer, ChildContextArgs...>& childContext);

	private:
		Detail::FormatterMemoryBufferOut<CharBuffer>	m_BufferOut;
		Detail::FormatterMemoryFormat<CharFormat>		m_FormatStr;

		Detail::FormatContextArgsTuple<ContextArgs...> 	m_ContextArgs;

		// Stride (mostly for container and new line format-style)
		std::size_t 			m_NoStride;

		// For handling color / format data and idx (for not specified parameter)
		FormatIdx						m_ValuesIdx;
		FormatDataType					m_FormatData;
		Detail::AnsiTextCurrentColor	m_AnsiTextCurrentColor;
		Detail::AnsiTextCurrentStyle	m_AnsiTextCurrentStyle;
		Detail::AnsiTextCurrentFront	m_AnsiTextCurrentFront;
		Detail::AnsiFormatterChange		m_AnsiFormatterChange;

	public:
		inline Detail::FormatterMemoryBufferOut<CharBuffer>&			BufferOut()			{ return m_BufferOut; }
		inline const Detail::FormatterMemoryBufferOut<CharBuffer>&		BufferOut() const	{ return m_BufferOut; }
		inline Detail::FormatterMemoryFormat<CharFormat>&				FormatStr()			{ return m_FormatStr; }
		inline const Detail::FormatterMemoryFormat<CharFormat>&			FormatStr() const	{ return m_FormatStr; }

		inline Detail::AnsiTextCurrentColor&		GetAnsiTextCurrentColor()		{ return m_AnsiTextCurrentColor; }
		inline const Detail::AnsiTextCurrentColor&	GetAnsiTextCurrentColor() const	{ return m_AnsiTextCurrentColor; }
		inline Detail::AnsiTextCurrentStyle&		GetAnsiTextCurrentStyle()		{ return m_AnsiTextCurrentStyle; }
		inline const Detail::AnsiTextCurrentStyle&	GetAnsiTextCurrentStyle() const	{ return m_AnsiTextCurrentStyle; }
		inline Detail::AnsiTextCurrentFront&		GetAnsiTextCurrentFront()		{ return m_AnsiTextCurrentFront; }
		inline const Detail::AnsiTextCurrentFront&	GetAnsiTextCurrentFront() const { return m_AnsiTextCurrentFront; }

		inline Detail::AnsiFormatterChange&			GetAnsiFormatterChange()		{ return m_AnsiFormatterChange; }
		inline const Detail::AnsiFormatterChange&	GetAnsiFormatterChange() const	{ return m_AnsiFormatterChange; }

		inline FormatData<CharFormat>&			GetFormatData()				{ return m_FormatData; }
		inline const FormatData<CharFormat>&	GetFormatData() const		{ return m_FormatData; }
		inline FormatData<CharFormat>			ForwardFormatData() const	{ return m_FormatData; }

		inline void AddNoStride(const std::size_t noStride)		{ m_NoStride += noStride; }
		inline std::size_t GetNoStride() const					{ return m_NoStride; }
		inline std::size_t GetStride() const					{ return m_BufferOut.GetBufferCurrentSize() - m_NoStride; }
		inline std::size_t StrideGetBufferCurrentSize() const	{ return m_BufferOut.GetBufferCurrentSize(); }

	public:
		inline static FormatterHandler& GetAPI()				{ return FormatterHandler::GetInstance(); }

	private:
		bool GetFormatIdx(FormatIdx& idx);
		bool ParameterPrint();
		void ParameterData();

	public:
		void Format();
		void MainFormat();

		template<typename NewCharFormat, typename ...NewContextArgs>
		void LittleFormat(const std::basic_string_view<NewCharFormat> format, NewContextArgs&& ...args);
		template<typename CharType, std::size_t SIZE, typename ...NewContextArgs>
		inline void LittleFormat(const CharType (&format)[SIZE], NewContextArgs&& ...args)				{ LittleFormat(std::basic_string_view<CharType>(format), std::forward<NewContextArgs>(args)...); }

	private:
		template<typename CharList, std::size_t SIZE>
		std::uint8_t GetWordFromList(const std::basic_string_view<CharList> (&formatTypes)[SIZE]);

		void CheckEndStr();

		void ReadAnsiTextColorParameter();
		std::uint8_t GetColorCode();
		std::uint8_t GetColorFG();
		std::uint8_t GetColorBG();

		void ReadTimerParameter();
		void ReadDateParameter();
		
		void ReadAnsiTextStyleParameter();
		void WriteStyleParameter();

		void ReadAnsiTextFrontParameter();
		std::uint8_t GetFrontCode();

	public:
		void ReloadColor(const Detail::AnsiTextCurrentColor& targetColor, const Detail::AnsiTextColorChange& changeColor);
		void ReloadStyle(const Detail::AnsiTextCurrentStyle& targetStyle, const Detail::AnsiTextStyleChange& changeStyle);
		void ReloadFront(const Detail::AnsiTextCurrentFront& targetFront, const Detail::AnsiTextFrontChange& changeFront);

	public:
		// Unsigned-Integer Only
		template<typename T> bool FormatReadParameter(T& i);

	public:
		// Type formating from FormatType<>
		template<typename Type>
		inline void WriteType(Type&& type)							{ FormatType<Detail::GetBaseType<Type>, BasicFormatContext<CharFormat, CharBuffer, ContextArgs...>>::Write(type, *this); }
		template<typename Type, typename ...Rest>
		inline void WriteType(Type&& type, const Rest&& ...rest)	{ WriteType(type); WriteType(std::forward<Rest>(rest)...); }

		// only support basic type that are considered as basic by the BufferOut class
		template<typename Type>
		inline void BasicWriteType(Type&& type)						{ m_BufferOut.BasicWriteType(type); }
		template<typename Type, typename ...Rest>
		inline void BasicWriteType(Type&& type, Rest&& ...rest)		{ m_BufferOut.BasicWriteType(type); BasicWriteType(std::forward<Rest>(rest)...); }



		template<typename CharStr>						inline void PrintCharPt(const CharStr* str)						{ m_BufferOut.WriteCharPt(str); }
		template<typename CharStr>						inline void PrintCharPt(const CharStr* str, std::size_t size)	{ m_BufferOut.WriteCharPt(str, size); }
		template<typename CharStr, std::size_t SIZE>	inline void Print(const CharStr(&str)[SIZE])					{ m_BufferOut.WriteCharPt(str, SIZE); }
		template<typename CharStr>						inline void Print(const std::basic_string_view<CharStr> str)	{ m_BufferOut.WriteCharPt(str.data(), str.size()); }

	public:
		inline void CopyFormatToBuffer() { m_BufferOut.PushBack(m_FormatStr.GetAndForward()); }

		template<typename ...CharToTest> inline void WriteUntilNextParameter(const CharToTest ...ele)	{ while (m_FormatStr.IsNotEqualTo('{', ele...) && m_FormatStr.CanMoveForward())	CopyFormatToBuffer(); }
		template<typename ...CharToTest> inline void WriteUntilEndOfParameter(const CharToTest ...ele)	{ while (m_FormatStr.IsNotEqualTo('}', ele...) && m_FormatStr.CanMoveForward())	CopyFormatToBuffer(); }

	};
}