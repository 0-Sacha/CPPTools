#pragma once

#include "BasicFormatContext.h"
#include "BasicFormatContextClassImpl.h"

#include "Handlers/FormatReader.h"
#include "Handlers/FormatWriter.h"


namespace CPPTools::Fmt {

	/////---------- Default Print Rec ----------/////
	template<typename CharFormat, typename CharBuffer>
	template<typename T, typename ...Args>
	inline void BasicFormatContext<CharFormat, CharBuffer>::FormatTypeFromIdx(FormatIdx idx, const T& t, Args&& ...args) {
		if (idx == 0)	FormatType<GetBaseType<T>, BasicFormatContext<CharFormat, CharBuffer>>::Write(t, *this);
		else			FormatTypeFromIdx(idx - 1, std::forward<Args>(args)...);
	}

	/////---------- Data Print Rec ----------/////
	static inline void GetParameterData(FormatData& data, FormatIdx idx) {}
	template<typename T, typename ...Args>
	static inline void GetParameterData(FormatData& data, FormatIdx idx, const T& t, Args&& ...args) {
		if (idx == 0)	Detail::CopyFormatData<T>::Copy(data, t);
		else			GetParameterData(data, idx - 1, std::forward<Args>(args)...);
	}

	/////---------- NamedArgs Print Rec ----------/////
	template<typename CharFormat, typename CharBuffer>
	template<typename T, typename ...Args, typename CharName>
	inline void BasicFormatContext<CharFormat, CharBuffer>::GetNamedArgsIdx(FormatIdx& idx, FormatIdx currentIdx, const FCStringViewNamedArgs<T, CharName, CharFormat>& t, Args&& ...args) {
		if (FormatNextIsANamedArgs(t.GetName()))	idx = currentIdx;
		else										GetNamedArgsIdx(idx, currentIdx + 1, std::forward<Args>(args)...);
	}
	template<typename CharFormat, typename CharBuffer>
	template<typename T, typename ...Args, typename CharName>
	inline void BasicFormatContext<CharFormat, CharBuffer>::GetNamedArgsIdx(FormatIdx& idx, FormatIdx currentIdx, const FCStringNamedArgs<T, CharName, CharFormat>& t, Args&& ...args) {
		if (FormatNextIsANamedArgs(t.GetName()))	idx = currentIdx;
		else										GetNamedArgsIdx(idx, currentIdx + 1, std::forward<Args>(args)...);
	}
	template<typename CharFormat, typename CharBuffer>
	template<typename T, typename ...Args>
	inline void BasicFormatContext<CharFormat, CharBuffer>::GetNamedArgsIdx(FormatIdx& idx, FormatIdx currentIdx, const T& t, Args&& ...args) {
		GetNamedArgsIdx(idx, currentIdx + 1, std::forward<Args>(args)...);
	}


