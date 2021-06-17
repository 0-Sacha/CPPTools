
#include "BasicUnFormatContext.h"

#include "BaseFormat/BaseFormat.h"
#include "BaseFormat/ColorFormat.h"
#include "BaseFormat/Chrono.h"
#include "BaseFormat/String.h"

namespace CPPTools::Fmt {

	template<typename Char>
	BasicUnFormatContext<Char>::BasicUnFormatContext(const std::basic_string_view<Char> format, const std::basic_string_view<Char> buffer)
		: m_Buffer(buffer.data())
		, m_SubBuffer(buffer.data())
		, m_BufferEnd(buffer.data() + buffer.size())
		, m_BufferSize(buffer.size())
		, m_Format(format.data())
		, m_SubFormat(format.data())
		, m_FormatEnd(format.data() + format.size())
		, m_FormatSize(format.size())
	{
	}

	template<typename Char>
	void BasicUnFormatContext<Char>::GetColorValue() {

	}

	template<typename Char>
	void BasicUnFormatContext<Char>::GetTimerPrinted() {

	}

	template<typename Char>
	void BasicUnFormatContext<Char>::GetDatePrinted() {

	}

	template<typename Char>
	void BasicUnFormatContext<Char>::IgnoreParameter() {
		if(BufferIsEqualForward(':')) {

		}
		else
			BufferIgnoreSpace();
	}
}