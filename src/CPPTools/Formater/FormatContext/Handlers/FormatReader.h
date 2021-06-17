#pragma once

#include "../BasicFormatContext.h"

namespace CPPTools::Fmt {

	template<typename CharFormat, typename CharBuffer>
	template<typename T>
	bool BasicFormatContext<CharFormat, CharBuffer>::FormatReadInt(T& i) {
		T res = 0;
		bool sign = false, isValid = false;

		sign	= FormatIsEqualForward('-');
		isValid = FormatIsADigit();

		while (FormatIsADigit()) { res = res * 10 + (FormatGetAndForward() - '0'); }
		if (isValid)	{ i = sign ? -res : res; }
		return isValid;
	}

	template<typename CharFormat, typename CharBuffer>
	template<typename T>
	bool BasicFormatContext<CharFormat, CharBuffer>::FormatReadUInt(T& i) {
		T res = (T)0;
		bool isValid = false;

		isValid = FormatIsADigit();

		while (FormatIsADigit()) { res = res * 10 + (FormatGetAndForward() - '0'); }

		if (isValid)	{ i = res; }
		return isValid;
	}
}