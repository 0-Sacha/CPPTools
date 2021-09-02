#pragma once

#include "BasicFormatContext.h"
#include "BasicFormatContextClassImpl.h"

namespace CPPTools::Fmt {

	/////---------- Get format parameter ----------/////
	template<typename CharFormat, typename CharBuffer, typename ...ContextArgs>
	template<typename T>
	bool BasicFormatContext<CharFormat, CharBuffer, ContextArgs...>::FormatReadParameter(T& i) {
		if (!m_FormatStr.IsEqualTo('{'))	return FormatStr().ReadUInt(i);

		const CharFormat* const mainSubFormat = m_FormatStr.GetBufferCurrentPos();
		FormatIdx formatIdx = FormatIdxNotFound;
		if (GetFormatIdx(formatIdx)) {
			m_FormatStr.Forward();
			m_ContextArgs.GetFormatValueAt(i, formatIdx);
			return true;
		}
		m_FormatStr.SetBufferCurrentPos(mainSubFormat);
		return false;
	}


	/////---------- Impl ----------/////
	template<typename CharFormat, typename CharBuffer, typename ...ContextArgs>
	void BasicFormatContext<CharFormat, CharBuffer, ContextArgs...>::ParameterData() {
		if (m_FormatStr.IsEqualTo(':')) {
			m_FormatData.HasSpec = true;
			while (!m_FormatStr.IsEndOfParameter()) {
				m_FormatStr.Forward();
				m_FormatStr.IgnoreSpace();

				if (m_FormatStr.IsEqualForward('{')) {		// Forward specifier
					FormatIdx dataIdx;
					GetFormatIdx(dataIdx);
					m_ContextArgs.GetParameterDataFromIdx(*this, dataIdx);
					m_FormatStr.Forward();

				} else if (m_FormatStr.IsEqualForward('C'))	{ m_FormatData.HasChangeColor = true; m_ColorMem = Detail::AnsiColorMem(); ColorValuePrint();

				} else if (m_FormatStr.IsEqualForward('='))	{ m_FormatData.BaseValue = true;

				} else if (m_FormatStr.IsEqualForward('B'))	{ m_FormatData.IntPrint = Detail::ValueIntPrint::Bin;	FormatReadParameter(m_FormatData.Precision);
				} else if (m_FormatStr.IsEqualForward('X'))	{ m_FormatData.IntPrint = Detail::ValueIntPrint::Hex;	FormatReadParameter(m_FormatData.Precision);
				} else if (m_FormatStr.IsEqualForward('O'))	{ m_FormatData.IntPrint = Detail::ValueIntPrint::Oct;	FormatReadParameter(m_FormatData.Precision);
				} else if (m_FormatStr.IsEqualForward('D'))	{ m_FormatData.IntPrint = Detail::ValueIntPrint::Int;	FormatReadParameter(m_FormatData.Precision);
				} else if (m_FormatStr.IsEqualForward('.'))	{ FormatReadParameter(m_FormatData.FloatPrecision);

				} else if (m_FormatStr.IsEqualForward('>'))	{ m_FormatData.ShiftType = Detail::ShiftType::Right;	FormatReadParameter(m_FormatData.ShiftValue);
				} else if (m_FormatStr.IsEqualForward('<'))	{ m_FormatData.ShiftType = Detail::ShiftType::Left;		FormatReadParameter(m_FormatData.ShiftValue);
				} else if (m_FormatStr.IsEqualForward('^'))	{ m_FormatData.ShiftType = Detail::ShiftType::Center;	FormatReadParameter(m_FormatData.ShiftValue);
				} else if (m_FormatStr.IsEqualForward('0'))	{ m_FormatData.ShiftPrint = Detail::ShiftPrint::Zeros;

				} else if (m_FormatStr.IsLowerCase()) {	// Custom Specifier / NameSpecifier
					const char* namePos = m_FormatStr.GetBufferCurrentPos();
					m_FormatStr.ParamGoTo(' ', '=');
					StringViewFormat name(namePos, m_FormatStr.GetBufferCurrentPos() - namePos);

					m_FormatStr.ParamGoToForward('=');
					m_FormatStr.IgnoreSpace();

					if (m_FormatStr.IsEqualForward('\'')) {
						const char* valuePos = m_FormatStr.GetBufferCurrentPos();
						m_FormatStr.ParamGoTo('\'');
						std::size_t valueSize = m_FormatStr.GetBufferCurrentPos() - valuePos;
						m_FormatData.AddSpecifier(name, StringViewFormat(valuePos, valueSize));
					} else if(m_FormatStr.IsADigit()) {
						std::intmax_t value = 0;
						m_FormatStr.ReadInt(value);
						m_FormatData.AddSpecifier(name, value);
					}
				}

				m_FormatStr.ParamGoTo(',');
			}
		}
	}

