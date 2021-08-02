#pragma once

#include "../BufferIn.h"
#include "Integer.h"

namespace CPPTools::Fmt::Detail {

	template<typename CharBuffer>
	template<typename T>
	bool FormatterMemoryBufferIn<CharBuffer>::BasicReadFloat(T& i, FormatDataType nbDecimal) {
		return false;
	}

	template<typename CharBuffer>
	template<typename T>
	bool FormatterMemoryBufferIn<CharBuffer>::BasicReadFloat(T& i, FormatDataType nbDecimal, ShiftType st, FormatDataType shift, ShiftPrint sp) {
		return false;
	}


}