	/////---------- Impl ----------/////
	template<typename CharFormat, typename CharBuffer>
	template<typename ...Args>
	void BasicFormatContext<CharFormat, CharBuffer>::ParameterData(Args&& ...args) {
		if (FormatIsEqualTo(':')) {
			m_FormatData.HasSpec = true;
			while (!FormatIsEndOfParameter()) {
				FormatForward();
				FormatIgnoreSpace();

				if (FormatIsEqualForward('{')) {		// Forward specifier
					FormatIdx dataIdx;
					GetFormatIdx(dataIdx, std::forward<Args>(args)...);
					GetParameterData(m_FormatData, dataIdx, std::forward<Args>(args)...);
					FormatForward();
				} else if (FormatIsEqualForward('b')) { m_FormatData.IntPrint = Detail::ValueIntPrint::Bin;	FormatReadUInt(m_FormatData.Precision);
				} else if (FormatIsEqualForward('x')) { m_FormatData.IntPrint = Detail::ValueIntPrint::Hex;	FormatReadUInt(m_FormatData.Precision);
				} else if (FormatIsEqualForward('o')) { m_FormatData.IntPrint = Detail::ValueIntPrint::Oct;	FormatReadUInt(m_FormatData.Precision);
				} else if (FormatIsEqualForward('d')) { m_FormatData.IntPrint = Detail::ValueIntPrint::Int;	FormatReadUInt(m_FormatData.Precision);
				} else if (FormatIsLowerCase()) {	// Custom specifier
					const char c = FormatGetAndForward();
					std::int8_t i = 0;
					FormatReadInt(i);
					m_FormatData.AddSpecifier(c, i);
				} else if (FormatIsEqualForward('C'))		{ m_FormatData.HasChangeColor = true; m_ColorMem = Detail::AnsiColorMem(); ColorValuePrint();
				} else if (FormatIsEqualForward('>'))		{ m_FormatData.ShiftType = Detail::ShiftType::Right;	FormatReadUInt(m_FormatData.ShiftValue);
				} else if (FormatIsEqualForward('<'))		{ m_FormatData.ShiftType = Detail::ShiftType::Left;		FormatReadUInt(m_FormatData.ShiftValue);
				} else if (FormatIsEqualForward('^'))		{ m_FormatData.ShiftType = Detail::ShiftType::Center;	FormatReadUInt(m_FormatData.ShiftValue);
				} else if (FormatIsEqualForward('.'))		{ 
					if (!FormatIsEqualTo('{')) 
						FormatReadUInt(m_FormatData.FloatPrecision);
					else
						FormatReadParameter(m_FormatData.FloatPrecision, std::forward<Args>(args)...);
				} else if (FormatIsEqualForward('S'))		{ FormatReadUInt(m_FormatData.Size);
				} else if (FormatIsEqualForward('B'))		{ FormatReadUInt(m_FormatData.Begin);
				} else if (FormatIsEqualForward('\n'))		{ m_FormatData.ContainerPrintStyle = Detail::ContainerPrintStyle::NewLine;
				} else if (FormatIsEqualForward('='))		{ m_FormatData.BaseValue = true; }

				FormatParamGoTo(',');
			}
		}
	}

	template<typename CharFormat, typename CharBuffer>
	template<typename ...Args>
	bool BasicFormatContext<CharFormat, CharBuffer>::GetFormatIdx(FormatIdx& idx, Args&& ...args) {
		const CharFormat* mainSubFormat = m_SubFormat;

		// I : if there is no number specified : ':' or '}'
		if (FormatIsEqualTo(':') || FormatIsEqualTo('}')) {
			idx = m_ValuesIdx++;
			return true;
		}

		// II: A number(idx)
		if (FormatReadUInt(idx))
			if (FormatIsEqualTo(':') || FormatIsEqualTo('}'))
				return true;
		m_SubFormat = mainSubFormat;
			
		// III : A name
		GetNamedArgsIdx(idx, 0, std::forward<Args>(args)...);
		if (idx != FormatIdxNotFound)
			return true;
		m_SubFormat = mainSubFormat;

		// VI : { which is a idx to a number
		if (FormatIsEqualForward('{'))
			if (GetFormatIdx(idx, std::forward<Args>(args)...))
				return true;
		m_SubFormat = mainSubFormat;

		return false;
	}

	template<typename CharFormat, typename CharBuffer>
	template<typename ...Args>
	bool BasicFormatContext<CharFormat, CharBuffer>::ParameterPrint(Args&& ...args) {
		FormatForward();				// Skip {

		if (FormatIsEqualForward('C'))			ColorValuePrint();
		else if (FormatIsEqualForward('T'))		TimerValuePrint();
		else if (FormatIsEqualForward('D'))		DateValuePrint();
		else {
			FormatIdx formatIdx;
			if (!GetFormatIdx(formatIdx, std::forward<Args>(args)...))	return false;
			else {
				FormatData data;
				data.Clone(m_FormatData);
				m_FormatData = FormatData();

				Detail::AnsiColorMem colorMem(m_ColorMem);

				if (!m_FormatData.IsInit)			ParameterData(std::forward<Args>(args)...);

				FormatTypeFromIdx(formatIdx, std::forward<Args>(args)...);

				if (m_FormatData.HasChangeColor) { m_ColorMem = colorMem; ReloadColor(); }
				m_FormatData.Clone(data);
			}
		}

		FormatGoOutOfParameter();		// Skip}
		return true;
	}

