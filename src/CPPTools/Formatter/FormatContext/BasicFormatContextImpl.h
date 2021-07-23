#pragma once

#include "BasicFormatContext.h"
#include "BasicFormatContextClassImpl.h"

namespace CPPTools::Fmt {

	/////---------- Get format parameter ----------/////
	template<typename CharFormat, typename CharBuffer, typename ...ContextArgs>
	template<typename T>
	bool BasicFormatContext<CharFormat, CharBuffer, ContextArgs...>::FormatReadParameter(T& i) {
		if (!FormatIsEqualTo('{'))	return FormatReadUInt(i);

		const CharFormat* const mainSubFormat = m_SubFormat;
		FormatIdx formatIdx = FormatIdxNotFound;
		if (GetFormatIdx(formatIdx)) {
			FormatForward();
			m_ContextArgs.GetFormatValueAt(i, formatIdx);
			return true;
		}
		m_SubFormat = mainSubFormat;
		return false;
	}


	/////---------- Impl ----------/////
	template<typename CharFormat, typename CharBuffer, typename ...ContextArgs>
	void BasicFormatContext<CharFormat, CharBuffer, ContextArgs...>::ParameterData() {
		if (FormatIsEqualTo(':')) {
			m_FormatData.HasSpec = true;
			while (!FormatIsEndOfParameter()) {
				FormatForward();
				FormatIgnoreSpace();

				if (FormatIsEqualForward('{')) {		// Forward specifier
					FormatIdx dataIdx;
					GetFormatIdx(dataIdx);
					m_ContextArgs.GetParameterDataFromIdx(*this, dataIdx);
					FormatForward();

				} else if (FormatIsEqualForward('C'))	{ m_FormatData.HasChangeColor = true; m_ColorMem = Detail::AnsiColorMem(); ColorValuePrint();

				} else if (FormatIsEqualForward('='))	{ m_FormatData.BaseValue = true;

				} else if (FormatIsEqualForward('B'))	{ m_FormatData.IntPrint = Detail::ValueIntPrint::Bin;	FormatReadParameter(m_FormatData.Precision);
				} else if (FormatIsEqualForward('X'))	{ m_FormatData.IntPrint = Detail::ValueIntPrint::Hex;	FormatReadParameter(m_FormatData.Precision);
				} else if (FormatIsEqualForward('O'))	{ m_FormatData.IntPrint = Detail::ValueIntPrint::Oct;	FormatReadParameter(m_FormatData.Precision);
				} else if (FormatIsEqualForward('D'))	{ m_FormatData.IntPrint = Detail::ValueIntPrint::Int;	FormatReadParameter(m_FormatData.Precision);
				} else if (FormatIsEqualForward('.'))	{ FormatReadParameter(m_FormatData.FloatPrecision);

				} else if (FormatIsEqualForward('>'))	{ m_FormatData.ShiftType = Detail::ShiftType::Right;	FormatReadParameter(m_FormatData.ShiftValue);
				} else if (FormatIsEqualForward('<'))	{ m_FormatData.ShiftType = Detail::ShiftType::Left;		FormatReadParameter(m_FormatData.ShiftValue);
				} else if (FormatIsEqualForward('^'))	{ m_FormatData.ShiftType = Detail::ShiftType::Center;	FormatReadParameter(m_FormatData.ShiftValue);
				} else if (FormatIsEqualForward('0'))	{ m_FormatData.ShiftPrint = Detail::ShiftPrint::Zeros;

				} else if (FormatIsLowerCase()) {	// Custom Specifier / NameSpecifier
					const char* namePos = GetSubFormat();
					FormatParamGoTo(' ', '=');
					std::string_view name(namePos, GetSubFormat() - namePos);

					FormatParamGoToForward('=');
					FormatIgnoreSpace();

					if (FormatIsEqualForward('\'')) {
						const char* valuePos = GetSubFormat();
						FormatParamGoTo('\'');
						std::size_t valueSize = GetSubFormat() - valuePos;
						m_FormatData.AddSpecifier(name, std::string_view(valuePos, valueSize));
					} else if(FormatIsADigit()) {
						std::intmax_t value = 0;
						FormatReadInt(value);
						m_FormatData.AddSpecifier(name, value);
					}
				}

				FormatParamGoTo(',');
			}
		}
	}

