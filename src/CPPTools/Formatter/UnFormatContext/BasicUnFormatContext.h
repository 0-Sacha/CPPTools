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

		Detail::UnFormatContextArgsTuple<ContextArgs...>	m_ContextArgs;
		std::size_t											m_ContextArgsSize;

		FormatIdx				m_ValuesIdx;
		Detail::AnsiColorMem	m_ColorMem;
		FormatData				m_FormatData;

	public:
		inline const CharBuffer*	GetBuffer() const						{ return m_Buffer; }
		inline const CharBuffer*	GetSubBuffer() const					{ return m_SubBuffer; }
		inline const CharBuffer*	GetBufferEnd() const					{ return m_BufferEnd; }
		inline std::size_t			GetBufferSize() const					{ return m_BufferSize; }
		inline std::size_t			GetCurrentBufferSize() const			{ return m_SubBuffer - m_Buffer; }
		inline void					SetSubBuffer(CharBuffer* const pos)		{ m_SubBuffer = pos; }

		inline const CharFormat*	GetFormat() const						{ return m_Format; }
		inline const CharFormat*	GetSubFormat() const					{ return m_SubFormat; }
		inline const CharFormat*	GetFormatEnd() const					{ return m_FormatEnd; }
		inline std::size_t			GetFormatSize() const					{ return m_FormatSize; }
		inline std::size_t			GetCurrentFormatSize() const			{ return m_SubFormat - m_Format; }
		inline void					SetSubFormat(CharFormat* const pos)		{ m_SubFormat = pos; }

		inline Detail::AnsiColorMem& GetColorMem()			{ return m_ColorMem; }
		inline FormatData& GetFormatData()					{ return m_FormatData; }
		inline const FormatData& GetFormatData() const		{ return m_FormatData; }
		inline FormatData ForwardFormatData() const			{ return m_FormatData; }

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
		template<typename T> bool FormatReadInt(T& i);
		template<typename T> bool FormatReadUInt(T& i);

		template<typename T> bool FormatReadParameter(T& i);

	public:
		// Integer
		template<typename T> bool BufferReadInt(T& i);
		template<typename T> bool BufferReadUInt(T& i);

		// Floating point
		template<typename T> bool BufferReadFloat(T& i);
		
		// C-Style String
		template<typename CharStr> 						inline bool BufferReadCharPt(CharStr* str, std::size_t size);
		template<typename CharStr> 						inline bool BufferReadCharPt(CharStr* str);

		// C-Style String wrapper
		template<typename CharStr, std::size_t SIZE> 	inline bool BufferReadCharArray(CharStr(&str)[SIZE])			{ return BufferReadCharPt(str, SIZE); }

		// Other Type from UnFormatType
		template<typename Type>							inline bool ReadType(Type& type) { return UnFormatType<Detail::GetBaseType<Type>, BasicUnFormatContext<CharFormat, CharBuffer, ContextArgs...>>::Read(type, *this); }

	public:
		// Format
		inline bool FormatCanMoveForward() const									{ return m_SubFormat < m_FormatEnd; }
		inline bool FormatCanMoveBackward() const									{ return m_SubFormat > m_Format; }
		inline bool FormatCanMoveForward(const std::size_t count) const				{ return m_SubFormat + count <= m_FormatEnd; }
		inline bool FormatCanMoveBackward(const std::size_t count) const			{ return m_SubFormat + count >= m_Format; }
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
		inline CharFormat FormatGetAndBackwardNoCheck()								{ return *m_SubFormat--; }
		inline CharFormat FormatGetNext() const										{ return FormatCanMoveForward() ? *(m_SubFormat + 1) : '\0'; }
		inline CharFormat FormatGetNextNoCheck() const								{ return *(m_SubFormat + 1); }

		// Format check
		inline bool FormatIsEqualTo(const CharFormat c) const						{ return FormatGet() == c; }
		inline bool FormatIsNotEqualTo(const CharFormat c) const					{ return FormatGet() != c; }
		inline bool FormatIsEqualForward(const CharFormat c)						{ if (FormatIsEqualTo(c)) { FormatForward(); return true; } return false; }
		inline bool FormatIsNotEqualForward(const CharFormat c)						{ if (FormatIsNotEqualTo(c)) { FormatForward(); return true; } return false; }
		template<typename ...CharToTest> 	inline bool FormatIsEqualTo(const CharFormat c, const CharToTest ...ele) const		{ return FormatIsEqualTo(c) || FormatIsEqualTo(ele...); }
		template<typename ...CharToTest> 	inline bool FormatIsEqualForward(const CharToTest ...ele) const						{ if (FormatIsEqualTo(ele...)) { FormatForward(); return true; } return false; }
		template<typename ...CharToTest> 	inline bool FormatIsNotEqualTo(const CharFormat c, const CharToTest ...ele) const	{ return FormatIsNotEqualTo(c) && FormatIsNotEqualTo(ele...); }
		template<typename ...CharToTest> 	inline bool FormatIsNotEqualForward(const CharToTest ...ele) const					{ if (FormatIsNotEqualTo(ele...)) { FormatForward(); return true; } return false; }

		template<typename CharToTest>		bool FormatNextIsANamedArgs(std::basic_string_view<CharToTest> sv);


		// Format Next check
		inline bool FormatNextIsEqualTo(const CharFormat c) const					{ return FormatGetNext() == c; }
		inline bool FormatNextIsNotEqualTo(const CharFormat c) const				{ return FormatGetNext() != c; }
		inline bool FormatNextIsEqualForward(const CharFormat c)					{ FormatForward(); if(!FormatIsEqualTo(c)) { FormatBackwardNoCheck(); return false; } return true; }
		inline bool FormatNextIsNotEqualForward(const CharFormat c)					{ FormatForward(); if (!FormatIsNotEqualTo(c)) { FormatBackwardNoCheck(); return false; } return true; }
		template<typename ...CharToTest> 	inline bool FormatNextIsEqualForward(const CharToTest ...ele)		{ FormatForward(); if (FormatIsEqualTo(ele...)) { return true; } FormatBackwardNoCheck(); return false; }
		template<typename ...CharToTest> 	inline bool FormatNextIsEqualTo(const CharToTest ...ele) const		{ FormatForward(); if (FormatIsEqualTo(ele...)) { FormatBackwardNoCheck(); return true; } FormatBackwardNoCheck(); return false; }
		template<typename ...CharToTest> 	inline bool FormatNextIsNotEqualForward(const CharToTest ...ele)	{ FormatForward(); if (FormatIsNotEqualTo(ele...)) { return true; } FormatBackwardNoCheck(); return false; }
		template<typename ...CharToTest> 	inline bool FormatNextIsNotEqualTo(const CharToTest ...ele) const	{ FormatForward(); if (FormatIsNotEqualTo(ele...)) { FormatBackwardNoCheck(); return true; } FormatBackwardNoCheck(); return false; }
		
		template<typename CharToTest>
		bool FormatNextIsSame(std::basic_string_view<CharToTest> sv);
		template<std::size_t SIZE, typename CharToTest>
		inline bool FormatNextIsSame(const CharToTest(&data)[SIZE])					{ return FormatNextIsSame(std::basic_string_view<CharToTest>(data)); }

		inline bool FormatIsLowerCase() const										{ return FormatGet() >= 'a' && FormatGet() <= 'z'; }
		inline bool FormatIsUpperCase() const										{ return FormatGet() >= 'A' && FormatGet() <= 'Z'; }
		inline bool FormatIsADigit() const											{ return FormatGet() >= '0' && FormatGet() <= '9'; }

		// Format commands
		inline void FormatIgnoreSpace()												{ while (FormatIsEqualTo(' ') && FormatCanMoveForward()) FormatForwardNoCheck(); }
		template<typename ...CharToTest> inline void FormatGoTo(const CharToTest ...ele)				{ while (FormatIsNotEqualTo(ele...) && FormatCanMoveForward())	FormatForwardNoCheck(); }
		template<typename ...CharToTest> inline void FormatGoToForward(const CharToTest ...ele)			{ while (FormatIsNotEqualTo(ele...) && FormatCanMoveForward())	FormatForwardNoCheck();	FormatForward(); }
	
		// Format commands in parameter (add check to '}' to avoid skip the end of the format specifier)
		template<typename ...CharToTest> inline void FormatParamGoTo(const CharToTest ...ele)			{ while (FormatIsNotEqualTo(ele..., '}') && FormatCanMoveForward())	FormatForwardNoCheck(); }
		template<typename ...CharToTest> inline void FormatParamGoToForward(const CharToTest ...ele)	{ while (FormatIsNotEqualTo(ele..., '}') && FormatCanMoveForward())	FormatForwardNoCheck();	FormatForward(); }

	public:
		// Buffer
		inline bool BufferCanMoveForward() const									{ return m_SubBuffer < m_BufferEnd; }
		inline bool BufferCanMoveBackward() const									{ return m_SubBuffer > m_Buffer; }
		inline bool BufferCanMoveForward(const std::size_t count) const				{ return m_SubBuffer + count <= m_BufferEnd; }
		inline bool BufferCanMoveBackward(const std::size_t count) const			{ return m_SubBuffer + count >= m_Buffer; }
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
		inline CharBuffer BufferGetAndBackwardNoCheck()								{ return *m_SubBuffer--; }
		inline CharBuffer BufferGetNext() const										{ return BufferCanMoveForward() ? *(m_SubBuffer + 1) : '\0'; }
		inline CharBuffer BufferGetNextNoCheck() const								{ return *(m_SubBuffer + 1); }

		// Buffer check
		inline bool BufferIsEqualTo(const CharBuffer c) const						{ return BufferGet() == c; }
		inline bool BufferIsNotEqualTo(const CharBuffer c) const					{ return BufferGet() != c; }
		inline bool BufferIsEqualForward(const CharBuffer c)						{ if (BufferIsEqualTo(c)) { BufferForward(); return true; } return false; }
		inline bool BufferIsNotEqualForward(const CharBuffer c)						{ if (BufferIsNotEqualTo(c)) { BufferForward(); return true; } return false; }
		template<typename ...CharToTest> inline bool BufferIsEqualTo(const CharBuffer c, const CharToTest ...ele) const		{ return BufferIsEqualTo(c) || BufferIsEqualTo(ele...); }
		template<typename ...CharToTest> inline bool BufferIsEqualForward(const CharToTest ...ele) const					{ if (BufferIsEqualTo(ele...)) { BufferForward(); return true; } return false; }
		template<typename ...CharToTest> inline bool BufferIsNotEqualTo(const CharBuffer c, const CharToTest ...ele) const	{ return BufferIsNotEqualTo(c) && BufferIsNotEqualTo(ele...); }
		template<typename ...CharToTest> inline bool BufferIsNotEqualForward(const CharToTest ...ele) const					{ if (BufferIsNotEqualTo(ele...)) { BufferForward(); return true; } return false; }

		// Buffer Next check
		inline bool BufferNextIsEqualTo(const CharBuffer c) const					{ return BufferGetNext() == c; }
		inline bool BufferNextIsNotEqualTo(const CharBuffer c) const				{ return BufferGetNext() != c; }
		inline bool BufferNextIsEqualForward(const CharBuffer c)					{ BufferForward(); if(!BufferIsEqualTo(c)) { BufferBackwardNoCheck(); return false; } return true; }
		inline bool BufferNextIsNotEqualForward(const CharBuffer c)					{ BufferForward(); if (!BufferIsNotEqualTo(c)) { BufferBackwardNoCheck(); return false; } return true; }
		template<typename ...CharToTest> inline bool BufferNextIsEqualForward(const CharToTest ...ele)		{ BufferForward(); if (BufferIsEqualTo(ele...)) { return true; } BufferBackwardNoCheck(); return false; }
		template<typename ...CharToTest> inline bool BufferNextIsEqualTo(const CharToTest ...ele) const		{ BufferForward(); if (BufferIsEqualTo(ele...)) { BufferBackwardNoCheck(); return true; } BufferBackwardNoCheck(); return false; }
		template<typename ...CharToTest> inline bool BufferNextIsNotEqualForward(const CharToTest ...ele)	{ BufferForward(); if (BufferIsNotEqualTo(ele...)) { return true; } BufferBackwardNoCheck(); return false; }
		template<typename ...CharToTest> inline bool BufferNextIsNotEqualTo(const CharToTest ...ele) const	{ BufferForward(); if (BufferIsNotEqualTo(ele...)) { BufferBackwardNoCheck(); return true; } BufferBackwardNoCheck(); return false; }
		
		template<typename CharToTest>
		bool BufferNextIsSame(std::basic_string_view<CharToTest> sv);
		template<std::size_t SIZE, typename CharToTest>
		inline bool BufferNextIsSame(const CharToTest(&data)[SIZE])					{ return BufferNextIsSame(std::basic_string_view<CharToTest>(data)); }

		inline bool BufferIsLowerCase() const										{ return BufferGet() >= 'a' && BufferGet() <= 'z'; }
		inline bool BufferIsUpperCase() const										{ return BufferGet() >= 'A' && BufferGet() <= 'Z'; }
		inline bool BufferIsADigit() const											{ return BufferGet() >= '0' && BufferGet() <= '9'; }

		// Buffer commands
		inline void BufferIgnoreSpace()												{ while (BufferIsEqualTo(' ') && BufferCanMoveForward()) BufferForwardNoCheck(); }
		template<typename ...CharToTest> inline void BufferGoTo(const CharToTest ...ele)				{ while (BufferIsNotEqualTo(ele...) && BufferCanMoveForward())	BufferForwardNoCheck(); }
		template<typename ...CharToTest> inline void BufferGoToForward(const CharToTest ...ele)			{ while (BufferIsNotEqualTo(ele...) && BufferCanMoveForward())	BufferForwardNoCheck();	BufferForward(); }

	public:
		inline bool FormatIsEndOfParameter()											{ return FormatIsEqualTo('}'); }
		inline void FormatGoToEndOfParameter()											{ while (FormatIsNotEqualTo('}') && FormatCanMoveForward()) FormatForwardNoCheck(); }
		inline void FormatGoOutOfParameter()											{ while (FormatIsNotEqualTo('}') && FormatCanMoveForward()) FormatForwardNoCheck(); FormatForward(); }

		inline bool Check()																{ return BufferIsEqualTo(FormatGet()); }
		inline bool CheckAndNext()														{ if (BufferIsEqualTo(FormatGet())) { BufferForward(); FormatForward(); return true; } return false; }

		template<typename ...CharToTest>
		inline bool CheckUntilNextParameter(const CharToTest ...ele) {
			while (FormatIsNotEqualTo('{', ele...) && FormatCanMoveForward()) {
				if (!Check()) return false;
				FormatForwardNoCheck(); BufferForward();
			}
			return true;
		}
		template<typename ...CharToTest>
		inline bool CheckUntilEndOfParameter(const CharToTest ...ele) {
			while (FormatIsNotEqualTo('}', ele...) && FormatCanMoveForward()) {
				if (!Check()) return false;
				FormatForwardNoCheck(); BufferForward();
			}
			return true;
		}
	};
}