	template<typename CharFormat, typename CharBuffer>
	template<typename ...Args>
	void BasicFormatContext<CharFormat, CharBuffer>::Format(Args&& ...args) {
		while (!FormatEnd()) {

			WriteUntilNextParameter();

			if (FormatIsEqualTo('{'))
				if (!ParameterPrint(std::forward<Args>(args)...))		BufferPushBack('{');
		}
	}

	template<typename CharFormat, typename CharBuffer>
	template<typename NewCharFormat, typename ...Args>
	void BasicFormatContext<CharFormat, CharBuffer>::LittleFormat(const std::basic_string_view<NewCharFormat> format, Args&& ...args) {
		BasicFormatContext<NewCharFormat, CharBuffer> newContext(format, *this);
		newContext.Format(std::forward<Args>(args)...);
		newContext.UpdateOldContext(*this);
	}

	template<typename CharFormat, typename CharBuffer>
	template<typename ...Args>
	void BasicFormatContext<CharFormat, CharBuffer>::LittleFormat(const std::basic_string_view<CharFormat> format, Args&& ...args) {
		// Copy
		const CharFormat* const mainFormat		= m_Format;
		const CharFormat* const mainSubFormat	= m_SubFormat;
		const CharFormat* const mainFormatEnd	= m_FormatEnd;
		const std::size_t mainFormatSize		= m_FormatSize;
		std::uint8_t mainIdx					= m_ValuesIdx;

		// Assign new value
		m_Format = format.data();
		m_SubFormat = format.data();
		m_FormatEnd = format.data() + format.size();
		m_FormatSize = format.size();
		m_ValuesIdx = 0;

		Format(std::forward<Args>(args)...);

		// Assign old value
		m_Format		= mainFormat;
		m_SubFormat		= mainSubFormat;
		m_FormatEnd		= mainFormatEnd;
		m_FormatSize	= mainFormatSize;
		m_ValuesIdx		= mainIdx;
	}

	template<typename CharFormat, typename CharBuffer>
	template<typename ...Args>
	void BasicFormatContext<CharFormat, CharBuffer>::MainFormat(Args&& ...args) {
		Format(std::forward<Args>(args)...);
		CheckEndStr();
	}


	/////---------- Impl with as Format ----------//////

	template<typename CharFormat = char, typename CharBuffer = CharFormat, std::size_t BUFFER_SIZE, typename ...Args>
	void FormatInChar(CharBuffer(&buffer)[BUFFER_SIZE], const std::basic_string_view<CharFormat> format, Args&& ...args) {
		BasicFormatContext<CharFormat, CharBuffer> formater(format, buffer, BUFFER_SIZE);
		formater.MainFormat(std::forward<Args>(args)...);
		formater.BufferPushEndChar();
	}
	template<typename CharFormat = char, typename CharBuffer = CharFormat, std::size_t BUFFER_SIZE, std::size_t FORMAT_SIZE, typename ...Args>
	inline void FormatInChar(CharBuffer(&buffer)[BUFFER_SIZE], const CharFormat(&format)[FORMAT_SIZE], Args&& ...args) {
		FormatInChar<CharFormat, CharBuffer>(buffer, std::basic_string_view<CharFormat>(format), std::forward<Args>(args)...);
	}

