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

	/////---------- Default Print Rec ----------/////
	template<typename CharFormat, typename CharBuffer>
	template<typename T, typename ...Args>
	inline void BasicUnFormatContext<CharFormat, CharBuffer>::UnFormatTypeFromIdx(FormatIdx idx, T& t, Args&& ...args) {
		if (idx == 0)	UnFormatType<GetBaseType<T>>::Read(t, *this);
		else			UnFormatTypeFromIdx(idx - 1, std::forward<Args>(args)...);
	}

	/////---------- Data Print Rec ----------/////
	template<typename CharFormat, typename CharBuffer>
	inline void BasicUnFormatContext<CharFormat, CharBuffer>::GetParameterData(FormatIdx idx) {}

	template<typename CharFormat, typename CharBuffer>
	template<typename T, typename ...Args>
	inline void BasicUnFormatContext<CharFormat, CharBuffer>::GetParameterData(FormatIdx idx, const T& t, Args&& ...args) {
		if (idx == 0)	Detail::CopyFormatData<T>::Copy(m_FormatData, t);
		else			GetParameterData(idx - 1, std::forward<Args>(args)...);
	}

	/////---------- NamedArgs Print Rec ----------/////
	template<typename CharFormat, typename CharBuffer>
	template<typename T, typename ...Args, typename CharName>
	inline void BasicUnFormatContext<CharFormat, CharBuffer>::GetNamedArgsIdx(FormatIdx& idx, FormatIdx currentIdx, const StringViewNamedArgs<T, CharName, CharFormat>& t, Args&& ...args) {
		if (FormatNextIsANamedArgs(t.GetName()))	idx = currentIdx;
		else										GetNamedArgsIdx(idx, currentIdx + 1, std::forward<Args>(args)...);
	}
	template<typename CharFormat, typename CharBuffer>
	template<typename T, typename ...Args, typename CharName>
	inline void BasicUnFormatContext<CharFormat, CharBuffer>::GetNamedArgsIdx(FormatIdx& idx, FormatIdx currentIdx, const StringNamedArgs<T, CharName, CharFormat>& t, Args&& ...args) {
		if (FormatNextIsANamedArgs(t.GetName()))	idx = currentIdx;
		else										GetNamedArgsIdx(idx, currentIdx + 1, std::forward<Args>(args)...);
	}
	template<typename CharFormat, typename CharBuffer>
	template<typename T, typename ...Args>
	inline void BasicUnFormatContext<CharFormat, CharBuffer>::GetNamedArgsIdx(FormatIdx& idx, FormatIdx currentIdx, const T& t, Args&& ...args) {
		GetNamedArgsIdx(idx, currentIdx + 1, std::forward<Args>(args)...);
	}

	/////---------- Impl ----------/////
	template<typename CharFormat, typename CharBuffer>
	template<typename ...Args>
	void BasicUnFormatContext<CharFormat, CharBuffer>::ParameterData(Args&& ...args) {
		if (FormatIsEqualTo(':')) {
			m_FormatData.HasSpec = true;
			while (!FormatIsEndOfParameter()) {
				FormatForward();
				FormatIgnoreSpace();

				if (BufferIsEqualForward('{')) {		// Forward specifier
					std::uint8_t dataIdx;
					if (!FormatReadUInt(dataIdx))
						dataIdx = m_ValuesIdx++;
					GetParameterData(dataIdx, std::forward<Args>(args)...);
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

	template<typename CharFormat, typename CharBuffer>
	template<typename ...Args>
	bool BasicUnFormatContext<CharFormat, CharBuffer>::GetFormatIdx(FormatIdx& idx, Args&& ...args) {
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
	bool BasicUnFormatContext<CharFormat, CharBuffer>::ParameterRead(Args&& ...args) {
		FormatForward();				// Skip {

		if (FormatIsEqualForward('C'))			GetColorValue();
		else if (FormatIsEqualForward('T'))		GetTimerPrinted();
		else if (FormatIsEqualForward('D'))		GetDatePrinted();
		else if (FormatIsEqualForward('I'))		IgnoreParameter();
		else {
			FormatIdx formatIdx;
			if (!GetFormatIdx(formatIdx, std::forward<Args>(args)...))	return false;
			else {
				FormatData data;
				data.Clone(m_FormatData);
				m_FormatData = FormatData();

				Detail::AnsiColorMem colorMem(m_ColorMem);

				if (!m_FormatData.IsInit)			ParameterData(std::forward<Args>(args)...);

				UnFormatTypeFromIdx(formatIdx, std::forward<Args>(args)...);

				m_FormatData.Clone(data);
			}
		}

		FormatGoOutOfParameter();		// Skip}
		return true;
	}

	template<typename CharFormat, typename CharBuffer>
	template<typename ...Args>
	bool BasicUnFormatContext<CharFormat, CharBuffer>::UnFormat(Args&& ...args) {

		bool error = false;

		while (!FormatEnd() && !error) {

			if (CheckUntilNextParameter()) {
				if (FormatIsEqualTo('{')) {
					if (!ParameterRead(std::forward<Args>(args)...)) {
						error = true;
					}
				}
			}
			else if (!Check())
				error = true;
		}

		return error;
	}

	template<typename CharFormat, typename CharBuffer>
	template<typename NewCharFormat, typename ...Args>
	void BasicUnFormatContext<CharFormat, CharBuffer>::LittleUnFormat(const std::basic_string_view<NewCharFormat> format, Args&& ...args) {
		BasicFormatContext<NewCharFormat, CharBuffer> newContext(format, *this);
		newContext.UnFormat(std::forward<Args>(args)...);
		newContext.UpdateOldContext(*this);
	}

	template<typename CharFormat, typename CharBuffer>
	template<typename ...Args>
	void BasicUnFormatContext<CharFormat, CharBuffer>::LittleUnFormat(const std::basic_string_view<CharFormat> format, Args&& ...args) {
		// Copy
		const CharFormat* const mainFormat		= m_Format;
		const CharFormat* const mainSubFormat	= m_SubFormat;
		const CharFormat* const mainFormatEnd	= m_FormatEnd;
		const std::size_t mainFormatSize		= m_FormatSize;
		std::uint8_t mainIdx = m_ValuesIdx;

		// Assign new value
		m_Format		= format.data();
		m_SubFormat		= format.data();
		m_FormatEnd		= format.data() + format.size();
		m_FormatSize	= format.size();
		m_ValuesIdx		= 0;

		bool error = UnFormat(std::forward<Args>(args)...);

		// Assign old value
		m_Format		= mainFormat;
		m_SubFormat		= mainSubFormat;
		m_FormatEnd		= mainFormatEnd;
		m_FormatSize	= mainFormatSize;
		m_ValuesIdx		= mainIdx;

		return error;
	}

	template<typename CharFormat, typename CharBuffer>
	template<typename ...Args>
	UnFormatContextError BasicUnFormatContext<CharFormat, CharBuffer>::MainUnFormat(Args&& ...args) {
		UnFormatContextError error;
		if (UnFormat(args...))		error = UnFormatContextError(GetFormatSize(), GetBufferSize());
		else if (!BufferEnd())		error = UnFormatContextError(GetFormatSize(), GetBufferSize());
		return error;
	}



	/////---------- Function ----------/////
	template<typename CharFormat = char, typename CharBuffer = CharFormat, typename ...Args>
	UnFormatContextError UnFormat(const std::basic_string_view<CharBuffer> buffer, const std::basic_string_view<CharFormat> format, Args&& ...args) {
		Fmt::BasicUnFormatContext<CharFormat, CharBuffer> context(format, buffer);
		return context.MainUnFormat(std::forward<Args>(args)...);
	}
	template<typename CharFormat = char, typename CharBuffer = CharFormat, std::size_t BUFFER_SIZE, std::size_t FORMAT_SIZE, typename ...Args>
	UnFormatContextError UnFormat(const CharBuffer (&buffer)[BUFFER_SIZE], const CharFormat (&format)[FORMAT_SIZE], Args&& ...args) {
		return UnFormat(std::basic_string_view<CharBuffer>(buffer), std::basic_string_view<CharFormat>(format), std::forward<Args>(args)...);
	}
}