	template<typename CharFormat, typename CharBuffer, typename ...ContextArgs>
	bool BasicFormatContext<CharFormat, CharBuffer, ContextArgs...>::GetFormatIdx(FormatIdx& idx) {
		const CharFormat* mainSubFormat = m_SubFormat;

		// I : if there is no number specified : ':' or '}'
		if (FormatIsEqualTo(':') || FormatIsEqualTo('}')) {
			idx = m_ValuesIdx++;
			if(idx < m_ContextArgsSize)	return true;
			--m_ValuesIdx;
		}

		// II: A number(idx)
		if (FormatReadUInt(idx))
			if (FormatIsEqualTo(':') || FormatIsEqualTo('}'))
				if (idx < m_ContextArgsSize)	return true;
		m_SubFormat = mainSubFormat;

		// III : A name
		m_ContextArgs.GetNamedArgsIdx(*this, idx, 0);
		if (idx < m_ContextArgsSize /* || idx != FormatIdxNotFound */)
			return true;
		m_SubFormat = mainSubFormat;

		// VI : { which is a idx to a number
		if (FormatIsEqualForward('{'))
			if (GetFormatIdx(idx))
				if (idx < m_ContextArgsSize)	return true;
		m_SubFormat = mainSubFormat;

		return false;
	}

	template<typename CharFormat, typename CharBuffer, typename ...ContextArgs>
	bool BasicFormatContext<CharFormat, CharBuffer, ContextArgs...>::ParameterPrint() {
		FormatForward();				// Skip {

		if (FormatIsEqualTo('C') && FormatNextIsEqualForward(':', '}'))			ColorValuePrint();
		else if (FormatIsEqualTo('T') && FormatNextIsEqualForward(':', '}'))	TimerValuePrint();
		else if (FormatIsEqualTo('D') && FormatNextIsEqualForward(':', '}'))	DateValuePrint();
		else {
			FormatIdx formatIdx;
			if (!GetFormatIdx(formatIdx))	return false;
			else {
				FormatData data;
				data.Clone(m_FormatData);
				m_FormatData = FormatData();

				Detail::AnsiColorMem colorMem(m_ColorMem);

				if (!m_FormatData.IsInit)		ParameterData();

				m_ContextArgs.FormatTypeFromIdx(*this, formatIdx);

				if (m_FormatData.HasChangeColor) { m_ColorMem = colorMem; ReloadColor(); }
				m_FormatData.Clone(data);
			}
		}

		FormatGoOutOfParameter();		// Skip}
		return true;
	}

	template<typename CharFormat, typename CharBuffer, typename ...ContextArgs>
	void BasicFormatContext<CharFormat, CharBuffer, ContextArgs...>::Format() {
		while (!FormatEnd()) {

			WriteUntilNextParameter();

			if (FormatIsEqualTo('{'))
				if (!ParameterPrint())	BufferPushBack('{');
		}
	}

	template<typename CharFormat, typename CharBuffer, typename ...ContextArgs>
	void BasicFormatContext<CharFormat, CharBuffer, ContextArgs...>::MainFormat() {
		Format();
		CheckEndStr();
	}

	template<typename CharFormat, typename CharBuffer, typename ...ContextArgs>
	template<typename NewCharFormat, typename ...Args>
	void BasicFormatContext<CharFormat, CharBuffer, ContextArgs...>::LittleFormat(const std::basic_string_view<NewCharFormat> format, Args&& ...args) {
		BasicFormatContext<NewCharFormat, CharBuffer, Args...> newContext(format, *this, std::forward<Args>(args)...);
		newContext.Format();
		newContext.UpdateOldContext(*this);
	}

	/////---------- Impl with as Format ----------//////


	template<typename CharFormat = char, typename CharBuffer = CharFormat, std::size_t BUFFER_SIZE, typename ...Args>
	void FormatInChar(CharBuffer(&buffer)[BUFFER_SIZE], const std::basic_string_view<CharFormat> format, Args&& ...args) {
		BasicFormatContext<CharFormat, CharBuffer, Args...> context(format, buffer, BUFFER_SIZE, std::forward<Args>(args)...);
		context.MainFormat();
		context.BufferPushEndChar();
	}
	template<typename CharFormat = char, typename CharBuffer = CharFormat, std::size_t BUFFER_SIZE, std::size_t FORMAT_SIZE, typename ...Args>
	inline void FormatInChar(CharBuffer(&buffer)[BUFFER_SIZE], const CharFormat(&format)[FORMAT_SIZE], Args&& ...args) {
		FormatInChar<CharFormat, CharBuffer>(buffer, std::basic_string_view<CharFormat>(format), std::forward<Args>(args)...);
	}
	
