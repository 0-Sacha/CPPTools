#pragma once

#include "BasicUnFormatContext.h"
#include "BasicUnFormatContextClassImpl.h"


#include "Handlers/FormatBufferReader.h"
#include "Handlers/FormatReader.h"

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
			return UnFormatType<GetBaseType<T>, BasicUnFormatContext<CharFormat, CharBuffer, ContextArgs...>>::Read(t.GetValue(), context);
		}
	};

	template<typename T, typename CharName, typename CharFormat, typename CharBuffer, typename ...ContextArgs>
	struct UnFormatType<StringNamedArgs<T, CharName>, BasicUnFormatContext<CharFormat, CharBuffer, ContextArgs...>>
	{
		inline static bool Read(StringNamedArgs<T, CharName>& t, BasicUnFormatContext<CharFormat, CharBuffer, ContextArgs...>& context) {
			return UnFormatType<GetBaseType<T>, BasicUnFormatContext<CharFormat, CharBuffer, ContextArgs...>>::Read(t.GetValue(), context);
		}
	};

	/////---------- Default Print Rec ----------/////
	template<typename CharFormat, typename CharBuffer, typename ...ContextArgs>
	inline void BasicUnFormatContext<CharFormat, CharBuffer, ContextArgs...>::UnFormatTypeFromIdxRec(FormatIdx idx) {}

	template<typename CharFormat, typename CharBuffer, typename ...ContextArgs>
	template<typename T, typename ...Args>
	inline void BasicUnFormatContext<CharFormat, CharBuffer, ContextArgs...>::UnFormatTypeFromIdxRec(FormatIdx idx, T& t, Args&& ...args) {
		if (idx == 0)	UnFormatType<GetBaseType<T>, BasicUnFormatContext<CharFormat, CharBuffer, ContextArgs...>>::Read(t, *this);
		else			UnFormatTypeFromIdxRec(idx - 1, std::forward<Args>(args)...);
	}

	/////---------- Data Print Rec ----------/////
	template<typename CharFormat, typename CharBuffer, typename ...ContextArgs>
	inline void BasicUnFormatContext<CharFormat, CharBuffer, ContextArgs...>::GetParameterDataRec(FormatIdx idx) {}

	template<typename CharFormat, typename CharBuffer, typename ...ContextArgs>
	template<typename T, typename ...Args>
	inline void BasicUnFormatContext<CharFormat, CharBuffer, ContextArgs...>::GetParameterDataRec(FormatIdx idx, const T& t, Args&& ...args) {
		if (idx == 0)	Detail::CopyFormatData<T>::Copy(m_FormatData, t);
		else			GetParameterDataRec(idx - 1, std::forward<Args>(args)...);
	}

	/////---------- NamedArgs Print Rec ----------/////
	template<typename CharFormat, typename CharBuffer, typename ...ContextArgs>
	inline void BasicUnFormatContext<CharFormat, CharBuffer, ContextArgs...>::GetNamedArgsIdxRec(FormatIdx& idx, FormatIdx currentIdx) { idx = FormatIdxNotFound; }
	template<typename CharFormat, typename CharBuffer, typename ...ContextArgs>
	template<typename T, typename ...Args, typename CharName>
	inline void BasicUnFormatContext<CharFormat, CharBuffer, ContextArgs...>::GetNamedArgsIdxRec(FormatIdx& idx, FormatIdx currentIdx, const StringViewNamedArgs<T, CharName>& t, Args&& ...args) {
		if (FormatNextIsANamedArgs(t.GetName()))	idx = currentIdx;
		else										GetNamedArgsIdxRec(idx, currentIdx + 1, std::forward<Args>(args)...);
	}
	template<typename CharFormat, typename CharBuffer, typename ...ContextArgs>
	template<typename T, typename ...Args, typename CharName>
	inline void BasicUnFormatContext<CharFormat, CharBuffer, ContextArgs...>::GetNamedArgsIdxRec(FormatIdx& idx, FormatIdx currentIdx, const StringNamedArgs<T, CharName>& t, Args&& ...args) {
		if (FormatNextIsANamedArgs(t.GetName()))	idx = currentIdx;
		else										GetNamedArgsIdxRec(idx, currentIdx + 1, std::forward<Args>(args)...);
	}
	template<typename CharFormat, typename CharBuffer, typename ...ContextArgs>
	template<typename T, typename ...Args>
	inline void BasicUnFormatContext<CharFormat, CharBuffer, ContextArgs...>::GetNamedArgsIdxRec(FormatIdx& idx, FormatIdx currentIdx, const T& t, Args&& ...args) {
		GetNamedArgsIdxRec(idx, currentIdx + 1, std::forward<Args>(args)...);
	}

	/////---------- Impl ----------/////
	template<typename CharFormat, typename CharBuffer, typename ...ContextArgs>
	void BasicUnFormatContext<CharFormat, CharBuffer, ContextArgs...>::ParameterData() {
		if (FormatIsEqualTo(':')) {
			m_FormatData.HasSpec = true;
			while (!FormatIsEndOfParameter()) {
				FormatForward();
				FormatIgnoreSpace();

				if (BufferIsEqualForward('{')) {		// Forward specifier
					std::uint8_t dataIdx;
					if (!FormatReadUInt(dataIdx)) dataIdx = m_ValuesIdx++;
					std::apply([&](auto &&... args) { GetParameterDataRec(dataIdx, args...); }, m_ContextArgs);
					FormatForward();
				} else if (BufferIsEqualForward('b')) { m_FormatData.IntPrint = Detail::ValueIntPrint::Bin;	FormatReadUInt(m_FormatData.Precision);
				} else if (BufferIsEqualForward('x')) { m_FormatData.IntPrint = Detail::ValueIntPrint::Hex;	FormatReadUInt(m_FormatData.Precision);
				} else if (BufferIsEqualForward('o')) { m_FormatData.IntPrint = Detail::ValueIntPrint::Oct;	FormatReadUInt(m_FormatData.Precision);
				} else if (BufferIsEqualForward('d')) { m_FormatData.IntPrint = Detail::ValueIntPrint::Int;	FormatReadUInt(m_FormatData.Precision);
				} else if (FormatIsLowerCase()) {	// Custom specifier
					const char c = FormatGetAndForward();
					std::int8_t i = 0;
					FormatReadInt(i);
					m_FormatData.AddSpecifier(c, i);
				} else if (BufferIsEqualForward('C')) {
				} else if (BufferIsEqualForward('>')) { m_FormatData.ShiftType = Detail::ShiftType::Right;	FormatReadUInt(m_FormatData.ShiftValue);
				} else if (BufferIsEqualForward('<')) { m_FormatData.ShiftType = Detail::ShiftType::Left;		FormatReadUInt(m_FormatData.ShiftValue);
				} else if (BufferIsEqualForward('^')) { m_FormatData.ShiftType = Detail::ShiftType::Center;	FormatReadUInt(m_FormatData.ShiftValue);
				} else if (BufferIsEqualForward('.')) { FormatReadUInt(m_FormatData.FloatPrecision);
				} else if (BufferIsEqualForward('S')) { FormatReadUInt(m_FormatData.Size);
				} else if (BufferIsEqualForward('B')) { FormatReadUInt(m_FormatData.Begin);
				} else if (BufferIsEqualForward('\n')) { m_FormatData.ContainerPrintStyle = Detail::ContainerPrintStyle::NewLine;
				} else if (BufferIsEqualForward('=')) { m_FormatData.BaseValue = true; }

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
		std::apply([&](auto &&... args) { GetNamedArgsIdxRec(idx, 0, args...); }, m_ContextArgs);
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
				m_FormatData = FormatData();

				Detail::AnsiColorMem colorMem(m_ColorMem);

				if (!m_FormatData.IsInit)			ParameterData();

				std::apply([&](auto &&... args) { UnFormatTypeFromIdxRec(formatIdx, args...); }, m_ContextArgs);

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