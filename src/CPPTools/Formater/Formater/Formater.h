#pragma once

#include "../Core/Core.h"
#include "../BaseFormat/FormatType.h"
#include "../BaseFormat/NamedArgs.h"
#include "../BaseFormat/FormatArgs.h"

#include "../Core/FormaterHandler/FormaterHandler.h"

namespace CPPTools::Fmt {

	class Formater {

	public:
		Formater(const char* const format, std::size_t formatSize, char* const buffer, std::size_t bufferSize);

	private:
		char* const m_Buffer;
		char* m_SubBuffer;
		std::size_t m_BufferSize;
		const char* const m_Format;
		const char* m_SubFormat;
		std::size_t m_FormatSize;

		std::size_t m_NoStride = 0;

		std::uint8_t m_ValuesIdx = 0;
		FormatData m_FormatData;
		AnsiColorMem m_ColorMem;

	public:
		inline char* const GetBuffer()					{ return m_Buffer; }
		inline const char* const GetBuffer() const		{ return m_Buffer; }
		inline std::size_t GetBufferSize() const		{ return m_BufferSize; }
		inline const char* const GetFormat() const		{ return m_Format; }
		inline std::size_t GetFormatSize() const		{ return m_FormatSize; }

		inline std::size_t GetCurrentBufferSize() const		{ return m_SubBuffer - m_Buffer; }

		inline AnsiColorMem& GetColorMem()					{ return m_ColorMem; }
		inline FormatData& GetFormatData()					{ return m_FormatData; }
		inline const FormatData& GetFormatData() const		{ return m_FormatData; }
		inline const FormatData ForwardFormatData() const	{ return m_FormatData; }

		inline void AddNoStride(const std::size_t noStride) { m_NoStride += noStride; }
		inline std::size_t GetStride() const				{ return GetCurrentBufferSize() - m_NoStride; }

	public:
		inline static FormaterHandler& GetAPI()			{ return FormaterHandler::GetInstance(); }

	private:
		/////---------- Default Print Rec ----------/////
		void FormatPrintRec( std::uint8_t idx);
		template<typename T, typename ...Args>
		void FormatPrintRec( std::uint8_t idx, const T& t, Args&& ...args);


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
		void ParameterDataRec(std::uint8_t idx);
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
		template<typename T>
		void BufferParseInt(T i);
		template<typename T>
		void BufferParseUInt(T i);

		template<typename T>
		void BufferParseFloat(T i);

		inline void BufferParseCharPt(const char* str, std::size_t size)			{ while (size-- != 0 && *str != 0) *m_SubBuffer++ = *str++; }
		template<std::size_t SIZE>
		inline void BufferParseCharPt(const char(&str)[SIZE])						{ BufferParseCharPt(str, SIZE); }
		inline void BufferParseCharPt(const char* str)								{ while (*str != 0) *m_SubBuffer++ = *str++; }

	public:
		inline bool FormatIsEndOfParameter()										{ return *m_SubFormat == '}'; }
		inline void FormatGoToEndOfParameter()										{ while (*m_SubFormat != '}' && *m_SubFormat != '\0') ++m_SubFormat; }
		inline void FormatGoOutOfParameter()										{ while (*m_SubFormat != '}' && *m_SubFormat != '\0') ++m_SubFormat; ++m_SubFormat; }

		inline bool FormatIsEndChar()												{ return *m_SubFormat == '\0'; }
		
	public:
		inline void FormatNext()													{ ++m_SubFormat; }
		inline char FormatPop()														{ return *m_SubFormat--; }
		inline bool FormatIsEqual(const char c) const								{ return *m_SubFormat == c; }
		inline bool FormatIsEqNext(const char c)									{ if (*m_SubFormat == c) { ++m_SubFormat; return true; } return false; }
		bool FormatNextIsSame(const char* str);

		inline char FormatGet() const												{ return *m_SubFormat; }
		inline char FormatGetAndNext()												{ return *m_SubFormat++; }

		inline void FormatIgnoreSpace()												{ while (*m_SubFormat == ' ' && *m_SubFormat != '\0') ++m_SubFormat; }
		inline void FormatGoTo(const char c)										{ while (*m_SubFormat != c && *m_SubFormat != '\0') ++m_SubFormat; }
		inline void FormatGoTo(const char c0, const char c1)						{ while (*m_SubFormat != c0 && *m_SubFormat != c1 && *m_SubFormat != '\0') ++m_SubFormat; }
		inline void FormatGoTo(const char c0, const char c1, const char c2)			{ while (*m_SubFormat != c0 && *m_SubFormat != c1 && *m_SubFormat != c2 && *m_SubFormat != '\0') ++m_SubFormat; }
		inline void FormatGoToNext(const char c)									{ while (*m_SubFormat != c && *m_SubFormat != '\0') ++m_SubFormat; ++m_SubFormat; }
		inline void FormatGoToNext(const char c0, const char c1)					{ while (*m_SubFormat != c0 && *m_SubFormat != c1 && *m_SubFormat != '\0') ++m_SubFormat; ++m_SubFormat; }
		inline void FormatGoToNext(const char c0, const char c1, const char c2)		{ while (*m_SubFormat != c0 && *m_SubFormat != c1 && *m_SubFormat != c2 && *m_SubFormat != '\0') ++m_SubFormat; ++m_SubFormat; }

