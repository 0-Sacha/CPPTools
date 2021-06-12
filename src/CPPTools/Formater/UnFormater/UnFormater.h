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
		int16_t FormatPosError;
		int16_t BufferPosError;
		UnFormatError()
			: error(false), FormatPosError(0), BufferPosError(0) { }
		UnFormatError(int16_t formatPosError, int32_t bufferPosError)
			: error(true), FormatPosError(formatPosError), BufferPosError(bufferPosError) { }
		inline operator bool() const { return error; }
	};

}

namespace CPPTools::Fmt {

	class UnFormater
	{
	public:
		UnFormater(const char* const format, size_t formatSize, const char* const buffer, size_t bufferSize);

	private:
		const char* const m_Buffer;
		const char* m_SubBuffer;
		size_t m_BufferSize;
		const char* const m_Format;
		const char* m_SubFormat;
		size_t m_FormatSize;

		uint8_t m_ValuesIdx = 0;
		AnsiColorMem m_ColorMem;
		FormatData m_FormatData;

	public:
		inline const char* const GetBuffer() const			{ return m_Buffer; }
		inline size_t GetBufferSize() const					{ return m_SubBuffer - m_Buffer; } // change name
		inline size_t GetFormatSize() const					{ return m_SubFormat - m_Format; } // change name
		inline AnsiColorMem& GetColorMem()					{ return m_ColorMem; }
		inline FormatData& GetFormatData()					{ return m_FormatData; }
		inline const FormatData& GetFormatData() const		{ return m_FormatData; }
		inline int16_t NoError()							{ return -1; }

		inline const FormatData ForwardFormatData() const	{ return m_FormatData; }

	private:
		template<typename ...Args>
		void ParameterType(Args&& ...args);
		template<typename ...Args>
		void ParameterData(Args&& ...args);

		/////---------- NamedArgs ----------/////
		void FormatReadRecNamedArgs(const char* const name);
		template<typename T, typename ...Args>
		void FormatReadRecNamedArgs(const char* const name, NamedArgs<T>& t, Args&& ...args);
		template<typename T, typename ...Args>
		inline void FormatReadRecNamedArgs(const char* const name, const CharPtNamedArgs<T>& t, Args&& ...args) {
			FormatReadRecNamedArgs(name, static_cast<const NamedArgs<T>&>(t), std::forward<Args>(args)...);
		}
		template<typename T, typename ...Args>
		inline void FormatReadRecNamedArgs(const char* const name, const StringNamedArgs<T>& t, Args&& ...args) {
			FormatReadRecNamedArgs(name, static_cast<const NamedArgs<T>&>(t), std::forward<Args>(args)...);
		}
		template<typename T, typename ...Args>
		void FormatReadRecNamedArgs(const char* const name, T& t, Args&& ...args);

		/////---------- Data ----------/////
		void ParameterDataRec(uint8_t idx);
		template<typename T, typename ...Args>
		void ParameterDataRec(uint8_t idx, const T& t, Args&& ...args);

		/////---------- Read Rec ----------/////
		void FormatReadRec(uint8_t idx);
		template<typename T, typename ...Args>
		void FormatReadRec(uint8_t idx, T& t, Args&& ...args);

	private:
		void GetColorValue();
		void GetTimerPrinted();
		void GetDatePrinted();
		void IgnoreParameter();

	public:
		template<typename ...Args>
		bool UnFormat(Args&& ...args);
		template<size_t SIZE, typename ...Args>
		bool LittleUnFormat(const char (&format)[SIZE], Args&& ...args);
		template<typename ...Args>
		bool LittleUnFormat(const std::string& fromat, Args&& ...args);
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

		inline bool BufferUnParseCharPt(char* str)										{ return false; }
		inline bool BufferUnParseCharPt(char* str, size_t size)							{ while (size-- != 0) *str++ = *m_SubBuffer++; }
		template<size_t SIZE>
		inline bool BufferUnParseCharPt(char(&str)[SIZE])								{ BufferUnParseCharPt(str, SIZE); }

	public:
		inline bool IsEndOfParameter()													{ return *m_SubFormat == '}'; }
		inline void GoToEndOfParameter()												{ while (*m_SubFormat != '}' && *m_SubFormat != '\0') ++m_SubFormat; }
		inline void GoOutOfParameter()													{ while (*m_SubFormat != '}' && *m_SubFormat != '\0') ++m_SubFormat; ++m_SubFormat; }

	public:
		inline bool IsEndOfFormat()														{ return *m_SubFormat == '\0'; }
		inline void FormatIgnoreSpace()													{ while (*m_SubFormat == ' ') ++m_SubFormat; }
		inline void FormatGoToNext(const char c)										{ while (*m_SubFormat != c && *m_SubFormat != '}' && *m_SubFormat != 0) ++m_SubFormat; }
		inline void FormatGoToNext(const char c0, const char c1)						{ while (*m_SubFormat != c0 && *m_SubFormat != c1 && *m_SubFormat != '}' && *m_SubFormat != 0) ++m_SubFormat; }
		inline void FormatGoToNext(const char c0, const char c1, const char c2)			{ while (*m_SubFormat != c0 && *m_SubFormat != c1 && *m_SubFormat != c2 && *m_SubFormat != '}' && *m_SubFormat != 0) ++m_SubFormat; }
		inline void FormatNext()														{ if(*m_SubFormat != 0) ++m_SubFormat; }
		inline bool FormatIsEqual(const char c)											{ return *m_SubFormat == c; }
		inline bool FormatIsEqNext(const char c)										{ if (*m_SubFormat == c) { ++m_SubFormat; return true; } return false; }

