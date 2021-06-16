#pragma once

#include "../Core/Core.h"
#include "../BaseFormat/FormatType.h"
#include "../BaseFormat/NamedArgs.h"
#include "../BaseFormat/FormatArgs.h"

#include "../Core/FormaterHandler/FormaterHandler.h"

namespace CPPTools::Fmt {

	class Formater {

	public:
		Formater(const std::string_view format, char* const buffer, const std::size_t bufferSize);

	private:
		// Buffer
		char* const			m_Buffer;
		char*				m_SubBuffer;
		char* const			m_BufferEnd;	// Point one behind the end char
		std::size_t			m_BufferSize;	// The real allocated size

		// Format
		const char* m_Format;				
		const char* m_SubFormat;
		const char* m_FormatEnd;			// Point to the end char of the format
		std::size_t m_FormatSize;			// Do not count the end char

		// Stride (mostly for container and new line format-style)
		std::size_t m_NoStride = 0;

		// For handling color / format data and idx (for not specified parameter)
		std::uint8_t			m_ValuesIdx = 0;
		FormatData				m_FormatData;
		Detail::AnsiColorMem	m_ColorMem;

	public:
		inline const char* GetBuffer() const				{ return m_Buffer; }
		inline std::size_t GetBufferSize() const			{ return m_BufferSize; }
		inline std::size_t GetCurrentBufferSize() const		{ return m_SubBuffer - m_Buffer; }

		inline const char* GetFormat() const				{ return m_Format; }
		inline std::size_t GetFormatSize() const			{ return m_FormatSize; }
		inline std::size_t GetCurrentFormatDist() const		{ return m_SubFormat - m_Format; }

		inline Detail::AnsiColorMem& GetColorMem()			{ return m_ColorMem; }
		inline FormatData& GetFormatData()					{ return m_FormatData; }
		inline const FormatData& GetFormatData() const		{ return m_FormatData; }
		inline FormatData ForwardFormatData() const			{ return m_FormatData; }

		inline void AddNoStride(const std::size_t noStride) { m_NoStride += noStride; }
		inline std::size_t GetStride() const				{ return GetCurrentBufferSize() - m_NoStride; }

	public:
		inline static FormaterHandler& GetAPI()			{ return FormaterHandler::GetInstance(); }

	private:
		/////---------- Default Print Rec ----------/////
		void FormatPrintRec(std::uint8_t idx)								{}
		template<typename T, typename ...Args>
		void FormatPrintRec(std::uint8_t idx, const T& t, Args&& ...args);

		/////---------- NamedArgs Print Rec ----------/////
		void FormatPrintRecNamedArgs(const char* const name)						{}
		template<typename T, typename ...Args>
		void FormatPrintRecNamedArgs(const char* const name, const NamedArgs<T>& t, Args&& ...args);
		template<typename T, typename ...Args>
		void FormatPrintRecNamedArgs(const char* const name, const CharPtNamedArgs<T>& t, Args&& ...args);
		template<typename T, typename ...Args>
		void FormatPrintRecNamedArgs(const char* const name, const StringNamedArgs<T>& t, Args&& ...args);
		template<typename T, typename ...Args>
		void FormatPrintRecNamedArgs(const char* const name, const T& t, Args&& ...args);

		/////---------- Data Print Rec ----------/////
		void ParameterDataRec(std::uint8_t idx)										{}
		template<typename T, typename ...Args>
		void ParameterDataRec(std::uint8_t idx, const T& t, Args&& ...args);


		/////---------- Impl ----------/////
		template<typename ...Args>
		void ParameterType(Args&& ...args);
		template<typename ...Args>
		void ParameterData(Args&& ...args);

	public:
		template<typename ...Args>
		void Format(Args&& ...args);
		template<typename ...Args>
		void LittleFormat(const std::string_view format, Args&& ...args);
		template<typename ...Args>
		void MainFormat(Args&& ...args);

	public:
		bool GetFormatType(const char* formatTypes[], FormatSpecifierIDX* arr, std::uint8_t size);
		std::uint8_t GetWordFromList(const char* formatTypes[], std::uint8_t count);

