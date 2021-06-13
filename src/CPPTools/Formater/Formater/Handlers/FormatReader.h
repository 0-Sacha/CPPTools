#pragma once

#include "../Formater.h"

namespace CPPTools::Fmt {

	template<typename T>
	bool Formater::FormatReadInt(T& i) {
		T res = 0;
		bool sign = false, isValid = false;

		sign	= FormatIsEqNext('-');
		isValid = FormatIsADigit();

		while (FormatIsADigit()) { res = res * 10 + (FormatGetAndNext() - '0'); }
		if (isValid)	{ i = sign ? -res : res; }
		return isValid;
	}

	template<typename T>
	bool Formater::FormatReadUInt(T& i) {
		T res = (T)0;
		bool isValid = false;

		isValid = FormatIsADigit();

		while (FormatIsADigit()) { res = res * 10 + (FormatGetAndNext() - '0'); }

		if (isValid)	{ i = res; }
		return isValid;
	}
}