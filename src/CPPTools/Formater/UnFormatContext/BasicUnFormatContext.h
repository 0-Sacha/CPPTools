#pragma once

#include "CPPTools/Formater/Core/Core.h"

#include "BaseFormat/UnFormatType.h"

#include "../Core/FormaterHandler/FormaterHandler.h"
#include "../FormatContext/BasicFormatContext.h"

namespace CPPTools::Fmt {

	struct UnFormatContextError
	{
		bool error;
		std::int16_t FormatPosError;
		std::int16_t BufferPosError;
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
		BasicUnFormatContext(const std::basic_string_view<CharFormat> format, const std::basic_string_view<CharBuffer> buffer, ContextArgs&& ...args);

		template<typename OldCharFormat, typename ...OldContextArgs>
		BasicUnFormatContext(const std::basic_string_view<CharFormat> format, BasicUnFormatContext<OldCharFormat, CharBuffer, OldContextArgs...>& oldContext, ContextArgs&& ...args);


		template<typename OldCharFormat, typename ...OldContextArgs>
		inline void UpdateOldContext(BasicFormatContext<OldCharFormat, CharBuffer, OldContextArgs...>& oldContext) { oldContext.SetSubBuffer(m_SubBuffer); }

	private:
		const CharBuffer* const		m_Buffer;
		const CharBuffer*			m_SubBuffer;
		const CharBuffer* const		m_BufferEnd;
		std::size_t					m_BufferSize;

		const CharFormat*	m_Format;
		const CharFormat*	m_SubFormat;
		const CharFormat*	m_FormatEnd;
		std::size_t			m_FormatSize;

		std::tuple<ContextArgs...>	m_ContextArgs;
		std::size_t					m_ContextArgsSize;

		FormatIdx				m_ValuesIdx;
		Detail::AnsiColorMem	m_ColorMem;
		FormatData				m_FormatData;

	public:
		inline CharBuffer*			GetBuffer()								{ return m_Buffer; }
		inline const CharBuffer*	GetBuffer() const						{ return m_Buffer; }
		inline CharBuffer*			GetSubBuffer()							{ return m_SubBuffer; }
		inline const CharBuffer*	GetSubBuffer() const					{ return m_SubBuffer; }
		inline CharBuffer*			GetBufferEnd()							{ return m_BufferEnd; }
		inline const CharBuffer*	GetBufferEnd() const					{ return m_BufferEnd; }
		inline std::size_t			GetBufferSize() const					{ return m_BufferSize; }
		inline std::size_t			GetCurrentBufferSize() const			{ return m_SubBuffer - m_Buffer; }
		inline void					SetSubBuffer(CharBuffer* const pos)		{ m_SubBuffer = pos; }

		inline CharFormat*			GetFormat()								{ return m_Format; }
		inline const CharFormat*	GetFormat() const						{ return m_Format; }
		inline CharFormat*			GetSubFormat()							{ return m_SubFormat; }
		inline const CharFormat*	GetSubFormat() const					{ return m_SubFormat; }
		inline CharFormat*			GetFormatEnd()							{ return m_FormatEnd; }
		inline const CharFormat*	GetFormatEnd() const					{ return m_FormatEnd; }
		inline std::size_t			GetFormatSize() const					{ return m_FormatSize; }
		inline std::size_t			GetCurrentFormatSize() const			{ return m_SubFormat - m_Format; }
		inline void					SetSubFormat(CharFormat* const pos)		{ m_SubFormat = pos; }

		inline Detail::AnsiColorMem& GetColorMem()			{ return m_ColorMem; }
		inline FormatData& GetFormatData()					{ return m_FormatData; }
		inline const FormatData& GetFormatData() const		{ return m_FormatData; }
		inline FormatData ForwardFormatData() const			{ return m_FormatData; }

		inline std::int16_t NoError()						{ return -1; }

	private:

		/////---------- Read Rec ----------/////
		void UnFormatTypeFromIdxRec(FormatIdx idx);
		template<typename T, typename ...Args>
		void UnFormatTypeFromIdxRec(FormatIdx idx, T& t, Args&& ...args);

		/////---------- Data Print Rec ----------/////
		inline void GetParameterDataRec(FormatIdx idx);
		template<typename T, typename ...Args>
		inline void GetParameterDataRec(FormatIdx idx, const T& t, Args&& ...args);