	private:
		void CheckEndStr();
		void ColorValuePrint();
		std::uint8_t GetColorCode();
		std::uint8_t GetColorFG();
		std::uint8_t GetColorBG();
		void TimerValuePrint();
		void DateValuePrint();
		void ReloadColor();

	public:
		template<typename T>
		bool FormatReadInt(T& i);
		template<typename T>
		bool FormatReadUInt(T& i);

	public:
		// Integer
		template<typename T>
		void BufferParseInt(T i);
		template<typename T>
		void BufferParseUInt(T i);

		// Floating point
		template<typename T>
		void BufferParseFloat(T i);

		// Str
		template<std::size_t SIZE>
		inline void BufferParseCharPt(const char(&str)[SIZE])						{ BufferParseCharPt(str, SIZE); }
		inline void BufferParseCharPt(const char* str)								{ while (*str != 0) BufferPushBack(*str++); }
		inline void BufferParseCharPt(const char* str, std::size_t size)			{ while (size-- != 0 && *str != 0) BufferPushBack(*str++); }

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
		inline void FormatForward(const std::size_t size)							{ m_SubFormat += size; if (!FormatCanMoveForward()) m_SubFormat = m_FormatEnd; }
		inline void FormatBackward(const std::size_t size)							{ m_SubFormat -= size; if (!FormatCanMoveBackward()) m_SubFormat = m_Format; }
		
		inline char FormatGet() const												{ return *m_SubFormat; }
		inline char FormatGetAndForward()											{ return FormatCanMoveForward() ? *m_SubFormat++ : '\0'; }
		inline char FormatGetAndForwardNoCheck()									{ return *m_SubFormat++; }
		inline char FormatGetAndBackward()											{ return FormatCanMoveBackward() ? *m_SubFormat-- : '\0'; }
		inline char FormatGetNext() const											{ return FormatCanMoveForward() ? *(m_SubFormat + 1) : '\0'; }

		// Format check
		inline bool FormatIsEqualTo(const char c) const								{ return FormatGet() == c; }
		inline bool FormatIsNotEqualTo(const char c) const							{ return FormatGet() != c; }
		inline bool FormatIsEqualForward(const char c)								{ if (FormatIsEqualTo(c)) { FormatForward(); return true; } return false; }
		inline bool FormatIsNotEqualForward(const char c)							{ if (FormatIsNotEqualTo(c)) { FormatForward(); return true; } return false; }
		inline bool FormatNextIsEqualTo(const char c) const							{ return FormatGetNext() == c; }
		inline bool FormatNextIsNotEqualTo(const char c) const						{ return FormatGetNext() != c; }
		inline bool FormatNextIsEqualForward(const char c)							{ if (FormatNextIsEqualTo(c)) { FormatForward(); return true; } return false; }
		inline bool FormatNextIsNotEqualForward(const char c)						{ if (FormatNextIsNotEqualTo(c)) { FormatForward(); return true; } return false; }

		bool FormatNextIsSame(const char* str) {
			const char* prevSubFormat = m_SubFormat; bool isSame = true;
			while (isSame && *str != 0 && FormatCanMoveForward())	isSame = FormatGetAndForwardNoCheck() == *str++;
			if (isSame && *str != 0)								isSame = false;
			if (!isSame)											m_SubFormat = prevSubFormat;
			return isSame;
		}

		inline char FormatIsLowerCase() const										{ return FormatGet() >= 'a' && FormatGet() <= 'z'; }
		inline char FormatIsUpperCase() const										{ return FormatGet() >= 'A' && FormatGet() <= 'Z'; }
		inline char FormatIsADigit() const											{ return FormatGet() >= '0' && FormatGet() <= '9'; }

		// Format commands
		inline void FormatIgnoreSpace()												{ while (FormatIsEqualTo(' ') && FormatCanMoveForward()) FormatForwardNoCheck(); }

