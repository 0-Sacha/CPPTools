#pragma once

#include "../BasicUnFormatContext.h"

namespace CPPTools::Fmt::FormatFunc {


	template<typename T, typename CharFormat, typename CharBuffer>
	bool UnFormatContextReadFloat(BasicUnFormatContext<CharFormat, CharBuffer>& context, T& i, FormatDataType nbDecimal) {
		return false;
	}

	template<typename T, typename CharFormat, typename CharBuffer>
	bool UnFormatContextReadFloat(BasicUnFormatContext<CharFormat, CharBuffer>& context, T& i, FormatDataType nbDecimal, Detail::ShiftType st, FormatDataType shift, Detail::ShiftPrint sp = Detail::ShiftPrint::Space) {
		return false;
	}


}