		/////---------- Get NamedArgs ----------/////
		void GetNamedArgsIdxRec(FormatIdx& idx, FormatIdx currentIdx);
		template<typename T, typename ...Args, typename CharName>
		void GetNamedArgsIdxRec(FormatIdx& idx, FormatIdx currentIdx, const StringViewNamedArgs<T, CharName, CharFormat>& t, Args&& ...args);
		template<typename T, typename ...Args, typename CharName>
		void GetNamedArgsIdxRec(FormatIdx& idx, FormatIdx currentIdx, const StringNamedArgs<T, CharName, CharFormat>& t, Args&& ...args);
		template<typename T, typename ...Args>
		void GetNamedArgsIdxRec(FormatIdx& idx, FormatIdx currentIdx, const T& t, Args&& ...args);
		template<typename CharToTest>
		bool FormatNextIsANamedArgs(std::basic_string_view<CharToTest> sv);

		/////---------- Impl ----------/////
		bool GetFormatIdx(FormatIdx& i);
		bool ParameterRead();
		void ParameterData();

	private:
		void GetColorValue();
		void GetTimerPrinted();
		void GetDatePrinted();
		void IgnoreParameter();

	public:
		bool UnFormat();
		UnFormatContextError MainUnFormat();

		template<typename NewCharFormat, typename ...NewContextArgs>
		void LittleUnFormat(const std::basic_string_view<NewCharFormat> format, NewContextArgs&& ...args);
		template<typename CharType, std::size_t SIZE, typename ...NewContextArgs>
		inline void LittleFormat(const CharType(&format)[SIZE], NewContextArgs&& ...args)						{ LittleFormat(std::basic_string_view<CharType>(format), std::forward<NewContextArgs>(args)...); }

	public:
		template<typename T> bool FormatReadInt(T& i);
		template<typename T> bool FormatReadUInt(T& i);

	public:
		template<typename T> bool BufferReadInt(T& i);
		template<typename T> bool BufferReadUInt(T& i);
		template<typename T> bool FormatReadParameter(T& i);

		template<typename T> bool BufferReadFloat(T& i);

		template<std::size_t SIZE>
		inline bool BufferReadCharType(CharBuffer(&str)[SIZE])							{ BufferReadCharType(str, SIZE); }
		inline bool BufferReadCharType(CharBuffer* str, std::size_t size)				{ while (size-- != 0) *str++ = BufferGetAndForward(); }
		inline bool BufferReadCharType(CharBuffer* str)									{ return false; }

	public:
		// Format
		inline bool FormatCanMoveForward() const									{ return m_SubFormat < m_FormatEnd; }
		inline bool FormatCanMoveBackward() const									{ return m_SubFormat > m_Format; }
		inline bool FormatIsNotOutOfBound() const									{ return !FormatCanMoveForward() || !FormatCanMoveBackward(); }
		inline bool FormatEnd() const												{ return m_SubFormat >= m_FormatEnd; }

		// Format base commands
		inline void FormatForward()													{ if (FormatCanMoveForward()) ++m_SubFormat; }
		inline void FormatForwardNoCheck()											{ ++m_SubFormat; }
		inline void FormatBackward()												{ if (FormatCanMoveBackward()) --m_SubFormat; }
		inline void FormatBackwardNoCheck()											{ --m_SubFormat; }
		inline void FormatForward(const std::size_t size)							{ m_SubFormat += size; if (!FormatCanMoveForward()) m_SubFormat = m_FormatEnd; }
		inline void FormatBackward(const std::size_t size)							{ m_SubFormat -= size; if (!FormatCanMoveBackward()) m_SubFormat = m_Format; }
		
		inline CharFormat FormatGet() const											{ return *m_SubFormat; }
		inline CharFormat FormatGetAndForward()										{ return FormatCanMoveForward() ? *m_SubFormat++ : '\0'; }
		inline CharFormat FormatGetAndForwardNoCheck()								{ return *m_SubFormat++; }
		inline CharFormat FormatGetAndBackward()									{ return FormatCanMoveBackward() ? *m_SubFormat-- : '\0'; }
		inline CharFormat FormatGetNext() const										{ return FormatCanMoveForward() ? *(m_SubFormat + 1) : '\0'; }