		inline void FormatGoTo(const char c)										{ while (FormatIsNotEqualTo(c) && FormatCanMoveForward())														FormatForwardNoCheck(); }
		inline void FormatGoTo(const char c0, const char c1)						{ while (FormatIsNotEqualTo(c0) && FormatIsNotEqualTo(c1) && FormatCanMoveForward())							FormatForwardNoCheck(); }
		inline void FormatGoTo(const char c0, const char c1, const char c2)			{ while (FormatIsNotEqualTo(c0) && FormatIsNotEqualTo(c1) && FormatIsNotEqualTo(c2) && FormatCanMoveForward())	FormatForwardNoCheck(); }
		inline void FormatGoToNext(const char c)									{ while (FormatIsNotEqualTo(c) && FormatCanMoveForward())														FormatForwardNoCheck();	FormatForward(); }
		inline void FormatGoToNext(const char c0, const char c1)					{ while (FormatIsNotEqualTo(c0) && FormatIsNotEqualTo(c1) && FormatCanMoveForward())							FormatForwardNoCheck();	FormatForward(); }
		inline void FormatGoToNext(const char c0, const char c1, const char c2)		{ while (FormatIsNotEqualTo(c0) && FormatIsNotEqualTo(c1) && FormatIsNotEqualTo(c2) && FormatCanMoveForward())	FormatForwardNoCheck();	FormatForward(); }

		// Format commands in parameter (add check to '}' to avoid skip the end of the format specifier)
		inline void FormatParamGoTo(const char c)										{ while (FormatIsNotEqualTo('}') && FormatIsNotEqualTo(c) && FormatCanMoveForward())														FormatForwardNoCheck(); }
		inline void FormatParamGoTo(const char c0, const char c1)						{ while (FormatIsNotEqualTo('}') && FormatIsNotEqualTo(c0) && FormatIsNotEqualTo(c1) && FormatCanMoveForward())								FormatForwardNoCheck(); }
		inline void FormatParamGoTo(const char c0, const char c1, const char c2)		{ while (FormatIsNotEqualTo('}') && FormatIsNotEqualTo(c0) && FormatIsNotEqualTo(c1) && FormatIsNotEqualTo(c2) && FormatCanMoveForward())	FormatForwardNoCheck(); }
		inline void FormatParamGoToNext(const char c)									{ while (FormatIsNotEqualTo('}') && FormatIsNotEqualTo(c) && FormatCanMoveForward())														FormatForwardNoCheck();	FormatForward(); }
		inline void FormatParamGoToNext(const char c0, const char c1)					{ while (FormatIsNotEqualTo('}') && FormatIsNotEqualTo(c0) && FormatIsNotEqualTo(c1) && FormatCanMoveForward())								FormatForwardNoCheck();	FormatForward(); }
		inline void FormatParamGoToNext(const char c0, const char c1, const char c2)	{ while (FormatIsNotEqualTo('}') && FormatIsNotEqualTo(c0) && FormatIsNotEqualTo(c1) && FormatIsNotEqualTo(c2) && FormatCanMoveForward())	FormatForwardNoCheck();	FormatForward(); }


	public:
		// Buffer
		inline bool BufferCanMoveForward() const									{ return m_SubBuffer < m_BufferEnd; }
		inline bool BufferCanMoveBackward() const									{ return m_SubBuffer > m_Buffer; }
		inline bool BufferIsNotOutOfBound() const									{ return !BufferCanMoveForward() || !BufferCanMoveBackward(); }

		// Buffer base commands
		inline void BufferForward()													{ if (BufferCanMoveForward()) ++m_SubBuffer; }
		inline void BufferForwardNoCheck()											{ ++m_SubBuffer; }
		inline void BufferBackward()												{ if (BufferCanMoveBackward())m_SubBuffer; }
		inline void BufferForward(const std::size_t size)							{ m_SubBuffer += size; if (!BufferCanMoveForward()) m_SubBuffer = m_BufferEnd; }
		inline void BufferBackward(const std::size_t size)							{ m_SubBuffer -= size; if (!BufferCanMoveBackward()) m_SubBuffer = m_Buffer; }

