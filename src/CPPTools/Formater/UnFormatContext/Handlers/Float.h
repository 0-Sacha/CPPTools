#pragma once

#include "../BasicUnFormatContext.h"

namespace CPPTools::Fmt::FormatFunc {


	template<typename T, typename Char>
	bool UnFormatContextReadFloat(BasicUnFormatContext<Char>& formater, T& i, std::int16_t nbDecimal) {
		return false;
	}

	template<typename T, typename Char>
	bool UnFormatContextReadFloat(BasicUnFormatContext<Char>& formater, T& i, std::int16_t nbDecimal, Detail::ShiftType st, std::int32_t shift, Detail::ShiftPrint sp = Detail::ShiftPrint::Space) {
		return false;
	}


}