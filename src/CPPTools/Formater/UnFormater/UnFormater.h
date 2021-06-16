#pragma once

#include "../Core/Core.h"
#include "../BaseFormat/FormatType.h"
#include "../BaseFormat/NamedArgs.h"
#include "../BaseFormat/FormatArgs.h"

#include "../Core/FormaterHandler/FormaterHandler.h"

namespace CPPTools::Fmt {

	struct UnFormatError
	{
		bool error;
		std::int16_t FormatPosError;
		std::int16_t BufferPosError;
		UnFormatError()
			: error(false), FormatPosError(0), BufferPosError(0) { }
		UnFormatError(std::int16_t formatPosError, std::int16_t bufferPosError)
			: error(true), FormatPosError(formatPosError), BufferPosError(bufferPosError) { }
		inline operator bool() const { return error; }
	};

}

namespace CPPTools::Fmt {

	class UnFormater
	{
	public:
		UnFormater(const std::string_view format, const std::string_view buffer);

	private:
		const char* const m_Buffer;
		const char* m_SubBuffer;
		const char* const m_BufferEnd;
		std::size_t m_BufferSize;

		const char* m_Format;
		const char* m_SubFormat;
		const char* m_FormatEnd;
		std::size_t m_FormatSize;

		std::uint8_t m_ValuesIdx = 0;
		Detail::AnsiColorMem m_ColorMem;
		FormatData m_FormatData;

	public:
		inline const char* GetBuffer() const			{ return m_Buffer; }
		inline std::size_t GetBufferSize() const			{ return m_BufferSize; }
		inline std::size_t GetCurrentBufferDist() const		{ return m_SubBuffer - m_Buffer; }

		inline const char* GetFormat() const			{ return m_Format; }
		inline std::size_t GetFormatSize() const			{ return m_FormatSize; }
		inline std::size_t GetCurrentFormatDist() const		{ return m_SubFormat - m_Format; }

		inline Detail::AnsiColorMem& GetColorMem()			{ return m_ColorMem; }
		inline FormatData& GetFormatData()					{ return m_FormatData; }
		inline const FormatData& GetFormatData() const		{ return m_FormatData; }
		inline FormatData ForwardFormatData() const			{ return m_FormatData; }

		inline std::int16_t NoError()						{ return -1; }

	private:

		/////---------- Read Rec ----------/////
		void FormatReadRec(std::uint8_t idx);
		template<typename T, typename ...Args>
		void FormatReadRec(std::uint8_t idx, T& t, Args&& ...args);

		/////---------- NamedArgs ----------/////
		void FormatReadRecNamedArgs(const char* const name);
		template<typename T, typename ...Args>
		void FormatReadRecNamedArgs(const char* const name, NamedArgs<T>& t, Args&& ...args);
		template<typename T, typename ...Args>
		inline void FormatReadRecNamedArgs(const char* const name, CharPtNamedArgs<T>& t, Args&& ...args) {
			FormatReadRecNamedArgs(name, static_cast<NamedArgs<T>&>(t), std::forward<Args>(args)...);
		}
		template<typename T, typename ...Args>
		inline void FormatReadRecNamedArgs(const char* const name, StringNamedArgs<T>& t, Args&& ...args) {
			FormatReadRecNamedArgs(name, static_cast<NamedArgs<T>&>(t), std::forward<Args>(args)...);
		}
		template<typename T, typename ...Args>
		void FormatReadRecNamedArgs(const char* const name, T& t, Args&& ...args);

		/////---------- Data ----------/////
		void ParameterDataRec(std::uint8_t idx);
		template<typename T, typename ...Args>
		void ParameterDataRec(std::uint8_t idx, const T& t, Args&& ...args);

		/////---------- Impl ----------/////
		template<typename ...Args>
		void ParameterType(Args&& ...args);
		template<typename ...Args>
		void ParameterData(Args&& ...args);

	private:
		void GetColorValue();
		void GetTimerPrinted();
		void GetDatePrinted();
		void IgnoreParameter();

	public:
		template<typename ...Args>
		bool UnFormat(Args&& ...args);
		template<typename ...Args>
		bool LittleUnFormat(const std::string_view format, Args&& ...args);
		template<typename ...Args>
		UnFormatError MainUnFormat(Args&& ...args);

	public:
		template<typename T>
		bool FormatReadInt(T& i);
		template<typename T>
		bool FormatReadUInt(T& i);

	public:
		template<typename T>
		bool BufferUnParseInt(T& i);
		template<typename T>
		bool BufferUnParseUInt(T& i);

		template<typename T>
		bool BufferUnParseFloat(T& i);

		template<std::size_t SIZE>
		inline bool BufferUnParseCharPt(char(&str)[SIZE])								{ BufferUnParseCharPt(str, SIZE); }
		inline bool BufferUnParseCharPt(char* str, std::size_t size)					{ while (size-- != 0) *str++ = BufferGetAndForward(); }
		inline bool BufferUnParseCharPt(char* str)										{ return false; }

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
		inline bool BufferEnd() const												{ return m_SubBuffer >= m_BufferEnd; }

		// Buffer base commands
		inline void BufferForward()													{ if (BufferCanMoveForward()) ++m_SubBuffer; }
		inline void BufferForwardNoCheck()											{ ++m_SubBuffer; }
		inline void BufferBackward()												{ if (BufferCanMoveBackward()) --m_SubBuffer; }
		inline void BufferForward(const std::size_t size)							{ m_SubBuffer += size; if (!BufferCanMoveForward()) m_SubBuffer = m_BufferEnd; }
		inline void BufferBackward(const std::size_t size)							{ m_SubBuffer -= size; if (!BufferCanMoveBackward()) m_SubBuffer = m_Buffer; }
		