		inline char BufferGet() const												{ return *m_SubBuffer; }
		inline char BufferGetAndForward()											{ return BufferCanMoveForward() ? *m_SubBuffer++ : '\0'; }
		inline char BufferGetAndForwardNoCheck()									{ return *m_SubBuffer++; }
		inline char BufferGetAndBackward()											{ return BufferCanMoveBackward() ? *m_SubBuffer-- : '\0'; }
		inline char BufferGetNext() const											{ return BufferCanMoveForward() ? *(m_SubBuffer + 1) : '\0'; }

		inline void BufferSet(const char c)											{ *m_SubBuffer = c; }
		inline void BufferPushBack(const char c)									{ if (BufferCanMoveForward()) *m_SubBuffer++ = c; }
		inline void BufferPushReverse(const char c)									{ if (BufferCanMoveBackward()) *m_SubBuffer-- = c; }
		inline void BufferPushBackNoCheck(const char c)								{ *m_SubBuffer++ = c; }
		inline void BufferPushReverseNoCheck(const char c)							{ *m_SubBuffer-- = c; }

		// Buffer commands
		inline void BufferPushEndChar()												{ BufferPushBack('\0'); }
		inline void BufferAddSpaces(const std::size_t count)						{ for (std::size_t i = count; i > 0 && BufferCanMoveForward(); --i) BufferPushBackNoCheck(' '); }

	public:
		// Format check for param
		inline bool FormatIsEndOfParameter()										{ return FormatIsEqualTo('}'); }
		inline void FormatGoToEndOfParameter()										{ while (FormatIsNotEqualTo('}') && FormatCanMoveForward()) FormatForwardNoCheck(); }
		inline void FormatGoOutOfParameter()										{ while (FormatIsNotEqualTo('}') && FormatCanMoveForward()) FormatForwardNoCheck(); FormatForward(); }

		inline void CopyFormatToBuffer()											{ BufferPushBack(FormatGetAndForward()); }

		inline void WriteUntilNextParameter() 												{ while (FormatIsNotEqualTo('{') && FormatCanMoveForward())																					CopyFormatToBuffer(); }
		inline void WriteUntilNextParameterOr(const char c)									{ while (FormatIsNotEqualTo(c) && FormatIsNotEqualTo('{') && FormatCanMoveForward())														CopyFormatToBuffer(); }
		inline void WriteUntilNextParameterOr(const char c0, const char c1)					{ while (FormatIsNotEqualTo(c0) && FormatIsNotEqualTo(c1) && FormatIsNotEqualTo('{') && FormatCanMoveForward())								CopyFormatToBuffer(); }
		inline void WriteUntilNextParameterOr(const char c0, const char c1, const char c2)	{ while (FormatIsNotEqualTo(c0) && FormatIsNotEqualTo(c1) && FormatIsNotEqualTo(c2) && FormatIsNotEqualTo('{') && FormatCanMoveForward())	CopyFormatToBuffer(); }

		inline void WriteUntilEndOfParameter()													{ while (FormatIsNotEqualTo('}') && FormatCanMoveForward())																					CopyFormatToBuffer(); }
		inline void WriteUntilEndOfParameterOr(const char c)									{ while (FormatIsNotEqualTo(c) && FormatIsNotEqualTo('}') && FormatCanMoveForward())														CopyFormatToBuffer(); }
		inline void WriteUntilEndOfParameterOr(const char c0, const char c1)					{ while (FormatIsNotEqualTo(c0) && FormatIsNotEqualTo(c1) && FormatIsNotEqualTo('}') && FormatCanMoveForward())								CopyFormatToBuffer(); }
		inline void WriteUntilEndOfParameterOr(const char c0, const char c1, const char c2)		{ while (FormatIsNotEqualTo(c0) && FormatIsNotEqualTo(c1) && FormatIsNotEqualTo(c2) && FormatIsNotEqualTo('}') && FormatCanMoveForward())	CopyFormatToBuffer(); }

		inline bool CheckForEscape() {
			if (FormatNextIsEqualForward('{')) {
				FormatForward();
				BufferPushBack('{');
				return true;
			}
			return false;
		}
	};
}