	template<typename CharFormat = char, typename CharBuffer = CharFormat, typename ...Args>
	void FormatInChar(CharBuffer* const buffer, const std::size_t bufferSize, const std::basic_string_view<CharFormat> format, Args&& ...args) {
		BasicFormatContext<CharFormat, CharBuffer, Args...> context(format, buffer, bufferSize, std::forward<Args>(args)...);
		context.MainFormat();
		context.BufferPushEndChar();
	}
	template<typename CharFormat = char, typename CharBuffer = CharFormat, std::size_t FORMAT_SIZE, typename ...Args>
	void FormatInChar(CharBuffer* const buffer, const std::size_t bufferSize, const CharFormat(&format)[FORMAT_SIZE], Args&& ...args) {
		FormatInChar<CharFormat, CharBuffer>(buffer, bufferSize, std::basic_string_view<CharFormat>(format), std::forward<Args>(args)...);
	}
	
	template<std::size_t BUFFER_SIZE = 1024, typename CharFormat = char, typename CharBuffer = CharFormat, typename ...Args>
	void CFilePrint(FILE* stream, const std::basic_string_view<CharFormat> format, Args&& ...args) {
		CharBuffer buffer[BUFFER_SIZE];
		BasicFormatContext<CharFormat, CharBuffer, Args...> context(format, buffer, BUFFER_SIZE, std::forward<Args>(args)...);
		context.MainFormat();
	
		std::fwrite(context.GetBuffer(), context.GetSize(), 1, stream);
		std::fflush(stream);
	}
	template<std::size_t BUFFER_SIZE = 1024, typename CharFormat = char, typename CharBuffer = CharFormat, std::size_t FORMAT_SIZE, typename ...Args>
	void CFilePrint(FILE* stream, const CharFormat(&format)[FORMAT_SIZE], Args&& ...args) {
		CFilePrint<BUFFER_SIZE, CharFormat, CharBuffer>(stream, std::basic_string_view<CharFormat>(format), std::forward<Args>(args)...);
	}
	
	template<std::size_t BUFFER_SIZE = 1024, typename CharFormat = char, typename CharBuffer = CharFormat, typename ...Args>
	void CFilePrintLn(FILE* stream, const std::basic_string_view<CharFormat> format, Args&& ...args) {
		CharBuffer buffer[BUFFER_SIZE];
		BasicFormatContext<CharFormat, CharBuffer, Args...> context(format, buffer, BUFFER_SIZE, std::forward<Args>(args)...);
		context.MainFormat();
		context.BufferPushBack('\n');
	
		std::fwrite(context.GetBuffer(), context.GetSize(), 1, stream);
		std::fflush(stream);
	}
	template<std::size_t BUFFER_SIZE = 1024, typename CharFormat = char, typename CharBuffer = CharFormat, std::size_t FORMAT_SIZE, typename ...Args>
	void CFilePrintLn(FILE* stream, const CharFormat(&format)[FORMAT_SIZE], Args&& ...args) {
		CFilePrintLn<BUFFER_SIZE, CharFormat, CharBuffer>(stream, std::basic_string_view<CharFormat>(format), std::forward<Args>(args)...);
	}
	
	template<std::size_t BUFFER_SIZE = 1024, typename CharFormat = char, typename CharBuffer = CharFormat, typename ...Args>
	void FilePrint(std::basic_ostream<CharBuffer>& stream, const std::basic_string_view<CharFormat> format, Args&& ...args) {
		CharBuffer buffer[BUFFER_SIZE];
		BasicFormatContext<CharFormat, CharBuffer, Args...> context(format, buffer, BUFFER_SIZE, std::forward<Args>(args)...);
		context.MainFormat();
	
		stream.write(context.GetBuffer(), context.GetCurrentBufferSize());
		stream.flush();
	}
	template<std::size_t BUFFER_SIZE = 1024, typename CharFormat = char, typename CharBuffer = CharFormat, std::size_t FORMAT_SIZE, typename ...Args>
	inline void FilePrint(std::basic_ostream<CharBuffer>& stream, const CharFormat(&format)[FORMAT_SIZE], Args&& ...args) {
		FilePrint<BUFFER_SIZE, CharFormat, CharBuffer>(stream, std::basic_string_view<CharFormat>(format), std::forward<Args>(args)...);
	}
	