		inline char BufferGet() const												{ return *m_SubBuffer; }
		inline char BufferGetAndForward()											{ return BufferCanMoveForward() ? *m_SubBuffer++ : '\0'; }
		inline char BufferGetAndForwardNoCheck()									{ return *m_SubBuffer++; }
		inline char BufferGetAndBackward()											{ return BufferCanMoveBackward() ? *m_SubBuffer-- : '\0'; }
		inline char BufferGetNext() const											{ return BufferCanMoveForward() ? *(m_SubBuffer + 1) : '\0'; }

		// Buffer check
		inline bool BufferIsEqualTo(const char c) const								{ return BufferGet() == c; }
		inline bool BufferIsNotEqualTo(const char c) const							{ return BufferGet() != c; }
		inline bool BufferIsEqualForward(const char c)								{ if (BufferIsEqualTo(c)) { BufferForward(); return true; } return false; }
		inline bool BufferIsNotEqualForward(const char c)							{ if (BufferIsNotEqualTo(c)) { BufferForward(); return true; } return false; }
		inline bool BufferNextIsEqualTo(const char c) const							{ return BufferGetNext() == c; }
		inline bool BufferNextIsNotEqualTo(const char c) const						{ return BufferGetNext() != c; }
		inline bool BufferNextIsEqualForward(const char c)							{ if (BufferNextIsEqualTo(c)) { BufferForward(); return true; } return false; }
		inline bool BufferNextIsNotEqualForward(const char c)						{ if (BufferNextIsNotEqualTo(c)) { BufferForward(); return true; } return false; }

		bool BufferNextIsSame(const char* str) {
			const char* prevSubBuffer = m_SubBuffer; bool isSame = true;
			while (isSame && *str != 0 && BufferCanMoveForward())	isSame = BufferGetAndForwardNoCheck() == *str++;
			if (isSame && *str != 0)								isSame = false;
			if (!isSame)											m_SubBuffer = prevSubBuffer;
			return isSame;
		}

		inline char BufferIsLowerCase() const										{ return BufferGet() >= 'a' && BufferGet() <= 'z'; }
		inline char BufferIsUpperCase() const										{ return BufferGet() >= 'A' && BufferGet() <= 'Z'; }
		inline char BufferIsADigit() const											{ return BufferGet() >= '0' && BufferGet() <= '9'; }

		// Buffer commands
		inline void BufferIgnoreSpace()												{ while (BufferIsEqualTo(' ') && BufferCanMoveForward()) BufferForwardNoCheck(); }

		inline void BufferGoTo(const char c)										{ while (BufferIsNotEqualTo(c) && BufferCanMoveForward())														BufferForwardNoCheck(); }
		inline void BufferGoTo(const char c0, const char c1)						{ while (BufferIsNotEqualTo(c0) && BufferIsNotEqualTo(c1) && BufferCanMoveForward())							BufferForwardNoCheck(); }
		inline void BufferGoTo(const char c0, const char c1, const char c2)			{ while (BufferIsNotEqualTo(c0) && BufferIsNotEqualTo(c1) && BufferIsNotEqualTo(c2) && BufferCanMoveForward())	BufferForwardNoCheck(); }
		inline void BufferGoToNext(const char c)									{ while (BufferIsNotEqualTo(c) && BufferCanMoveForward())														BufferForwardNoCheck();	BufferForward(); }
		inline void BufferGoToNext(const char c0, const char c1)					{ while (BufferIsNotEqualTo(c0) && BufferIsNotEqualTo(c1) && BufferCanMoveForward())							BufferForwardNoCheck();	BufferForward(); }
		inline void BufferGoToNext(const char c0, const char c1, const char c2)		{ while (BufferIsNotEqualTo(c0) && BufferIsNotEqualTo(c1) && BufferIsNotEqualTo(c2) && BufferCanMoveForward())	BufferForwardNoCheck();	BufferForward(); }

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
		inline bool CheckUntilEndOfParameterOr(const char c) {
			while (FormatIsNotEqualTo('}') && FormatIsNotEqualTo(c) && FormatCanMoveForward()) {
				if (!Check()) return false;
				FormatForwardNoCheck(); BufferForward();
			}
			return true;
		}
		inline bool CheckUntilEndOfParameterOr(const char c0, const char c1) {
			while (FormatIsNotEqualTo('}') && FormatIsNotEqualTo(c0) && FormatIsNotEqualTo(c1) && FormatCanMoveForward()) {
				if (!Check()) return false;
				FormatForwardNoCheck(); BufferForward();
			}
			return true;
		}
		inline bool CheckUntilEndOfParameterOr(const char c0, const char c1, const char c2) {
			while (FormatIsNotEqualTo('}') && FormatIsNotEqualTo(c0) && FormatIsNotEqualTo(c1) && FormatIsNotEqualTo(c2) && FormatCanMoveForward()) {
				if (!Check()) return false;
				FormatForwardNoCheck(); BufferForward();
			}
			return true;
		}

		inline bool CheckForEscape() {
			if(FormatNextIsEqualForward('{')) {
				FormatForward();
				BufferIsEqualForward('{');
				return true;
			}
			return false;
		}
	};
}