		inline char FormatIsLowerCase() const										{ return *m_SubFormat >= 'a' && *m_SubFormat <= 'z'; }
		inline char FormatIsUpperCase() const										{ return *m_SubFormat >= 'A' && *m_SubFormat <= 'Z'; }
		inline char FormatIsADigit() const											{ return *m_SubFormat >= '0' && *m_SubFormat <= '9'; }

	// Format in parameter (add check to '}' to avoid skip the end of the format specifier)
	public:
		inline void FormatParamIgnoreSpace()										{ while (*m_SubFormat == ' ' && *m_SubFormat != '}' && *m_SubFormat != '\0') ++m_SubFormat; }
		inline void FormatParamGoTo(const char c)									{ while (*m_SubFormat != c && *m_SubFormat != '}' && *m_SubFormat != '\0') ++m_SubFormat; }
		inline void FormatParamGoTo(const char c0, const char c1)					{ while (*m_SubFormat != c0 && *m_SubFormat != c1 && *m_SubFormat != '}' && *m_SubFormat != '\0') ++m_SubFormat; }
		inline void FormatParamGoTo(const char c0, const char c1, const char c2)	{ while (*m_SubFormat != c0 && *m_SubFormat != c1 && *m_SubFormat != c2 && *m_SubFormat != '}' && *m_SubFormat != '\0') ++m_SubFormat; }
		inline void FormatParamGoToNext(const char c)									{ while (*m_SubFormat != c && *m_SubFormat != '}' && *m_SubFormat != '\0') ++m_SubFormat; ++m_SubFormat; }
		inline void FormatParamGoToNext(const char c0, const char c1)					{ while (*m_SubFormat != c0 && *m_SubFormat != c1 && *m_SubFormat != '}' && *m_SubFormat != '\0') ++m_SubFormat; ++m_SubFormat; }
		inline void FormatParamGoToNext(const char c0, const char c1, const char c2)	{ while (*m_SubFormat != c0 && *m_SubFormat != c1 && *m_SubFormat != c2 && *m_SubFormat != '}' && *m_SubFormat != '\0') ++m_SubFormat; ++m_SubFormat; }

	public:
		inline void BufferPushBack(const char c)									{ *m_SubBuffer++ = c; }
		inline void BufferPushEndChar()												{ *m_SubBuffer++ = '\0'; }
		inline void BufferPushReverse(const char c)									{ *m_SubBuffer-- = c; }
		inline void BufferShiftAdd(const std::size_t size)							{ m_SubBuffer += size; }
		inline void BufferShiftRemove(const std::size_t size)						{ m_SubBuffer -= size; }
		inline void BufferAddSpaces(const std::size_t count)						{ for (std::size_t i = count; i > 0; --i) *m_SubBuffer++ = ' '; }

		inline char BufferFormatGet() const											{ return *m_SubFormat; }
		inline char BufferFormatGetAndNext()										{ return *m_SubFormat++; }
		inline char BufferPop()														{ return *m_SubBuffer--; }

	public:
		inline void WriteUntilNextParameter()										{ while (*m_SubFormat != '{' && *m_SubFormat != '\0') *m_SubBuffer++ = *m_SubFormat++; }
		inline void WriteUntilEndOfParameter()										{ while (*m_SubFormat != '}' && *m_SubFormat != '\0') *m_SubBuffer++ = *m_SubFormat++; }
		inline void WriteUntil(const char c)										{ while (*m_SubFormat != c && *m_SubFormat != '}' && *m_SubFormat != '\0') *m_SubBuffer++ = *m_SubFormat++; }
		inline void WriteUntil(const char c0, const char c1)						{ while (*m_SubFormat != c0 && *m_SubFormat != c1 && *m_SubFormat != '}' && *m_SubFormat != '\0') *m_SubBuffer++ = *m_SubFormat++; }
		inline void WriteUntil(const char c0, const char c1, const char c2)			{ while (*m_SubFormat != c0 && *m_SubFormat != c1 && *m_SubFormat != c2 && *m_SubFormat != '}' && *m_SubFormat != '\0') *m_SubBuffer++ = *m_SubFormat++; }

		inline bool CheckForEscape() {
			if (*(m_SubBuffer - 1) != '#' || m_SubBuffer == m_Buffer)	return false;
			*(m_SubBuffer - 1) = '{';
			++m_SubFormat;
			return true;
		}
	};
}