	template<std::size_t BUFFER_SIZE = 1024, typename CharFormat = char, typename CharBuffer = CharFormat, typename ...Args>
	void FilePrintLn(std::basic_ostream<CharBuffer>& stream, const std::basic_string_view<CharFormat> format, Args&& ...args) {
		CharBuffer buffer[BUFFER_SIZE];
		BasicFormatContext<CharFormat, CharBuffer, Args...> context(format, buffer, BUFFER_SIZE, std::forward<Args>(args)...);
		context.MainFormat();
		context.BufferPushBack('\n');
	
		stream.write(context.GetBuffer(), context.GetCurrentBufferSize());
		stream.flush();
	}
	template<std::size_t BUFFER_SIZE = 1024, typename CharFormat = char, typename CharBuffer = CharFormat, std::size_t FORMAT_SIZE, typename ...Args>
	inline void FilePrintLn(std::basic_ostream<CharBuffer>& stream, const CharFormat(&format)[FORMAT_SIZE], Args&& ...args) {
		FilePrintLn<BUFFER_SIZE, CharFormat, CharBuffer>(stream, std::basic_string_view<CharFormat>(format), std::forward<Args>(args)...);
	}
	
	template<std::size_t BUFFER_SIZE = 1024, typename CharFormat = char, typename CharBuffer = CharFormat, typename ...Args>
	void FormatInString(std::basic_string<CharBuffer>& str, const std::basic_string_view<CharFormat> format, Args&& ...args) {
		CharBuffer buffer[BUFFER_SIZE];
		BasicFormatContext<CharFormat, CharBuffer, Args...> context(format, buffer, BUFFER_SIZE, std::forward<Args>(args)...);
		context.MainFormat();
		context.BufferPushEndChar();
		str = context.GetBuffer();
	}
	template<std::size_t BUFFER_SIZE = 1024, typename CharFormat = char, typename CharBuffer = CharFormat, std::size_t FORMAT_SIZE, typename ...Args>
	inline void FormatInString(std::basic_string<CharBuffer>& str, const CharFormat(&format)[FORMAT_SIZE], Args&& ...args) {
		FormatInString<BUFFER_SIZE, CharFormat, CharBuffer>(str, std::basic_string_view<CharFormat>(format), std::forward<Args>(args)...);
	}
	
	template<std::size_t BUFFER_SIZE = 1024, typename CharFormat = char, typename CharBuffer = CharFormat, typename ...Args>
	inline std::basic_string<CharBuffer> FormatString(const std::basic_string_view<CharFormat> format, Args&& ...args) {
		CharBuffer buffer[BUFFER_SIZE];
		BasicFormatContext<CharFormat, CharBuffer, Args...> context(format, buffer, BUFFER_SIZE, std::forward<Args>(args)...);
		context.MainFormat();
		context.BufferPushEndChar();
		return context.GetBuffer();
	}
	template<std::size_t BUFFER_SIZE = 1024, typename CharFormat = char, typename CharBuffer = CharFormat, std::size_t FORMAT_SIZE, typename ...Args>
	inline std::basic_string<CharBuffer> FormatString(const CharFormat(&format)[FORMAT_SIZE], Args&& ...args) {
		return FormatString<BUFFER_SIZE, CharFormat, CharBuffer>(std::basic_string_view<CharFormat>(format), std::forward<Args>(args)...);
	}
	
	/////---------- NO-FORMAT Impl ----------//////
	
	template<typename CharBuffer = char, size_t BUFFER_SIZE, typename T>
	void FormatInChar(CharBuffer(&buffer)[BUFFER_SIZE], T&& t) {
		BasicFormatContext<char, CharBuffer> context(std::basic_string_view<char>(), buffer, BUFFER_SIZE);
		FormatType<Detail::GetBaseType<T>, BasicFormatContext<char, CharBuffer>>::Write(t, context);
		context.BufferPushEndChar();
	}
	