	template<typename CharFormat = char, typename CharBuffer = CharFormat, typename ...Args>
	void FormatInChar(CharBuffer* const buffer, const std::size_t bufferSize, const std::basic_string_view<CharFormat> format, Args&& ...args) {
		BasicFormatContext<CharFormat, CharBuffer> formater(format, buffer, bufferSize);
		formater.MainFormat(std::forward<Args>(args)...);
		formater.BufferPushEndChar();
	}
	template<typename CharFormat = char, typename CharBuffer = CharFormat, std::size_t FORMAT_SIZE, typename ...Args>
	void FormatInChar(CharBuffer* const buffer, const std::size_t bufferSize, const CharFormat(&format)[FORMAT_SIZE], Args&& ...args) {
		FormatInChar<CharFormat, CharBuffer>(buffer, bufferSize, std::basic_string_view<CharFormat>(format), std::forward<Args>(args)...);
	}

	template<std::size_t BUFFER_SIZE = 1024, typename CharFormat = char, typename CharBuffer = CharFormat, typename ...Args>
	void CFilePrint(FILE* stream, const std::basic_string_view<CharFormat> format, Args&& ...args) {
		CharBuffer buffer[BUFFER_SIZE];
		BasicFormatContext<CharFormat, CharBuffer> formater(format, buffer, BUFFER_SIZE);
		formater.MainFormat(std::forward<Args>(args)...);

		std::fwrite(formater.GetBuffer(), formater.GetSize(), 1, stream);
		std::fflush(stream);
	}
	template<std::size_t BUFFER_SIZE = 1024, typename CharFormat = char, typename CharBuffer = CharFormat, std::size_t FORMAT_SIZE, typename ...Args>
	void CFilePrint(FILE* stream, const CharFormat(&format)[FORMAT_SIZE], Args&& ...args) {
		CFilePrint<BUFFER_SIZE, CharFormat, CharBuffer>(stream, std::basic_string_view<CharFormat>(format), std::forward<Args>(args)...);
	}

	template<std::size_t BUFFER_SIZE = 1024, typename CharFormat = char, typename CharBuffer = CharFormat, typename ...Args>
	void CFilePrintLn(FILE* stream, const std::basic_string_view<CharFormat> format, Args&& ...args) {
		CharBuffer buffer[BUFFER_SIZE];
		BasicFormatContext<CharFormat, CharBuffer> formater(format, buffer, BUFFER_SIZE);
		formater.MainFormat(std::forward<Args>(args)...);
		formater.BufferPushBack('\n');

		std::fwrite(formater.GetBuffer(), formater.GetSize(), 1, stream);
		std::fflush(stream);
	}
	template<std::size_t BUFFER_SIZE = 1024, typename CharFormat = char, typename CharBuffer = CharFormat, std::size_t FORMAT_SIZE, typename ...Args>
	void CFilePrintLn(FILE* stream, const CharFormat(&format)[FORMAT_SIZE], Args&& ...args) {
		CFilePrintLn<BUFFER_SIZE, CharFormat, CharBuffer>(stream, std::basic_string_view<CharFormat>(format), std::forward<Args>(args)...);
	}

	template<std::size_t BUFFER_SIZE = 1024, typename CharFormat = char, typename CharBuffer = CharFormat, typename ...Args>
	void FilePrint(std::basic_ostream<CharBuffer>& stream, const std::basic_string_view<CharFormat> format, Args&& ...args) {
		CharBuffer buffer[BUFFER_SIZE];
		BasicFormatContext<CharFormat, CharBuffer> formater(format, buffer, BUFFER_SIZE);
		formater.MainFormat(std::forward<Args>(args)...);

		stream.write(formater.GetBuffer(), formater.GetCurrentBufferSize());
		stream.flush();
	}
	template<std::size_t BUFFER_SIZE = 1024, typename CharFormat = char, typename CharBuffer = CharFormat, std::size_t FORMAT_SIZE, typename ...Args>
	inline void FilePrint(std::basic_ostream<CharBuffer>& stream, const CharFormat(&format)[FORMAT_SIZE], Args&& ...args) {
		FilePrint<BUFFER_SIZE, CharFormat, CharBuffer>(stream, std::basic_string_view<CharFormat>(format), std::forward<Args>(args)...);
	}

