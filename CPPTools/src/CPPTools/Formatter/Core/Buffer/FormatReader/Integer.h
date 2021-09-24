#pragma once

#include "../Format.h"

namespace CPPTools::Fmt::Detail {
	template<typename CharFormat>
	template<typename T>
	bool FormatterMemoryFormat<CharFormat>::ReadInt(T& i) {
		T res = (T)0;
		bool sign		= Base::IsEqualForward('-');
		bool isValid	= Base::IsADigit();

		while (Base::IsADigit())
			res = res * 10 + T(Base::GetAndForward() - '0');

		if (isValid)
			i = sign ? -res : res;

		return isValid;
	}

	template<typename CharFormat>
	template<typename T>
	bool FormatterMemoryFormat<CharFormat>::ReadUInt(T& i) {
		T res = (T)0;
		bool isValid = Base::IsADigit();

		while (Base::IsADigit())
			res = res * 10 + (Base::GetAndForward() - '0');

		if (isValid) i = res;

		return isValid;
	}
}