		// Format check
		inline bool FormatIsEqualTo(const CharFormat c) const						{ return FormatGet() == c; }
		inline bool FormatIsNotEqualTo(const CharFormat c) const					{ return FormatGet() != c; }
		inline bool FormatIsEqualForward(const CharFormat c)						{ if (FormatIsEqualTo(c)) { FormatForward(); return true; } return false; }
		inline bool FormatIsNotEqualForward(const CharFormat c)						{ if (FormatIsNotEqualTo(c)) { FormatForward(); return true; } return false; }
		inline bool FormatNextIsEqualTo(const CharFormat c) const					{ return FormatGetNext() == c; }
		inline bool FormatNextIsNotEqualTo(const CharFormat c) const				{ return FormatGetNext() != c; }
		inline bool FormatNextIsEqualForward(const CharFormat c)					{ if (FormatNextIsEqualTo(c)) { FormatForward(); return true; } return false; }
		inline bool FormatNextIsNotEqualForward(const CharFormat c)					{ if (FormatNextIsNotEqualTo(c)) { FormatForward(); return true; } return false; }

		template<typename CharToTest>
		bool FormatNextIsSame(std::basic_string_view<CharToTest> sv);
		template<std::size_t SIZE, typename CharToTest>
		inline bool FormatNextIsSame(const CharToTest(&data)[SIZE])					{ return FormatNextIsSame(std::basic_string_view<CharToTest>(data)); }

		inline bool FormatIsLowerCase() const										{ return FormatGet() >= 'a' && FormatGet() <= 'z'; }
		inline bool FormatIsUpperCase() const										{ return FormatGet() >= 'A' && FormatGet() <= 'Z'; }
		inline bool FormatIsADigit() const											{ return FormatGet() >= '0' && FormatGet() <= '9'; }

		// Format commands
		inline void FormatIgnoreSpace()												{ while (FormatIsEqualTo(' ') && FormatCanMoveForward()) FormatForwardNoCheck(); }

		inline void FormatGoTo(const CharFormat c)														{ while (FormatIsNotEqualTo(c) && FormatCanMoveForward())														FormatForwardNoCheck(); }
		inline void FormatGoTo(const CharFormat c0, const CharFormat c1)								{ while (FormatIsNotEqualTo(c0) && FormatIsNotEqualTo(c1) && FormatCanMoveForward())							FormatForwardNoCheck(); }
		inline void FormatGoTo(const CharFormat c0, const CharFormat c1, const CharFormat c2)			{ while (FormatIsNotEqualTo(c0) && FormatIsNotEqualTo(c1) && FormatIsNotEqualTo(c2) && FormatCanMoveForward())	FormatForwardNoCheck(); }
		inline void FormatGoToForward(const CharFormat c)												{ while (FormatIsNotEqualTo(c) && FormatCanMoveForward())														FormatForwardNoCheck();	FormatForward(); }
		inline void FormatGoToForward(const CharFormat c0, const CharFormat c1)							{ while (FormatIsNotEqualTo(c0) && FormatIsNotEqualTo(c1) && FormatCanMoveForward())							FormatForwardNoCheck();	FormatForward(); }
		inline void FormatGoToForward(const CharFormat c0, const CharFormat c1, const CharFormat c2)	{ while (FormatIsNotEqualTo(c0) && FormatIsNotEqualTo(c1) && FormatIsNotEqualTo(c2) && FormatCanMoveForward())	FormatForwardNoCheck();	FormatForward(); }

