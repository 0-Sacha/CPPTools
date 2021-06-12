#pragma once

#include "../UnFormater.h"

namespace CPPTools::Fmt::FormatFunc {


	template<typename T>
	bool FormaterUnParseFloat(UnFormater& formater, T& i, int16_t nbDecimal) {
		return false;
	}

	template<typename T>
	bool FormaterUnParseFloat(UnFormater& formater, T& i, int16_t nbDecimal, ShiftType st, int32_t shift, ShiftPrint sp = ShiftPrint::Space) {
		return false;
	}


}