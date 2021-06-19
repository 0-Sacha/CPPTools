#pragma once

#include "BasicUnFormatContext.h"

#include "BaseFormat/BaseFormat.h"
#include "BaseFormat/ColorFormat.h"
#include "BaseFormat/Chrono.h"
#include "BaseFormat/String.h"

namespace CPPTools::Fmt {

	template<typename CharFormat, typename CharBuffer>
	BasicUnFormatContext<CharFormat, CharBuffer>::BasicUnFormatContext(const std::basic_string_view<CharFormat> format, const std::basic_string_view<CharBuffer> buffer)
		: m_Buffer(buffer.data())
		, m_SubBuffer(buffer.data())
		, m_BufferEnd(buffer.data() + buffer.size())
		, m_BufferSize(buffer.size())
		, m_Format(format.data())
		, m_SubFormat(format.data())
		, m_FormatEnd(format.data() + format.size())
		, m_FormatSize(format.size())
		, m_ValuesIdx(0)
	{
	}

	template<typename CharFormat, typename CharBuffer>
	void BasicUnFormatContext<CharFormat, CharBuffer>::GetColorValue() {

	}

	template<typename CharFormat, typename CharBuffer>
	void BasicUnFormatContext<CharFormat, CharBuffer>::GetTimerPrinted() {

	}

	template<typename CharFormat, typename CharBuffer>
	void BasicUnFormatContext<CharFormat, CharBuffer>::GetDatePrinted() {

	}

	template<typename CharFormat, typename CharBuffer>
	void BasicUnFormatContext<CharFormat, CharBuffer>::IgnoreParameter() {
		if(BufferIsEqualForward(':')) {

		}
		else
			BufferIgnoreSpace();
	}
}