		// Format commands in parameter (add check to '}' to avoid skip the end of the format specifier)
		inline void FormatParamGoTo(const CharFormat c)														{ while (FormatIsNotEqualTo('}') && FormatIsNotEqualTo(c) && FormatCanMoveForward())														FormatForwardNoCheck(); }
		inline void FormatParamGoTo(const CharFormat c0, const CharFormat c1)								{ while (FormatIsNotEqualTo('}') && FormatIsNotEqualTo(c0) && FormatIsNotEqualTo(c1) && FormatCanMoveForward())								FormatForwardNoCheck(); }
		inline void FormatParamGoTo(const CharFormat c0, const CharFormat c1, const CharFormat c2)			{ while (FormatIsNotEqualTo('}') && FormatIsNotEqualTo(c0) && FormatIsNotEqualTo(c1) && FormatIsNotEqualTo(c2) && FormatCanMoveForward())	FormatForwardNoCheck(); }
		inline void FormatParamGoToForward(const CharFormat c)												{ while (FormatIsNotEqualTo('}') && FormatIsNotEqualTo(c) && FormatCanMoveForward())														FormatForwardNoCheck();	FormatForward(); }
		inline void FormatParamGoToForward(const CharFormat c0, const CharFormat c1)						{ while (FormatIsNotEqualTo('}') && FormatIsNotEqualTo(c0) && FormatIsNotEqualTo(c1) && FormatCanMoveForward())								FormatForwardNoCheck();	FormatForward(); }
		inline void FormatParamGoToForward(const CharFormat c0, const CharFormat c1, const CharFormat c2)	{ while (FormatIsNotEqualTo('}') && FormatIsNotEqualTo(c0) && FormatIsNotEqualTo(c1) && FormatIsNotEqualTo(c2) && FormatCanMoveForward())	FormatForwardNoCheck();	FormatForward(); }

	public:
		// Buffer
		inline bool BufferCanMoveForward() const									{ return m_SubBuffer < m_BufferEnd; }
		inline bool BufferCanMoveBackward() const									{ return m_SubBuffer > m_Buffer; }
		inline bool BufferIsNotOutOfBound() const									{ return !BufferCanMoveForward() || !BufferCanMoveBackward(); }
		inline bool BufferEnd() const												{ return m_SubBuffer >= m_BufferEnd; }

		// Buffer base commands
		inline void BufferForward()													{ if (BufferCanMoveForward()) ++m_SubBuffer; }
		inline void BufferForwardNoCheck()											{ ++m_SubBuffer; }
		inline void BufferBackward()												{ if (BufferCanMoveBackward()) --m_SubBuffer; }
		inline void BufferBackwardNoCheck()											{ --m_SubBuffer; }
		inline void BufferForward(const std::size_t size)							{ m_SubBuffer += size; if (!BufferCanMoveForward()) m_SubBuffer = m_BufferEnd; }
		inline void BufferBackward(const std::size_t size)							{ m_SubBuffer -= size; if (!BufferCanMoveBackward()) m_SubBuffer = m_Buffer; }
		
		inline CharBuffer BufferGet() const											{ return *m_SubBuffer; }
		inline CharBuffer BufferGetAndForward()										{ return BufferCanMoveForward() ? *m_SubBuffer++ : '\0'; }
		inline CharBuffer BufferGetAndForwardNoCheck()								{ return *m_SubBuffer++; }
		inline CharBuffer BufferGetAndBackward()									{ return BufferCanMoveBackward() ? *m_SubBuffer-- : '\0'; }
		inline CharBuffer BufferGetNext() const										{ return BufferCanMoveForward() ? *(m_SubBuffer + 1) : '\0'; }

		// Buffer check
		inline bool BufferIsEqualTo(const CharBuffer c) const						{ return BufferGet() == c; }
		inline bool BufferIsNotEqualTo(const CharBuffer c) const					{ return BufferGet() != c; }
		inline bool BufferIsEqualForward(const CharBuffer c)						{ if (BufferIsEqualTo(c)) { BufferForward(); return true; } return false; }
		inline bool BufferIsNotEqualForward(const CharBuffer c)						{ if (BufferIsNotEqualTo(c)) { BufferForward(); return true; } return false; }
		inline bool BufferNextIsEqualTo(const CharBuffer c) const					{ return BufferGetNext() == c; }
		inline bool BufferNextIsNotEqualTo(const CharBuffer c) const				{ return BufferGetNext() != c; }
		inline bool BufferNextIsEqualForward(const CharBuffer c)					{ if (BufferNextIsEqualTo(c)) { BufferForward(); return true; } return false; }
		inline bool BufferNextIsNotEqualForward(const CharBuffer c)					{ if (BufferNextIsNotEqualTo(c)) { BufferForward(); return true; } return false; }

		template<typename CharToTest>
		bool BufferNextIsSame(std::basic_string_view<CharToTest> sv);
		template<std::size_t SIZE, typename CharToTest>
		inline bool BufferNextIsSame(const CharToTest(&data)[SIZE])					{ return BufferNextIsSame(std::basic_string_view<CharToTest>(data)); }

