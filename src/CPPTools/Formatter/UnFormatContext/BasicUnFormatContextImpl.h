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
	void BasicUnFormatContext<CharFormat, CharBuffer, ContextArgs...>::ParameterData() {
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

				} else if (FormatIsEqualForward('C'))	{ m_FormatData.HasChangeColor = true; m_ColorMem = Detail::AnsiColorMem(); GetColorValue();

				} else if (FormatIsEqualForward('='))	{ m_FormatData.BaseValue = true;

				} else if (FormatIsEqualForward('b'))	{ m_FormatData.IntPrint = Detail::ValueIntPrint::Bin;	FormatReadParameter(m_FormatData.Precision);
				} else if (FormatIsEqualForward('x'))	{ m_FormatData.IntPrint = Detail::ValueIntPrint::Hex;	FormatReadParameter(m_FormatData.Precision);
				} else if (FormatIsEqualForward('o'))	{ m_FormatData.IntPrint = Detail::ValueIntPrint::Oct;	FormatReadParameter(m_FormatData.Precision);
				} else if (FormatIsEqualForward('d'))	{ m_FormatData.IntPrint = Detail::ValueIntPrint::Int;	FormatReadParameter(m_FormatData.Precision);
				} else if (FormatIsEqualForward('.'))	{ FormatReadParameter(m_FormatData.FloatPrecision);

				} else if (FormatIsEqualForward('>'))	{ m_FormatData.ShiftType = Detail::ShiftType::Right;	FormatReadParameter(m_FormatData.ShiftValue);
				} else if (FormatIsEqualForward('<'))	{ m_FormatData.ShiftType = Detail::ShiftType::Left;		FormatReadParameter(m_FormatData.ShiftValue);
				} else if (FormatIsEqualForward('^'))	{ m_FormatData.ShiftType = Detail::ShiftType::Center;	FormatReadParameter(m_FormatData.ShiftValue);
				} else if (FormatIsEqualForward('0'))	{ m_FormatData.ShiftPrint = Detail::ShiftPrint::Zeros;

				} else if (FormatIsLowerCase()) {	// Custom specifier
					const char* namePos = GetSubFormat();
					FormatParamGoTo(' ', '=');
					StringViewFormat name(namePos, GetSubFormat() - namePos);

					FormatParamGoToForward('=');
					FormatIgnoreSpace();

					if (FormatIsEqualForward('\'')) {
						const char* valuePos = GetSubFormat();
						FormatParamGoTo('\'');
						std::size_t valueSize = GetSubFormat() - valuePos;
						m_FormatData.AddSpecifier(name, StringViewFormat(valuePos, valueSize));
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
	bool BasicUnFormatContext<CharFormat, CharBuffer, ContextArgs...>::GetFormatIdx(FormatIdx& idx) {
		const CharFormat* mainSubFormat = m_SubFormat;

		// I : if there is no number specified : ':' or '}'
		if (FormatIsEqualTo(':') || FormatIsEqualTo('}')) {
			idx = m_ValuesIdx++;
			if (idx < m_ContextArgsSize)	return true;
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
	bool BasicUnFormatContext<CharFormat, CharBuffer, ContextArgs...>::ParameterRead() {
		FormatForward();				// Skip {

		if (FormatIsEqualForward('C'))			GetColorValue();
		else if (FormatIsEqualForward('T'))		GetTimerPrinted();
		else if (FormatIsEqualForward('D'))		GetDatePrinted();
		else if (FormatIsEqualForward('I'))		IgnoreParameter();
		else {
			FormatIdx formatIdx;
			if (!GetFormatIdx(formatIdx))	return false;
			else {
				FormatData data;
				data.Clone(m_FormatData);
				m_FormatData = FormatDataType();

				Detail::AnsiColorMem colorMem(m_ColorMem);

				if (!m_FormatData.IsInit)	ParameterData();

				m_ContextArgs.ReadTypeFromIdx(*this, formatIdx);

				m_FormatData.Clone(data);
			}
		}

		FormatGoOutOfParameter();		// Skip}
		return true;
	}

	template<typename CharFormat, typename CharBuffer, typename ...ContextArgs>
	bool BasicUnFormatContext<CharFormat, CharBuffer, ContextArgs...>::UnFormat() {

		bool error = false;

		while (!FormatEnd() && !error) {

			if (CheckUntilNextParameter()) {
				if (FormatIsEqualTo('{')) {
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
		if (UnFormat())				error = UnFormatContextError((std::int16_t)GetFormatSize(), (std::int16_t)GetBufferSize());
		else if (!BufferEnd())		error = UnFormatContextError((std::int16_t)GetFormatSize(), (std::int16_t)GetBufferSize());
		return error;
	}

	template<typename CharFormat, typename CharBuffer, typename ...ContextArgs>
	template<typename NewCharFormat, typename ...Args>
	void BasicUnFormatContext<CharFormat, CharBuffer, ContextArgs...>::LittleUnFormat(const std::basic_string_view<NewCharFormat> format, Args&& ...args) {
		BasicUnFormatContext<NewCharFormat, CharBuffer, Args...> newContext(format, *this, std::forward<Args>(args)...);
		newContext.Format();
		newContext.UpdateOldContext(*this);
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