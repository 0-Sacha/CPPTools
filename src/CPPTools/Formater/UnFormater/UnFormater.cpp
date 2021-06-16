
#include "CPPToolspch.h"

#include "UnFormater.h"

#include "../BaseFormat/BaseFormat.h"
#include "../BaseFormat/ColorFormat.h"
#include "../BaseFormat/Chrono.h"
#include "../BaseFormat/String.h"

namespace CPPTools::Fmt {

	UnFormater::UnFormater(const std::string_view format, const std::string_view buffer)
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


	void UnFormater::FormatReadRec(std::uint8_t idx) { }
	void UnFormater::ParameterDataRec(std::uint8_t idx) { }
	void UnFormater::FormatReadRecNamedArgs(const char* const name) { }


	void UnFormater::GetColorValue() {

	}

	void UnFormater::GetTimerPrinted() {

	}

	void UnFormater::GetDatePrinted() {

	}

	void UnFormater::IgnoreParameter() {
		if(BufferIsEqualForward(':')) {

		}
		else
			BufferIgnoreSpace();
	}
}