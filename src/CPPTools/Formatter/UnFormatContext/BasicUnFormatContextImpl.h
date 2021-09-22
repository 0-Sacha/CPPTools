#pragma once

#include "BasicUnFormatContext.h"
#include "BasicUnFormatContextClassImpl.h"


namespace CPPTools::Fmt {

	template<typename FormatContext>
	struct FormatType<UnFormatContextError, FormatContext>
	{
		static void Write(const UnFormatContextError t, FormatContext& context) {
			if (t)	context.LittleFormat("Format error idx : {} -- Buffer error idx : {}", t.FormatPosError, t.BufferPosError);
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
		if (!m_FormatStr.IsEqualTo('{'))	return FormatStr().ReadUInt(i);

		const CharFormat* const mainSubFormat = m_FormatStr.GetBufferCurrentPos();
		FormatIdx formatIdx = FORMAT_IDX_NOT_FOUND;
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

				if(m_FormatStr.IsUpperCase()) {

						 if (m_FormatStr.IsEqualForward('C')) { m_AnsiFormatterChange.HasMadeChange = true; m_FormatData.AnsiTextColorChange.HasChangeColor = true; m_AnsiTextCurrentColor = Detail::AnsiTextCurrentColor();	ReadAnsiTextColorParameter(); }
					else if (m_FormatStr.IsEqualForward('S')) { m_AnsiFormatterChange.HasMadeChange = true; m_FormatData.AnsiTextStyleChange.HasChangeStyle = true; m_AnsiTextCurrentStyle = Detail::AnsiTextCurrentStyle();	ReadAnsiTextStyleParameter(); }
					else if (m_FormatStr.IsEqualForward('F')) { m_AnsiFormatterChange.HasMadeChange = true; m_FormatData.AnsiTextFrontChange.HasChangeFront = true; m_AnsiTextCurrentFront = Detail::AnsiTextCurrentFront();	ReadAnsiTextFrontParameter(); }
					
					else if (m_FormatStr.IsEqualForward('B')) { m_FormatData.IntPrint = Detail::ValueIntPrint::Bin;	FormatReadParameter(m_FormatData.DigitSize); }
					else if (m_FormatStr.IsEqualForward('X')) { m_FormatData.IntPrint = Detail::ValueIntPrint::Hex;	FormatReadParameter(m_FormatData.DigitSize); }
					else if (m_FormatStr.IsEqualForward('O')) { m_FormatData.IntPrint = Detail::ValueIntPrint::Oct;	FormatReadParameter(m_FormatData.DigitSize); }
					else if (m_FormatStr.IsEqualForward('D')) { m_FormatData.IntPrint = Detail::ValueIntPrint::Int;	FormatReadParameter(m_FormatData.DigitSize); }

					else if (m_FormatStr.IsEqualForward('L')) { m_FormatData.PrintStyle = Detail::PrintStyle::LowerCase; }
					else if (m_FormatStr.IsEqualForward('U')) { m_FormatData.PrintStyle = Detail::PrintStyle::UpperCase; }

				} else if(!m_FormatStr.IsLowerCase()) {

					if (m_FormatStr.IsEqualForward('{')) {		// Forward specifier
						FormatIdx dataIdx;
						GetFormatIdx(dataIdx);
						m_ContextArgs.GetParameterDataFromIdx(*this, dataIdx);
						m_FormatStr.Forward();
					}
					else if (m_FormatStr.IsEqualForward('=')) { m_FormatData.TrueValue = true; }

					else if (m_FormatStr.IsEqualForward('.')) { FormatReadParameter(m_FormatData.FloatPrecision); }

					else if (m_FormatStr.IsEqualForward('>')) { m_FormatData.ShiftType = Detail::ShiftType::Right;	FormatReadParameter(m_FormatData.ShiftValue); }
					else if (m_FormatStr.IsEqualForward('<')) { m_FormatData.ShiftType = Detail::ShiftType::Left;	FormatReadParameter(m_FormatData.ShiftValue); }
					else if (m_FormatStr.IsEqualForward('^')) { m_FormatData.ShiftType = Detail::ShiftType::Center;	FormatReadParameter(m_FormatData.ShiftValue); }
					else if (m_FormatStr.IsEqualForward('0')) { m_FormatData.ShiftPrint = Detail::ShiftPrint::Zeros;											  }

				} else {
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
					}
					else if (m_FormatStr.IsADigit()) {
						Detail::FormatDataType value = 0;
						m_FormatStr.ReadInt(value);
						m_FormatData.AddSpecifier(name, value);
					}
					else if (m_FormatStr.IsEqualForward('{')) {
						Detail::FormatDataType value = 0;
						FormatIdx idx = 0;
						bool get = GetFormatIdx(idx);
						m_FormatStr.IsEqualForward('}');
						m_ContextArgs.GetFormatValueAt(value, idx);
						m_FormatData.AddSpecifier(name, value);
					}
				}

				m_FormatStr.ParamGoTo(',');
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
			if		(m_FormatStr.IsEqualForward('C'))		ReadAnsiTextColorParameter();
			else if (m_FormatStr.IsEqualForward('T'))		ReadTimerParameter();
			else if (m_FormatStr.IsEqualForward('D'))		ReadDateParameter();
			else if (m_FormatStr.IsEqualForward('I'))		IgnoreParameter();
		} else {
			FormatIdx formatIdx;
			if (!GetFormatIdx(formatIdx))	return false;
			else {
				FormatDataType data;
				data.Clone(m_FormatData);
				m_FormatData = FormatDataType();

				Detail::AnsiTextCurrentColor ansiTextCurrentColor(m_AnsiTextCurrentColor);
				Detail::AnsiTextCurrentStyle ansiTextCurrentStyle(m_AnsiTextCurrentStyle);

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