	template<typename CharFormat, typename CharBuffer, typename ...ContextArgs>
	bool BasicFormatContext<CharFormat, CharBuffer, ContextArgs...>::GetFormatIdx(FormatIdx& idx) {
		const CharFormat* mainSubFormat = m_FormatStr.GetBufferCurrentPos();

		// I : if there is no number specified : ':' or '}'
		if (m_FormatStr.IsEqualTo(':') || m_FormatStr.IsEqualTo('}')) {
			idx = m_ValuesIdx++;
			if(idx < m_ContextArgs.Size())	return true;
			--m_ValuesIdx;
		}

		// II: A number(idx)
		if (m_FormatStr.ReadUInt(idx))
			if (m_FormatStr.IsEqualTo(':') || m_FormatStr.IsEqualTo('}'))
				if (idx < m_ContextArgs.Size())	return true;
		m_FormatStr.SetBufferCurrentPos(mainSubFormat);

		// III : A name
		m_ContextArgs.GetNamedArgsIdx(*this, idx, 0);
		if (idx < m_ContextArgs.Size() /* || idx != FormatIdxNotFound */)
			return true;
		m_FormatStr.SetBufferCurrentPos(mainSubFormat);

		// VI : { which is a idx to a number
		if (m_FormatStr.IsEqualForward('{'))
			if (GetFormatIdx(idx))
				if (idx < m_ContextArgs.Size())	return true;
		m_FormatStr.SetBufferCurrentPos(mainSubFormat);

		return false;
	}

	template<typename CharFormat, typename CharBuffer, typename ...ContextArgs>
	bool BasicFormatContext<CharFormat, CharBuffer, ContextArgs...>::ParameterPrint() {
		m_FormatStr.Forward();				// Skip {

		if (m_FormatStr.IsUpperCase()) {
			if		(m_FormatStr.IsEqualTo('C') && m_FormatStr.NextIsEqualForward(':', '}'))		ColorValuePrint();
			else if (m_FormatStr.IsEqualTo('T') && m_FormatStr.NextIsEqualForward(':', '}'))		TimerValuePrint();
			else if (m_FormatStr.IsEqualTo('D') && m_FormatStr.NextIsEqualForward(':', '}'))		DateValuePrint();
		} else {
			FormatIdx formatIdx;
			if (!GetFormatIdx(formatIdx))	return false;
			else {
				FormatDataType data;
				data.Clone(m_FormatData);
				m_FormatData = FormatDataType();

				Detail::AnsiColorMem colorMem(m_ColorMem);

				if (!m_FormatData.IsInit)		ParameterData();

				m_ContextArgs.FormatTypeFromIdx(*this, formatIdx);

				if (m_FormatData.HasChangeColor) { m_ColorMem = colorMem; ReloadColor(); }
				m_FormatData.Clone(data);
			}
		}

		m_FormatStr.GoOutOfParameter();		// Skip}
		return true;
	}