		bool FormatNextIsSame(const char* str);
		inline char FormatIsLowerCase() const											{ return *m_SubFormat >= 'a' && *m_SubFormat <= 'z'; }
		inline char FormatIsUpperCase() const											{ return *m_SubFormat >= 'A' && *m_SubFormat <= 'Z'; }
		inline char FormatIsADigit() const												{ return *m_SubFormat >= '0' && *m_SubFormat <= '9'; }

		inline char FormatGet() const													{ return *m_SubFormat; }
		inline char FormatGetAndNext()													{ if (*m_SubFormat != 0) return *m_SubFormat++; return 0; }
		inline char FormatPop()															{ return *m_SubFormat--; }
		inline void FormatShift(const size_t size)										{ m_SubFormat += size; }

	public:
		inline bool IsEndOfBuffer()														{ return *m_SubBuffer == '\0'; }
		inline void BufferIgnoreSpace()													{ while (*m_SubBuffer == ' ') ++m_SubBuffer; }
		inline void BufferGoToNext(const char c)										{ while (*m_SubBuffer != c && *m_SubBuffer != '}' && *m_SubBuffer != '\0') ++m_SubBuffer; }
		inline void BufferGoToNext(const char c0, const char c1)						{ while (*m_SubBuffer != c0 && *m_SubBuffer != c1 && *m_SubBuffer != '}' && *m_SubBuffer != '\0') ++m_SubBuffer; }
		inline void BufferGoToNext(const char c0, const char c1, const char c2)			{ while (*m_SubBuffer != c0 && *m_SubBuffer != c1 && *m_SubBuffer != c2 && *m_SubBuffer != '}' && *m_SubBuffer != '\0') ++m_SubBuffer; }
		inline void BufferNext()														{ if (*m_SubBuffer != 0) ++m_SubBuffer; }
		inline bool BufferIsEqual(const char c)											{ return *m_SubBuffer == c; }
		inline bool BufferIsEqNext(const char c)										{ if (*m_SubBuffer == c) { ++m_SubBuffer; return true; } return false; }

		bool BufferNextIsSame(const char* str);
		inline char BufferIsLowerCase() const											{ return *m_SubBuffer >= 'a' && *m_SubBuffer <= 'z'; }
		inline char BufferIsUpperCase() const											{ return *m_SubBuffer >= 'A' && *m_SubBuffer <= 'Z'; }
		inline char BufferIsADigit() const												{ return *m_SubBuffer >= '0' && *m_SubBuffer <= '9'; }

		inline char BufferGet() const													{ return *m_SubBuffer; }
		inline char BufferGetAndNext()													{ if (*m_SubBuffer != 0) return *m_SubBuffer++; return 0; }
		inline char BufferPop()															{ return *m_SubBuffer--; }
		inline void BufferShift(const size_t size)										{ m_SubBuffer += size; }

	public:
		inline bool Check()																{ return *m_SubBuffer == *m_SubFormat; }
		inline bool CheckAndNext()														{ if (*m_SubBuffer == *m_SubFormat) { ++m_SubBuffer; ++m_SubFormat; return true; } else return false; }

		inline bool CheckUntilNextParameter() {
			while (*m_SubFormat != '{' && *m_SubFormat != 0) {
				if (*m_SubBuffer != *m_SubFormat)
					return false;
				m_SubBuffer++;
				m_SubFormat++;
			}
			return true;
		}
		inline bool CheckUntilEndOfParameter() {
			while (*m_SubFormat != '}' && *m_SubFormat != 0) {
				if (*m_SubBuffer != *m_SubFormat)
					return false;
				m_SubBuffer++;
				m_SubFormat++;
			}
			return true;
		}
		inline bool CheckUntil(const char c) {
			while (*m_SubFormat != c && *m_SubFormat != '}' && *m_SubFormat != '\0') {
				if (*m_SubBuffer != *m_SubFormat)
					return false;
				m_SubBuffer++;
				m_SubFormat++;
			}
			return true;
		}
		inline bool CheckUntil(const char c0, const char c1) {
			while (*m_SubFormat != c0 && *m_SubFormat != c1 && *m_SubFormat != '}' && *m_SubFormat != '\0') {
				if (*m_SubBuffer != *m_SubFormat)
					return false;
				m_SubBuffer++;
				m_SubFormat++;
			}
			return true;
		}
		inline bool CheckUntil(const char c0, const char c1, const char c2) {
			while (*m_SubFormat != c0 && *m_SubFormat != c1 && *m_SubFormat != c2 && *m_SubFormat != '}' && *m_SubFormat != '\0') {
				if (*m_SubBuffer != *m_SubFormat)
					return false;
				m_SubBuffer++;
				m_SubFormat++;
			}
			return true;
		}

		inline bool CheckForEscape() {
			if (*m_SubFormat != '#')
				return false;

			if (*(m_SubFormat + 1) == '{' && *m_SubBuffer == '{') {
				m_SubFormat += 2;
				m_SubBuffer++;
				return true;
			}
			return false;
		}
	};
}