	template<std::size_t BUFFER_SIZE = 1024, typename CharFormat = char, typename CharBuffer = CharFormat, typename ...Args>
	void FilePrintLn(std::basic_ostream<CharBuffer>& stream, const std::basic_string_view<CharFormat> format, Args&& ...args) {
		CharBuffer buffer[BUFFER_SIZE];
		BasicFormatContext<CharFormat, CharBuffer> formater(format, buffer, BUFFER_SIZE);
		formater.MainFormat(std::forward<Args>(args)...);
		formater.BufferPushBack('\n');
		
		stream.write(formater.GetBuffer(), formater.GetCurrentBufferSize());
		stream.flush();
	}
	template<std::size_t BUFFER_SIZE = 1024, typename CharFormat = char, typename CharBuffer = CharFormat, std::size_t FORMAT_SIZE, typename ...Args>
	inline void FilePrintLn(std::basic_ostream<CharBuffer>& stream, const CharFormat(&format)[FORMAT_SIZE], Args&& ...args) {
		FilePrintLn<BUFFER_SIZE, CharFormat, CharBuffer>(stream, std::basic_string_view<CharFormat>(format), std::forward<Args>(args)...);
	}

	template<std::size_t BUFFER_SIZE = 1024, typename CharFormat = char, typename CharBuffer = CharFormat, typename ...Args>
	void FormatInString(std::basic_string<CharBuffer>& str, const std::basic_string_view<CharFormat> format, Args&& ...args) {
		CharBuffer buffer[BUFFER_SIZE];
		BasicFormatContext<CharFormat, CharBuffer> formater(format, buffer, BUFFER_SIZE);
		formater.MainFormat(std::forward<Args>(args)...);
		formater.BufferPushEndChar();
		str = formater.GetBuffer();
	}
	template<std::size_t BUFFER_SIZE = 1024, typename CharFormat = char, typename CharBuffer = CharFormat, std::size_t FORMAT_SIZE, typename ...Args>
	inline void FormatInString(std::basic_string<CharBuffer>& str, const CharFormat (&format)[FORMAT_SIZE], Args&& ...args) {
		FormatInString<BUFFER_SIZE, CharFormat, CharBuffer>(str, std::basic_string_view<CharFormat>(format), std::forward<Args>(args)...);
	}

	template<std::size_t BUFFER_SIZE = 1024, typename CharFormat = char, typename CharBuffer = CharFormat, typename ...Args>
	inline std::basic_string<CharBuffer> FormatString(const std::basic_string_view<CharFormat> format, Args&& ...args) {
		CharBuffer buffer[BUFFER_SIZE];
		BasicFormatContext<CharFormat, CharBuffer> formater(format, buffer, BUFFER_SIZE);
		formater.MainFormat(std::forward<Args>(args)...);
		formater.BufferPushEndChar();
		return formater.GetBuffer();
	}
	template<std::size_t BUFFER_SIZE = 1024, typename CharFormat = char, typename CharBuffer = CharFormat, std::size_t FORMAT_SIZE, typename ...Args>
	inline std::basic_string<CharBuffer> FormatString(const CharFormat(&format)[FORMAT_SIZE], Args&& ...args) {
		return FormatString<BUFFER_SIZE, CharFormat, CharBuffer>(std::basic_string_view<CharFormat>(format), std::forward<Args>(args)...);
	}

	/////---------- NO-FORMAT Impl ----------//////

	template<typename CharBuffer = char, size_t BUFFER_SIZE, typename T>
	void FormatInChar(CharBuffer(&buffer)[BUFFER_SIZE], T&& t) {
		BasicFormatContext<char, CharBuffer> formater(std::basic_string_view<char>(), buffer, BUFFER_SIZE);
		FormatType<GetBaseType<T>, BasicFormatContext<char, CharBuffer>>::Write(t, formater);
		formater.BufferPushEndChar();
	}