		inline bool BufferIsLowerCase() const										{ return BufferGet() >= 'a' && BufferGet() <= 'z'; }
		inline bool BufferIsUpperCase() const										{ return BufferGet() >= 'A' && BufferGet() <= 'Z'; }
		inline bool BufferIsADigit() const											{ return BufferGet() >= '0' && BufferGet() <= '9'; }

		// Buffer commands
		inline void BufferIgnoreSpace()												{ while (BufferIsEqualTo(' ') && BufferCanMoveForward()) BufferForwardNoCheck(); }

		inline void BufferGoTo(const CharBuffer c)														{ while (BufferIsNotEqualTo(c) && BufferCanMoveForward())														BufferForwardNoCheck(); }
		inline void BufferGoTo(const CharBuffer c0, const CharBuffer c1)								{ while (BufferIsNotEqualTo(c0) && BufferIsNotEqualTo(c1) && BufferCanMoveForward())							BufferForwardNoCheck(); }
		inline void BufferGoTo(const CharBuffer c0, const CharBuffer c1, const CharBuffer c2)			{ while (BufferIsNotEqualTo(c0) && BufferIsNotEqualTo(c1) && BufferIsNotEqualTo(c2) && BufferCanMoveForward())	BufferForwardNoCheck(); }
		inline void BufferGoToForward(const CharBuffer c)												{ while (BufferIsNotEqualTo(c) && BufferCanMoveForward())														BufferForwardNoCheck();	BufferForward(); }
		inline void BufferGoToForward(const CharBuffer c0, const CharBuffer c1)							{ while (BufferIsNotEqualTo(c0) && BufferIsNotEqualTo(c1) && BufferCanMoveForward())							BufferForwardNoCheck();	BufferForward(); }
		inline void BufferGoToForward(const CharBuffer c0, const CharBuffer c1, const CharBuffer c2)	{ while (BufferIsNotEqualTo(c0) && BufferIsNotEqualTo(c1) && BufferIsNotEqualTo(c2) && BufferCanMoveForward())	BufferForwardNoCheck();	BufferForward(); }

	public:
		inline bool FormatIsEndOfParameter()											{ return FormatIsEqualTo('}'); }
		inline void FormatGoToEndOfParameter()											{ while (FormatIsNotEqualTo('}') && FormatCanMoveForward()) FormatForwardNoCheck(); }
		inline void FormatGoOutOfParameter()											{ while (FormatIsNotEqualTo('}') && FormatCanMoveForward()) FormatForwardNoCheck(); FormatForward(); }

		inline bool Check()																{ return BufferIsEqualTo(FormatGet()); }
		inline bool CheckAndNext()														{ if (BufferIsEqualTo(FormatGet())) { BufferForward(); FormatForward(); return true; } return false; }

		inline bool CheckUntilNextParameter() {
			while (FormatIsNotEqualTo('{') && FormatCanMoveForward()) {
				if (!Check()) return false;
				FormatForwardNoCheck(); BufferForward();
			}
			return true;
		}

		inline bool CheckUntilEndOfParameter() {
			while (FormatIsNotEqualTo('}') && FormatCanMoveForward()) {
				if (!Check()) return false;
				FormatForwardNoCheck(); BufferForward();
			}
			return true;
		}
		inline bool CheckUntilEndOfParameterOr(const CharFormat c) {
			while (FormatIsNotEqualTo('}') && FormatIsNotEqualTo(c) && FormatCanMoveForward()) {
				if (!Check()) return false;
				FormatForwardNoCheck(); BufferForward();
			}
			return true;
		}
		inline bool CheckUntilEndOfParameterOr(const CharFormat c0, const CharFormat c1) {
			while (FormatIsNotEqualTo('}') && FormatIsNotEqualTo(c0) && FormatIsNotEqualTo(c1) && FormatCanMoveForward()) {
				if (!Check()) return false;
				FormatForwardNoCheck(); BufferForward();
			}
			return true;
		}
		inline bool CheckUntilEndOfParameterOr(const CharFormat c0, const CharFormat c1, const CharFormat c2) {
			while (FormatIsNotEqualTo('}') && FormatIsNotEqualTo(c0) && FormatIsNotEqualTo(c1) && FormatIsNotEqualTo(c2) && FormatCanMoveForward()) {
				if (!Check()) return false;
				FormatForwardNoCheck(); BufferForward();
			}
			return true;
		}
	};
}