	template<typename CharFormat, typename CharBuffer, typename ...ContextArgs>
	void BasicFormatContext<CharFormat, CharBuffer, ContextArgs...>::Format() {
		while (!m_FormatStr.End()) {

			WriteUntilNextParameter();

			if (m_FormatStr.IsEqualTo('{'))
				if (!ParameterPrint())	m_BufferOut.PushBack('{');
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
		BasicFormatContext<NewCharFormat, CharBuffer, Args...> child(format, *this, std::forward<Args>(args)...);
		child.Format();
		UpdateContextFromChild(child);
	}







	/////---------- Impl with as Format ----------//////

	namespace Detail {
		template<typename CharBuffer, typename CharFormat, typename ...Args>
		Detail::FormatterMemoryBufferOutCopy<CharBuffer> FormatAndGetBufferOut(const std::basic_string_view<CharFormat> format, Args&& ...args) {
			BasicFormatContext<CharFormat, CharBuffer, Args...> context(true, format, std::forward<Args>(args)...);
			context.MainFormat();
			context.BufferOut().PushEndChar();
			return context.BufferOut();
		}
		template<typename CharBuffer, typename CharFormat, std::size_t BUFFER_SIZE, std::size_t FORMAT_SIZE, typename ...Args>
		Detail::FormatterMemoryBufferOutCopy<CharBuffer> FormatAndGetBufferOut(CharFormat(&format)[FORMAT_SIZE], Args&& ...args) {
			return FormatAndGetBufferOut<CharFormat, CharBuffer>(std::basic_string_view<CharFormat>(format), std::forward<Args>(args)...);
		}
	}


	template<typename CharBuffer, typename CharFormat, std::size_t BUFFER_SIZE, typename ...Args>
	void FormatInChar(CharBuffer(&buffer)[BUFFER_SIZE], const std::basic_string_view<CharFormat> format, Args&& ...args) {
		BasicFormatContext<CharFormat, CharBuffer, Args...> context(format, buffer, BUFFER_SIZE, std::forward<Args>(args)...);
		context.MainFormat();
		context.BufferOut().PushEndChar();
	}
	template<typename CharBuffer, typename CharFormat, std::size_t BUFFER_SIZE, std::size_t FORMAT_SIZE, typename ...Args>
	inline void FormatInChar(CharBuffer(&buffer)[BUFFER_SIZE], const CharFormat(&format)[FORMAT_SIZE], Args&& ...args) {
		FormatInChar<CharFormat, CharBuffer>(buffer, std::basic_string_view<CharFormat>(format), std::forward<Args>(args)...);
	}
	
	template<typename CharBuffer, typename CharFormat, typename ...Args>
	void FormatInChar(CharBuffer* const buffer, const std::size_t bufferSize, const std::basic_string_view<CharFormat> format, Args&& ...args) {
		BasicFormatContext<CharFormat, CharBuffer, Args...> context(format, buffer, bufferSize, std::forward<Args>(args)...);
		context.MainFormat();
		context.BufferOut().PushEndChar();
	}
	template<typename CharBuffer, typename CharFormat, std::size_t FORMAT_SIZE, typename ...Args>
	void FormatInChar(CharBuffer* const buffer, const std::size_t bufferSize, const CharFormat(&format)[FORMAT_SIZE], Args&& ...args) {
		FormatInChar<CharFormat, CharBuffer>(buffer, bufferSize, std::basic_string_view<CharFormat>(format), std::forward<Args>(args)...);
	}
	

	template<typename CharBuffer = char, typename CharFormat, typename ...Args>
	void CFilePrint(FILE* stream, const std::basic_string_view<CharFormat> format, Args&& ...args) {
		BasicFormatContext<CharFormat, CharBuffer, Args...> context(true, format, std::forward<Args>(args)...);
		context.MainFormat();
	
		std::fwrite(context.BufferOut().GetBuffer(), context.BufferOut().GetBufferCurrentSize(), 1, stream);
		std::fflush(stream);
	}
	template<typename CharBuffer = char, typename CharFormat, std::size_t FORMAT_SIZE, typename ...Args>
	void CFilePrint(FILE* stream, const CharFormat(&format)[FORMAT_SIZE], Args&& ...args) {
		CFilePrint<CharFormat, CharBuffer>(stream, std::basic_string_view<CharFormat>(format), std::forward<Args>(args)...);
	}
	
	template<typename CharFormat = char, typename CharBuffer = CharFormat, typename ...Args>
	void CFilePrintLn(FILE* stream, const std::basic_string_view<CharFormat> format, Args&& ...args) {
		BasicFormatContext<CharFormat, CharBuffer, Args...> context(true, format, std::forward<Args>(args)...);
		context.MainFormat();
		context.BufferOut().PushBack('\n');
	
		std::fwrite(context.BufferOut().GetBuffer(), context.BufferOut().GetBufferCurrentSize(), 1, stream);
		std::fflush(stream);
	}
	template<typename CharFormat = char, typename CharBuffer = CharFormat, std::size_t FORMAT_SIZE, typename ...Args>
	void CFilePrintLn(FILE* stream, const CharFormat(&format)[FORMAT_SIZE], Args&& ...args) {
		CFilePrintLn<CharFormat, CharBuffer>(stream, std::basic_string_view<CharFormat>(format), std::forward<Args>(args)...);
	}
	

	template<typename CharBuffer, typename CharFormat, typename ...Args>
	void FilePrint(std::basic_ostream<CharBuffer>& stream, const std::basic_string_view<CharFormat> format, Args&& ...args) {
		BasicFormatContext<CharFormat, CharBuffer, Args...> context(true, format, std::forward<Args>(args)...);
		context.MainFormat();
	
		stream.write(context.BufferOut().GetBuffer(), context.BufferOut().GetBufferCurrentSize());
		stream.flush();
	}
	template<typename CharBuffer, typename CharFormat, std::size_t FORMAT_SIZE, typename ...Args>
	inline void FilePrint(std::basic_ostream<CharBuffer>& stream, const CharFormat(&format)[FORMAT_SIZE], Args&& ...args) {
		FilePrint<CharFormat, CharBuffer>(stream, std::basic_string_view<CharFormat>(format), std::forward<Args>(args)...);
	}
	
	template<typename CharBuffer, typename CharFormat, typename ...Args>
	void FilePrintLn(std::basic_ostream<CharBuffer>& stream, const std::basic_string_view<CharFormat> format, Args&& ...args) {
		BasicFormatContext<CharFormat, CharBuffer, Args...> context(true, format, std::forward<Args>(args)...);
		context.MainFormat();
		context.BufferOut().PushBack('\n');
	
		stream.write(context.BufferOut().GetBuffer(), context.BufferOut().GetBufferCurrentSize());
		stream.flush();
	}
	template<typename CharBuffer, typename CharFormat, std::size_t FORMAT_SIZE, typename ...Args>
	inline void FilePrintLn(std::basic_ostream<CharBuffer>& stream, const CharFormat(&format)[FORMAT_SIZE], Args&& ...args) {
		FilePrintLn<CharFormat, CharBuffer>(stream, std::basic_string_view<CharFormat>(format), std::forward<Args>(args)...);
	}
	

	template<typename CharBuffer, typename CharFormat, typename ...Args>
	void FormatInString(std::basic_string<CharBuffer>& str, const std::basic_string_view<CharFormat> format, Args&& ...args) {
		BasicFormatContext<CharFormat, CharBuffer, Args...> context(true, format, std::forward<Args>(args)...);
		context.MainFormat();
		context.BufferOut().PushEndChar();
		str = context.BufferOut().GetBuffer();
	}
	template<typename CharBuffer, typename CharFormat, std::size_t FORMAT_SIZE, typename ...Args>
	inline void FormatInString(std::basic_string<CharBuffer>& str, const CharFormat(&format)[FORMAT_SIZE], Args&& ...args) {
		FormatInString<CharFormat, CharBuffer>(str, std::basic_string_view<CharFormat>(format), std::forward<Args>(args)...);
	}
	
	template<typename CharBuffer, typename CharFormat, typename ...Args>
	inline std::basic_string<CharBuffer> FormatString(const std::basic_string_view<CharFormat> format, Args&& ...args) {
		BasicFormatContext<CharFormat, CharBuffer, Args...> context(true, format, std::forward<Args>(args)...);
		context.MainFormat();
		context.BufferOut().PushEndChar();
		return context.BufferOut().GetBuffer();
	}
	template<typename CharBuffer, typename CharFormat, std::size_t FORMAT_SIZE, typename ...Args>
	inline std::basic_string<CharBuffer> FormatString(const CharFormat(&format)[FORMAT_SIZE], Args&& ...args) {
		return FormatString<CharFormat, CharBuffer>(std::basic_string_view<CharFormat>(format), std::forward<Args>(args)...);
	}
	
	/////---------- NO-FORMAT Impl ----------//////
	
	template<typename CharBuffer, size_t BUFFER_SIZE, typename T>
	void FormatInChar(CharBuffer(&buffer)[BUFFER_SIZE], T&& t) {
		BasicFormatContext<char, CharBuffer> context(std::basic_string_view<char>(nullptr, 0), buffer, BUFFER_SIZE);
		context.WriteType(t);
		context.BufferOut().PushEndChar();
	}
	
	template<typename CharBuffer, typename T>
	void FormatInChar(CharBuffer* const buffer, const std::size_t bufferSize, T&& t) {
		BasicFormatContext<char, CharBuffer> context(std::basic_string_view<char>(nullptr, 0), buffer, bufferSize);
		context.WriteType(t);
		context.BufferOut().PushEndChar();
	}

	
	template<typename CharBuffer = char, typename T>
	void CFilePrint(FILE* stream, T&& t) {
		BasicFormatContext<char, CharBuffer> context(true, std::basic_string_view<char>(nullptr, 0));
		context.WriteType(t);
	
		std::fwrite(context.BufferOut().GetBuffer(), context.BufferOut().GetBufferCurrentSize(), 1, stream);
		std::fflush(stream);
	}
	
	template<typename CharBuffer = char, typename T>
	void CFilePrintLn(FILE* stream, T&& t) {
		BasicFormatContext<char, CharBuffer> context(true, std::basic_string_view<char>(nullptr, 0));
		context.WriteType(t);
		context.BufferOut().PushBack('\n');
	
		std::fwrite(context.BufferOut().GetBuffer(), context.BufferOut().GetBufferCurrentSize(), 1, stream);
		std::fflush(stream);
	}
	

	template<typename CharBuffer, typename T>
	void FilePrint(std::basic_ostream<CharBuffer>& stream, T&& t) {
		BasicFormatContext<char, CharBuffer> context(true, std::basic_string_view<char>(nullptr, 0));
		context.WriteType(t);
	
		stream.write(context.BufferOut().GetBuffer(), context.BufferOut().GetBufferCurrentSize());
		stream.flush();
	}
	
	template<typename CharBuffer, typename T>
	void FilePrintLn(std::basic_ostream<CharBuffer>& stream, T&& t) {
		BasicFormatContext<char, CharBuffer> context(true, std::basic_string_view<char>(nullptr, 0));
		context.WriteType(t);
		context.BufferOut().PushBack('\n');
	
		stream.write(context.BufferOut().GetBuffer(), context.BufferOut().GetBufferCurrentSize());
		stream.flush();
	}

	
	template<typename CharBuffer, typename T>
	void FormatInString(std::basic_string<CharBuffer>& str, T&& t) {
		BasicFormatContext<char, CharBuffer> context(true, std::basic_string_view<char>(nullptr, 0));
		context.WriteType(t);
		context.BufferOut().PushEndChar();
		str = context.BufferOut().GetBuffer();
	}
	
	template<typename CharBuffer, typename T>
	inline std::basic_string<CharBuffer> FormatString(T&& t) {
		BasicFormatContext<char, CharBuffer> context(true, std::basic_string_view<char>(nullptr, 0));
		context.WriteType(t);
		context.BufferOut().PushEndChar();
		return context.BufferOut().GetBuffer();
	}
}