	template<typename CharBuffer = char, typename T>
	void FormatInChar(CharBuffer* const buffer, const std::size_t bufferSize, T&& t) {
		BasicFormatContext<char, CharBuffer> context(std::basic_string_view<char>(), buffer, bufferSize);
		FormatType<Detail::GetBaseType<T>, BasicFormatContext<char, CharBuffer>>::Write(t, context);
		context.BufferPushEndChar();
	}
	
	template<std::size_t BUFFER_SIZE = 256, typename CharBuffer = char, typename T>
	void CFilePrint(FILE* stream, T&& t) {
		CharBuffer buffer[BUFFER_SIZE];
		BasicFormatContext<char, CharBuffer> context(std::basic_string_view<char>(), buffer, BUFFER_SIZE);
		FormatType<Detail::GetBaseType<T>, BasicFormatContext<char, CharBuffer>>::Write(t, context);
	
		std::fwrite(context.GetBuffer(), context.GetCurrentBufferSize(), 1, stream);
		std::fflush(stream);
	}
	
	template<std::size_t BUFFER_SIZE = 256, typename CharBuffer = char, typename T>
	void CFilePrintLn(FILE* stream, T&& t) {
		CharBuffer buffer[BUFFER_SIZE];
		BasicFormatContext<char, CharBuffer> context(std::basic_string_view<char>(), buffer, BUFFER_SIZE);
		FormatType<Detail::GetBaseType<T>, BasicFormatContext<char, CharBuffer>>::Write(t, context);
		context.BufferPushBack('\n');
	
		std::fwrite(context.GetBuffer(), context.GetCurrentBufferSize(), 1, stream);
		std::fflush(stream);
	}
	
	template<std::size_t BUFFER_SIZE = 256, typename CharBuffer = char, typename T>
	void FilePrint(std::basic_ostream<CharBuffer>& stream, T&& t) {
		CharBuffer buffer[BUFFER_SIZE];
		BasicFormatContext<char, CharBuffer> context(std::basic_string_view<char>(), buffer, BUFFER_SIZE);
		FormatType<Detail::GetBaseType<T>, BasicFormatContext<char, CharBuffer>>::Write(t, context);
	
		stream.write(context.GetBuffer(), context.GetCurrentBufferSize());
		stream.flush();
	}
	
	template<std::size_t BUFFER_SIZE = 256, typename CharBuffer = char, typename T>
	void FilePrintLn(std::basic_ostream<CharBuffer>& stream, T&& t) {
		CharBuffer buffer[BUFFER_SIZE];
		BasicFormatContext<char, CharBuffer> context(std::basic_string_view<char>(), buffer, BUFFER_SIZE);
		FormatType<Detail::GetBaseType<T>, BasicFormatContext<char, CharBuffer>>::Write(t, context);
		context.BufferPushBack('\n');
	
		stream.write(context.GetBuffer(), context.GetCurrentBufferSize());
		stream.flush();
	}
	
	template<std::size_t BUFFER_SIZE = 256, typename CharBuffer = char, typename T>
	void FormatInString(std::basic_string<CharBuffer>& str, T&& t) {
		CharBuffer buffer[BUFFER_SIZE];
		BasicFormatContext<char, CharBuffer> context(std::basic_string_view<char>(), buffer, BUFFER_SIZE);
		FormatType<Detail::GetBaseType<T>, BasicFormatContext<char, CharBuffer>>::Write(t, context);
		context.BufferPushEndChar();
		str = context.GetBuffer();
	}
	
	template<std::size_t BUFFER_SIZE = 256, typename CharBuffer = char, typename T>
	inline std::basic_string<CharBuffer> FormatString(T&& t) {
		CharBuffer buffer[BUFFER_SIZE];
		BasicFormatContext<char, CharBuffer> context(std::basic_string_view<char>(), buffer, BUFFER_SIZE);
		FormatType<Detail::GetBaseType<T>, BasicFormatContext<char, CharBuffer>>::Write(t, context);
		context.BufferPushEndChar();
		return context.GetBuffer();
	}
}


