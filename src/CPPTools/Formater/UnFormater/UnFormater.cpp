
#include "CPPToolspch.h"

#include "UnFormater.h"

#include "../BaseFormat/BaseFormat.h"
#include "../BaseFormat/ColorFormat.h"
#include "../BaseFormat/Chrono.h"
#include "../BaseFormat/String.h"

namespace CPPTools::Fmt {

	UnFormater::UnFormater(const char* const format, size_t formatSize, const char* const buffer, size_t bufferSize)
		: m_Buffer(buffer), m_SubBuffer(buffer), m_BufferSize(bufferSize), m_Format(format), m_SubFormat(format), m_FormatSize(formatSize)
	{
	}


	void UnFormater::FormatReadRec(uint8_t idx) { }
	void UnFormater::ParameterDataRec(uint8_t idx) { }
	void UnFormater::FormatReadRecNamedArgs(const char* const name) { }


	void UnFormater::GetColorValue() {

	}

	void UnFormater::GetTimerPrinted() {

	}

	void UnFormater::GetDatePrinted() {

	}

	void UnFormater::IgnoreParameter() {
		if(FormatIsEqNext(':')) {

		}
		else
			BufferIgnoreSpace();
	}

	bool UnFormater::FormatNextIsSame(const char* str) {
		const char* format = m_SubFormat;
		bool isSame = true;
		while (isSame && *str != 0 && *format != '}')	{ if (*str++ != *format++) isSame = false; }
		if (isSame && *str != 0)	isSame = false;
		if (isSame)					m_SubFormat = format;
		return isSame;
	}


	bool UnFormater::BufferNextIsSame(const char* str) {
		const char* buffer = m_SubBuffer;
		bool isSame = true;
		while (isSame && *str != 0 && *buffer != '}')	{ if (*str++ != *buffer++) isSame = false; }
		if (isSame && *str != 0)	isSame = false;
		if (isSame)					m_SubBuffer = buffer;
		return isSame;
	}

}