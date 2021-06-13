#pragma once

#include "../UnFormater.h"

namespace CPPTools::Fmt::FormatFunc {


	template<typename T>
	bool FormaterUnParseFloat(UnFormater& formater, T& i, std::int16_t nbDecimal) {
		return false;
	}

	template<typename T>
	bool FormaterUnParseFloat(UnFormater& formater, T& i, std::int16_t nbDecimal, Detail::ShiftType st, std::int32_t shift, Detail::ShiftPrint sp = Detail::ShiftPrint::Space) {
		return false;
	}


}