	template<typename CharBuffer = char, typename T>
	void FormatInChar(CharBuffer* const buffer, const std::size_t bufferSize, T&& t) {
		BasicFormatContext<char, CharBuffer> formater(std::basic_string_view<char>(), buffer, bufferSize);
		FormatType<GetBaseType<T>, BasicFormatContext<char, CharBuffer>>::Write(t, formater);
		formater.BufferPushEndChar();
	}

	template<std::size_t BUFFER_SIZE = 256, typename CharBuffer = char, typename T>
	void CFilePrint(FILE* stream, T&& t) {
		CharBuffer buffer[BUFFER_SIZE];
		BasicFormatContext<char, CharBuffer> formater(std::basic_string_view<char>(), buffer, BUFFER_SIZE);
		FormatType<GetBaseType<T>, BasicFormatContext<char, CharBuffer>>::Write(t, formater);

		std::fwrite(formater.GetBuffer(), formater.GetCurrentBufferSize(), 1, stream);
		std::fflush(stream);
	}

	template<std::size_t BUFFER_SIZE = 256, typename CharBuffer = char, typename T>
	void CFilePrintLn(FILE* stream, T&& t) {
		CharBuffer buffer[BUFFER_SIZE];
		BasicFormatContext<char, CharBuffer> formater(std::basic_string_view<char>(), buffer, BUFFER_SIZE);
		FormatType<GetBaseType<T>, BasicFormatContext<char, CharBuffer>>::Write(t, formater);
		formater.BufferPushBack('\n');

		std::fwrite(formater.GetBuffer(), formater.GetCurrentBufferSize(), 1, stream);
		std::fflush(stream);
	}

	template<std::size_t BUFFER_SIZE = 256, typename CharBuffer = char, typename T>
	void FilePrint(std::basic_ostream<CharBuffer>& stream, T&& t) {
		CharBuffer buffer[BUFFER_SIZE];
		BasicFormatContext<char, CharBuffer> formater(std::basic_string_view<char>(), buffer, BUFFER_SIZE);
		FormatType<GetBaseType<T>, BasicFormatContext<char, CharBuffer>>::Write(t, formater);

		stream.write(formater.GetBuffer(), formater.GetCurrentBufferSize());
		stream.flush();
	}

	template<std::size_t BUFFER_SIZE = 256, typename CharBuffer = char, typename T>
	void FilePrintLn(std::basic_ostream<CharBuffer>& stream, T&& t) {
		CharBuffer buffer[BUFFER_SIZE];
		BasicFormatContext<char, CharBuffer> formater(std::basic_string_view<char>(), buffer, BUFFER_SIZE);
		FormatType<GetBaseType<T>, BasicFormatContext<char, CharBuffer>>::Write(t, formater);
		formater.BufferPushBack('\n');

		stream.write(formater.GetBuffer(), formater.GetCurrentBufferSize());
		stream.flush();
	}

	template<std::size_t BUFFER_SIZE = 256, typename CharBuffer = char, typename T>
	void FormatInString(std::basic_string<CharBuffer>& str, T&& t) {
		CharBuffer buffer[BUFFER_SIZE];
		BasicFormatContext<char, CharBuffer> formater(std::basic_string_view<char>(), buffer, BUFFER_SIZE);
		FormatType<GetBaseType<T>, BasicFormatContext<char, CharBuffer>>::Write(t, formater);
		formater.BufferPushEndChar();
		str = formater.GetBuffer();
	}

	template<std::size_t BUFFER_SIZE = 256, typename CharBuffer = char, typename T>
	inline std::basic_string<CharBuffer> FormatString(T&& t) {
		CharBuffer buffer[BUFFER_SIZE];
		BasicFormatContext<char, CharBuffer> formater(std::basic_string_view<char>(), buffer, BUFFER_SIZE);
		FormatType<GetBaseType<T>, BasicFormatContext<char, CharBuffer>>::Write(t, formater);
		formater.BufferPushEndChar();
		return formater.GetBuffer();
	}

}


