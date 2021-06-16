#pragma once


#include "../UnFormater.h"

namespace CPPTools::Fmt {

	template<typename T>
	bool UnFormater::FormatReadInt(T& i) {
		T res = 0;
		bool sign = false, isValid = false;

		sign = BufferIsEqualForward('-');
		isValid = FormatIsADigit();

		while (FormatIsADigit()) { res = res * 10 + (FormatGetAndForward() - '0'); }

		if (isValid)	{ i = sign ? -res : res; }
		return isValid;
	}

	template<typename T>
	bool UnFormater::FormatReadUInt(T& i) {
		T res = (T)0;
		bool isValid = false;

		isValid = FormatIsADigit();

		while (FormatIsADigit()) { res = res * 10 + (FormatGetAndForward() - '0'); }

		if (isValid)	{ i = res; }
		return isValid;
	}
}