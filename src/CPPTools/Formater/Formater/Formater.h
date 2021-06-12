#pragma once

#include "../Core/Core.h"
#include "../BaseFormat/FormatType.h"
#include "../BaseFormat/NamedArgs.h"
#include "../BaseFormat/FormatArgs.h"

#include "../Core/FormaterHandler/FormaterHandler.h"

namespace CPPTools::Fmt {

	class Formater {

	public:
		Formater(const char* const format, size_t formatSize, char* const buffer, size_t bufferSize);

	private:
		char* const m_Buffer;
		char* m_SubBuffer;
		size_t m_BufferSize;
		const char* const m_Format;
		const char* m_SubFormat;
		size_t m_FormatSize;

		uint8_t m_ValuesIdx = 0;
		FormatData m_FormatData;
		AnsiColorMem m_ColorMem;

	public:
		inline char* const GetBuffer() const				{ return m_Buffer; }
		inline size_t GetSize() const						{ return m_SubBuffer - m_Buffer; }
		inline AnsiColorMem& GetColorMem()					{ return m_ColorMem; }
		inline FormatData& GetFormatData()					{ return m_FormatData; }
		inline const FormatData& GetFormatData() const		{ return m_FormatData; }

		inline const FormatData FowardFormatData() const	{ return m_FormatData; }

	public:
		inline static FormaterHandler& GetAPI()			{ return FormaterHandler::GetInstance(); }

	private:
		/////---------- Default Print Rec ----------/////
		void FormatPrintRec(uint8_t idx);
		template<typename T, typename ...Args>
		void FormatPrintRec(uint8_t idx, const T& t, Args&& ...args);


		/////---------- NamedArgs Print Rec ----------/////
		void FormatPrintRecNamedArgs(const char* const name);
		template<typename T, typename ...Args>
		void FormatPrintRecNamedArgs(const char* const name, const NamedArgs<T>& t, Args&& ...args);
		template<typename T, typename ...Args>
		inline void FormatPrintRecNamedArgs(const char* const name, const CharPtNamedArgs<T>& t, Args&& ...args) {
			FormatPrintRecNamedArgs(name, static_cast<const NamedArgs<T>&>(t), std::forward<Args>(args)...);
		}
		template<typename T, typename ...Args>
		inline void FormatPrintRecNamedArgs(const char* const name, const StringNamedArgs<T>& t, Args&& ...args) {
			FormatPrintRecNamedArgs(name, static_cast<const NamedArgs<T>&>(t), std::forward<Args>(args)...);
		}
		template<typename T, typename ...Args>
		void FormatPrintRecNamedArgs(const char* const name, const T& t, Args&& ...args);


		/////---------- Data Print Rec ----------/////
		void ParameterDataRec(uint8_t idx);
		template<typename T, typename ...Args>
		void ParameterDataRec(uint8_t idx, const T& t, Args&& ...args);


		/////---------- Impl ----------/////
		template<typename ...Args>
		void ParameterType(Args&& ...args);
		template<typename ...Args>
		void ParameterData(Args&& ...args);

	public:
		template<typename ...Args>
		void Format(Args&& ...args);
		template<size_t SIZE, typename ...Args>
		void LittleFormat(const char (&format)[SIZE], Args&& ...args);
		template<typename ...Args>
		void LittleFormat(const std::string& format, Args&& ...args);
		template<typename ...Args>
		void MainFormat(Args&& ...args);

	public:
		bool GetFormatType(const char* formatTypes[], FormatSpecifierIDX* arr, uint8_t size);
		uint8_t GetWordFromList(const char* formatTypes[], uint8_t count);

	private:
		void CheckEndStr();
		void ColorValuePrint();
		uint8_t GetColorCode();
		uint8_t GetColorFG();
		uint8_t GetColorBG();
		void TimerValuePrint();
		void DateValuePrint();
		void ReloadColor();

	public:
		template<typename T>
		bool FormatReadInt(T& i);
		template<typename T>
		bool FormatReadUInt(T& i);

	public:
		template<typename T>
		void BufferParseInt(T i);
		template<typename T>
		void BufferParseUInt(T i);

		template<typename T>
		void BufferParseFloat(T i);

