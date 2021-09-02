#pragma once

#include "BasicUnFormatContext.h"
#include "BasicUnFormatContextClassImpl.h"


namespace CPPTools::Fmt {

	template<typename FormatContext>
	struct FormatType<UnFormatContextError, FormatContext>
	{
		static void Write(const UnFormatContextError t, FormatContext& context) {
			if (t)	context.LittleFormat("FormatType error idx : {} -- Buffer error idx : {}", t.FormatPosError, t.BufferPosError);
			else	context.LittleFormat("No-Error");
		}
	};

	template<typename T, typename CharName, typename CharFormat, typename CharBuffer, typename ...ContextArgs>
	struct UnFormatType<StringViewNamedArgs<T, CharName>, BasicUnFormatContext<CharFormat, CharBuffer, ContextArgs...>>
	{
		inline static bool Read(StringViewNamedArgs<T, CharName>& t, BasicUnFormatContext<CharFormat, CharBuffer, ContextArgs...>& context) {
			return UnFormatType<Detail::GetBaseType<T>, BasicUnFormatContext<CharFormat, CharBuffer, ContextArgs...>>::Read(t.GetValue(), context);
		}
	};

	template<typename T, typename CharName, typename CharFormat, typename CharBuffer, typename ...ContextArgs>
	struct UnFormatType<StringNamedArgs<T, CharName>, BasicUnFormatContext<CharFormat, CharBuffer, ContextArgs...>>
	{
		inline static bool Read(StringNamedArgs<T, CharName>& t, BasicUnFormatContext<CharFormat, CharBuffer, ContextArgs...>& context) {
			return UnFormatType<Detail::GetBaseType<T>, BasicUnFormatContext<CharFormat, CharBuffer, ContextArgs...>>::Read(t.GetValue(), context);
		}
	};