		inline void BufferParseCharPt(const char* str, size_t size)					{ while (size-- != 0 && *str != 0) *m_SubBuffer++ = *str++; }
		template<size_t SIZE>
		inline void BufferParseCharPt(const char(&str)[SIZE])						{ BufferParseCharPt(str, SIZE); }
		inline void BufferParseCharPt(const char* str)								{ while (*str != 0) *m_SubBuffer++ = *str++; }

	public:
		inline bool IsEndOfParameter()												{ return *m_SubFormat == '}'; }
		inline void GoToEndOfParameter()											{ while (*m_SubFormat != '}' && *m_SubFormat != '\0') ++m_SubFormat; }
		inline void GoOutOfParameter()												{ while (*m_SubFormat != '}' && *m_SubFormat != '\0') ++m_SubFormat; ++m_SubFormat; }

		inline bool IsEndOfFormat()													{ return *m_SubFormat == '\0'; }
		inline void FormatIgnoreSpace()												{ while (*m_SubFormat == ' ') ++m_SubFormat; }
		inline void FormatGoToNext(const char c)									{ while (*m_SubFormat != c && *m_SubFormat != '}' && *m_SubFormat != '\0') ++m_SubFormat; }
		inline void FormatGoToNext(const char c0, const char c1)					{ while (*m_SubFormat != c0 && *m_SubFormat != c1 && *m_SubFormat != '}' && *m_SubFormat != '\0') ++m_SubFormat; }
		inline void FormatGoToNext(const char c0, const char c1, const char c2)		{ while (*m_SubFormat != c0 && *m_SubFormat != c1 && *m_SubFormat != c2 && *m_SubFormat != '}' && *m_SubFormat != '\0') ++m_SubFormat; }
		inline void FormatNext() { ++m_SubFormat; }
		inline bool FormatIsEqual(const char c) const								{ return *m_SubFormat == c; }
		inline bool FormatIsEqNext(const char c)									{ if (*m_SubFormat == c) { ++m_SubFormat; return true; } return false; }
		bool FormatNextIsSame(const char* str);

		inline char FormatGet() const												{ return *m_SubFormat; }
		inline char FormatGetAndNext()												{ return *m_SubFormat++; }

		inline char FormatIsLowerCase() const										{ return *m_SubFormat >= 'a' && *m_SubFormat <= 'z'; }
		inline char FormatIsUpperCase() const										{ return *m_SubFormat >= 'A' && *m_SubFormat <= 'Z'; }
		inline char FormatIsADigit() const											{ return *m_SubFormat >= '0' && *m_SubFormat <= '9'; }

	public:
		inline void BufferPushBack(const char c)									{ *m_SubBuffer++ = c; }
		inline void BufferPushEndChar()												{ BufferPushBack('\0'); }
		inline void BufferPushReverse(const char c)									{ *m_SubBuffer-- = c; }
		inline void BufferShift(const size_t size)									{ m_SubBuffer += size; }

		inline char BufferFormatGet() const											{ return *m_SubFormat; }
		inline char BufferFormatGetAndNext()										{ return *m_SubFormat++; }
		inline char BufferPop()														{ return *m_SubBuffer--; }

	public:
		inline void WriteUntilNextParameter()										{ while (*m_SubFormat != '{' && *m_SubFormat != 0) *m_SubBuffer++ = *m_SubFormat++; }
		inline void WriteUntilEndOfParameter()										{ while (*m_SubFormat != '}' && *m_SubFormat != 0) *m_SubBuffer++ = *m_SubFormat++; }
		inline void WriteUntil(const char c)										{ while (*m_SubFormat != c && *m_SubFormat != '}' && *m_SubFormat != '\0') *m_SubBuffer++ = *m_SubFormat++; }
		inline void WriteUntil(const char c0, const char c1)						{ while (*m_SubFormat != c0 && *m_SubFormat != c1 && *m_SubFormat != '}' && *m_SubFormat != '\0') *m_SubBuffer++ = *m_SubFormat++; }
		inline void WriteUntil(const char c0, const char c1, const char c2)			{ while (*m_SubFormat != c0 && *m_SubFormat != c1 && *m_SubFormat != c2 && *m_SubFormat != '}' && *m_SubFormat != '\0') *m_SubBuffer++ = *m_SubFormat++; }

		inline bool CheckForEscape() {
			bool i = *(m_SubBuffer - 1) == '#' && m_SubBuffer != m_Buffer;

			if (!i)
				return false;

			*(m_SubBuffer - 1) = '{';
			++m_SubFormat;

			return true;
		}
	};
}