	/////---------- Get format parameter ----------/////
	template<typename CharFormat, typename CharBuffer, typename ...ContextArgs>
	template<typename T>
	bool BasicUnFormatContext<CharFormat, CharBuffer, ContextArgs...>::FormatReadParameter(T& i) {
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
	void BasicUnFormatContext<CharFormat, CharBuffer, ContextArgs...>::ParameterData() {
		if (m_FormatStr.IsEqualTo(':')) {
			m_FormatData.HasSpec = true;
			while (!FormatIsEndOfParameter()) {
				m_FormatStr.Forward();
				m_FormatStr.IgnoreSpace();

				if (m_FormatStr.IsEqualForward('{')) {		// Forward specifier
					FormatIdx dataIdx;
					GetFormatIdx(dataIdx);
					m_ContextArgs.GetParameterDataFromIdx(*this, dataIdx);
					m_FormatStr.Forward();

				} else if (m_FormatStr.IsEqualForward('C'))	{ m_FormatData.HasChangeColor = true; m_ColorMem = Detail::AnsiColorMem(); GetColorValue();

				} else if (m_FormatStr.IsEqualForward('='))	{ m_FormatData.BaseValue = true;

				} else if (m_FormatStr.IsEqualForward('b'))	{ m_FormatData.IntPrint = Detail::ValueIntPrint::Bin;	FormatReadParameter(m_FormatData.Precision);
				} else if (m_FormatStr.IsEqualForward('x'))	{ m_FormatData.IntPrint = Detail::ValueIntPrint::Hex;	FormatReadParameter(m_FormatData.Precision);
				} else if (m_FormatStr.IsEqualForward('o'))	{ m_FormatData.IntPrint = Detail::ValueIntPrint::Oct;	FormatReadParameter(m_FormatData.Precision);
				} else if (m_FormatStr.IsEqualForward('d'))	{ m_FormatData.IntPrint = Detail::ValueIntPrint::Int;	FormatReadParameter(m_FormatData.Precision);
				} else if (m_FormatStr.IsEqualForward('.'))	{ FormatReadParameter(m_FormatData.FloatPrecision);

				} else if (m_FormatStr.IsEqualForward('>'))	{ m_FormatData.ShiftType = Detail::ShiftType::Right;	FormatReadParameter(m_FormatData.ShiftValue);
				} else if (m_FormatStr.IsEqualForward('<'))	{ m_FormatData.ShiftType = Detail::ShiftType::Left;		FormatReadParameter(m_FormatData.ShiftValue);
				} else if (m_FormatStr.IsEqualForward('^'))	{ m_FormatData.ShiftType = Detail::ShiftType::Center;	FormatReadParameter(m_FormatData.ShiftValue);
				} else if (m_FormatStr.IsEqualForward('0'))	{ m_FormatData.ShiftPrint = Detail::ShiftPrint::Zeros;

				} else if (m_FormatStr.IsLowerCase()) {	// Custom specifier
					const char* namePos = m_FormatStr.GetBufferCurrentPos();
					FormatStr().ParamGoTo(' ', '=');
					StringViewFormat name(namePos, m_FormatStr.GetBufferCurrentPos() - namePos);

					m_FormatStr.ParamGoToForward('=');
					m_FormatStr.IgnoreSpace();

					if (m_FormatStr.IsEqualForward('\'')) {
						const char* valuePos = m_FormatStr.GetBufferCurrentPos();
						FormatStr().ParamGoTo('\'');
						std::size_t valueSize = m_FormatStr.GetBufferCurrentPos() - valuePos;
						m_FormatData.AddSpecifier(name, StringViewFormat(valuePos, valueSize));
					} else if(m_FormatStr.IsADigit()) {
						std::intmax_t value = 0;
						m_FormatStr.ReadInt(value);
						m_FormatData.AddSpecifier(name, value);
					}
				}

				FormatStr().ParamGoTo(',');
			}
		}
	}

	template<typename CharFormat, typename CharBuffer, typename ...ContextArgs>
	bool BasicUnFormatContext<CharFormat, CharBuffer, ContextArgs...>::GetFormatIdx(FormatIdx& idx) {
		const CharFormat* mainSubFormat = m_FormatStr.GetBufferCurrentPos();

		// I : if there is no number specified : ':' or '}'
		if (m_FormatStr.IsEqualTo(':') || m_FormatStr.IsEqualTo('}')) {
			idx = m_ValuesIdx++;
			if (idx < m_ContextArgs.Size())	return true;
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
	bool BasicUnFormatContext<CharFormat, CharBuffer, ContextArgs...>::ParameterRead() {
		m_FormatStr.Forward();				// Skip {

		if (m_FormatStr.IsUpperCase()) {
			if		(m_FormatStr.IsEqualForward('C'))		GetColorValue();
			else if (m_FormatStr.IsEqualForward('T'))		GetTimerPrinted();
			else if (m_FormatStr.IsEqualForward('D'))		GetDatePrinted();
			else if (m_FormatStr.IsEqualForward('I'))		IgnoreParameter();
		} else {
			FormatIdx formatIdx;
			if (!GetFormatIdx(formatIdx))	return false;
			else {
				FormatDataType data;
				data.Clone(m_FormatData);
				m_FormatData = FormatDataType();

				Detail::AnsiColorMem colorMem(m_ColorMem);

				if (!m_FormatData.IsInit)	ParameterData();

				m_ContextArgs.ReadTypeFromIdx(*this, formatIdx);

				m_FormatData.Clone(data);
			}
		}

		m_FormatStr.GoOutOfParameter();		// Skip}
		return true;
	}

	template<typename CharFormat, typename CharBuffer, typename ...ContextArgs>
	bool BasicUnFormatContext<CharFormat, CharBuffer, ContextArgs...>::UnFormat() {

		bool error = false;

		while (!m_FormatStr.End() && !error) {

			if (CheckUntilNextParameter()) {
				if (m_FormatStr.IsEqualTo('{')) {
					if (!ParameterRead()) {
						error = true;
					}
				}
			}
			else if (!Check())
				error = true;
		}

		return error;
	}

	template<typename CharFormat, typename CharBuffer, typename ...ContextArgs>
	UnFormatContextError BasicUnFormatContext<CharFormat, CharBuffer, ContextArgs...>::MainUnFormat() {
		UnFormatContextError error;
		if (UnFormat())					error = UnFormatContextError((std::int16_t)m_FormatStr.GetBufferCurrentSize(), (std::int16_t)m_BufferIn.GetBufferSize());
		else if (!m_BufferIn.End())		error = UnFormatContextError((std::int16_t)m_FormatStr.GetBufferCurrentSize(), (std::int16_t)m_BufferIn.GetBufferSize());
		return error;
	}

	template<typename CharFormat, typename CharBuffer, typename ...ContextArgs>
	template<typename NewCharFormat, typename ...Args>
	void BasicUnFormatContext<CharFormat, CharBuffer, ContextArgs...>::LittleUnFormat(const std::basic_string_view<NewCharFormat> format, Args&& ...args) {
		BasicUnFormatContext<NewCharFormat, CharBuffer, Args...> child(format, *this, std::forward<Args>(args)...);
		child.Format();
		UpdateContextFromChild(child);
	}



	/////---------- Function ----------/////
	template<typename CharFormat = char, typename CharBuffer = CharFormat, typename ...Args>
	UnFormatContextError UnFormat(const std::basic_string_view<CharBuffer> buffer, const std::basic_string_view<CharFormat> format, Args&& ...args) {
		Fmt::BasicUnFormatContext<CharFormat, CharBuffer, Args...> context(format, buffer, std::forward<Args>(args)...);
		return context.MainUnFormat();
	}
	template<typename CharFormat = char, typename CharBuffer = CharFormat, std::size_t BUFFER_SIZE, std::size_t FORMAT_SIZE, typename ...Args>
	UnFormatContextError UnFormat(const CharBuffer (&buffer)[BUFFER_SIZE], const CharFormat (&format)[FORMAT_SIZE], Args&& ...args) {
		return UnFormat(std::basic_string_view<CharBuffer>(buffer), std::basic_string_view<CharFormat>(format), std::forward<Args>(args)...);
	}
}